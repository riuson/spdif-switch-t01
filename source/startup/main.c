#include "stm32f030xx.h"

int main(void) {
    while (1) {
        __asm("nop");
        __asm("nop");
    }
}