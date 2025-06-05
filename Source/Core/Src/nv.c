#include "nv.h"

static volatile const __attribute__((section(".storage_space_1"))) uint8_t StorageSpace1[1024] = {};
static volatile const __attribute__((section(".storage_space_2"))) uint8_t StorageSpace2[1024] = {};

void nvInit(void) {
    if (StorageSpace1[0] == 0) 
    {

    }
 }

UserSource nvGetState(void) { return UserSource1; }

void nvSetState(UserSource value) { }
