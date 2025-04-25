set(SRCDIR ${CMAKE_CURRENT_LIST_DIR})

set(CPP_SOURCES
    ${SRCDIR}/startup/startup_stm32f030f4px.s
    ${SRCDIR}/startup/syscalls_stub.c
    ${SRCDIR}/startup/main.c
)
