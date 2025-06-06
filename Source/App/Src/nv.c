#include "nv.h"
#include "stm32f030x6.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Note from RM0360 Reference Manual:
// The Flash memory interface preliminarily reads the value at the addressed main Flash
// memory location and checks that it has been erased. If not, the program operation is
// skipped and a warning is issued by the PGERR bit in FLASH_SR register. The only
// exception to this is when 0x0000 is programmed. In this case, the location is correctly
// programmed to 0x0000 and the PGERR bit is not set.

#define PageSize (1024)
#define ItemsOnPage (PageSize / sizeof(uint16_t))

volatile FLASH_TypeDef* Flash = (volatile FLASH_TypeDef*)FLASH_R_BASE;

typedef enum { First = 14, Second = 15 } Page;

static const size_t Page1Address = FLASH_BASE + ((uint16_t)First) * 1024;
static const size_t Page2Address = FLASH_BASE + ((uint16_t)Second) * 1024;

static Page CurrentPage = 0;
static uint16_t wearCycle = 0;

static bool nvLock(void);
static bool nvUnlock(void);
static bool nvCheckIsErased(Page value);
static bool nvGetLast(Page page, size_t* offset, uint16_t* value);
static bool nvErasePage(Page value);
static bool nvWait(void);
static uint16_t nvReadWord(Page page, size_t offset);
static bool nvWriteWord(Page page, size_t offset, uint16_t value);

bool nvInit(void) {
    bool result = false;

    bool isFirstErased = nvCheckIsErased(First);
    bool isSecondErased = nvCheckIsErased(Second);

    if (isFirstErased && isSecondErased) {
        CurrentPage = First;
        result = true;
    } else if (isFirstErased && !isSecondErased) {
        CurrentPage = Second;
        result = true;
    } else if (!isFirstErased && isSecondErased) {
        CurrentPage = First;
        result = true;
    } else if (!isFirstErased && !isSecondErased) {
        size_t offsetFirst = 0, offsetSecond = 0;
        uint16_t _;

        nvGetLast(First, &offsetFirst, &_);
        nvGetLast(Second, &offsetSecond, &_);

        if (offsetFirst < offsetSecond) {
            if (nvErasePage(Second)) {
                result = true;
            }

            CurrentPage = First;
        } else {
            if (nvErasePage(First)) {
                result = true;
            }

            CurrentPage = Second;
        }
    }

    return result;
}

UserSource nvGetState(void) {
    UserSource result = UserSource1;
    size_t _;
    uint16_t value;

    if (nvGetLast(CurrentPage, &_, &value)) {
        wearCycle = (value >> 3) & 0x1FFF;
        result = (UserSource)(value & 0x7);
    }

    return result;
}

bool nvSetState(UserSource value) {
    bool result = false;

    size_t offset;
    uint16_t _;

    if (nvGetLast(CurrentPage, &offset, &_)) {
        offset += 1;

        if (offset < ItemsOnPage) {
            uint16_t word = (uint16_t)value;
            word = (word & 0x7) | (wearCycle << 3);
            result = nvWriteWord(CurrentPage, offset, word);
        } else {
            uint16_t newWearCycle = wearCycle + 1;
            uint16_t word = (uint16_t)value;
            word = (word & 0x7) | (newWearCycle << 3);

            Page anotherPage = (CurrentPage == First) ? Second : First;
            bool isWritten = nvWriteWord(anotherPage, 0, word);
            bool isErased = nvErasePage(CurrentPage);

            if (isWritten && isErased) {
                CurrentPage = anotherPage;
                wearCycle = newWearCycle;
                result = true;
            }
        }
    } else {
        uint16_t word = (uint16_t)value;
        word = (word & 0x7) | (wearCycle << 3);
        result = nvWriteWord(CurrentPage, offset, word);
    }

    return result;
}

static bool nvLock() {
    Flash->CR |= FLASH_CR_LOCK;
    return (Flash->CR & FLASH_CR_LOCK) == FLASH_CR_LOCK;
}

static bool nvUnlock() {
    if (Flash->CR & FLASH_CR_LOCK) {
        Flash->KEYR = 0x45670123;
        Flash->KEYR = 0xCDEF89AB;
    }

    return (Flash->CR & FLASH_CR_LOCK) != FLASH_CR_LOCK;
}

static bool nvCheckIsErased(Page page) {
    bool result = true;

    for (size_t i = 0; i < ItemsOnPage; ++i) {
        if (nvReadWord(page, i) != 0xFFFF) {
            result = false;
            break;
        }
    }

    return result;
}

static bool nvGetLast(Page page, size_t* offset, uint16_t* value) {
    bool result = false;

    for (size_t i = ItemsOnPage - 1;; --i) {
        uint16_t item = nvReadWord(page, i);

        if (item != 0xFFFF) {
            *offset = i;
            *value = item;
            result = true;
            break;
        }

        if (i == 0) {
            break;
        }
    }

    return result;
}

static bool nvErasePage(Page page) {
    nvUnlock();

    Flash->CR |= FLASH_CR_PER;
    Flash->AR = ((uint32_t)page) * PageSize;
    Flash->CR |= FLASH_CR_STRT;
    bool result = nvWait();
    Flash->CR &= ~FLASH_CR_PER;

    nvLock();

    return result;
}

static bool nvWait(void) {
    while (Flash->SR & FLASH_SR_BSY) { }

    if (Flash->SR & (FLASH_SR_PGERR | FLASH_SR_WRPRTERR)) {
        Flash->SR |= FLASH_SR_PGERR | FLASH_SR_WRPRTERR | FLASH_SR_EOP;
        return false;
    }

    Flash->SR |= FLASH_SR_PGERR | FLASH_SR_WRPRTERR | FLASH_SR_EOP;

    return true;
}

static uint16_t nvReadWord(Page page, size_t offset) {
    assert(page == First || page == Second);
    assert(offset < ItemsOnPage);

    volatile const uint16_t* p
        = (volatile const uint16_t*)((page == First) ? Page1Address : Page2Address);
    p += offset;
    return *p;
}

static bool nvWriteWord(Page page, size_t offset, uint16_t value) {
    assert(page == First || page == Second);
    assert(offset < ItemsOnPage);

    nvUnlock();

    Flash->CR |= FLASH_CR_PG;

    volatile uint16_t* p = (volatile uint16_t*)((page == First) ? Page1Address : Page2Address);
    p += offset;
    *p = value;
    bool result = nvWait();

    Flash->CR &= ~FLASH_CR_PG;

    nvLock();

    if (nvReadWord(page, offset) != value) {
        return false;
    }

    return result;
}
