set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX17_STANDARD_COMPILE_OPTION "")
set(CMAKE_CXX17_EXTENSION_COMPILE_OPTION "")

function(configure_compiler TARGET_NAME)
    target_compile_definitions(
        ${TARGET_NAME} PUBLIC
        $<$<BOOL:${IS_DEBUG}>:DEBUG=1 TRACE=1>
        STM32F030F4
        STM32F030F4P6
    )

    target_compile_options(
        ${TARGET_NAME} PRIVATE
        $<IF:$<BOOL:${IS_DEBUG}>,-Og -g3,-O2 -g0>
        -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -fmessage-length=0
        -fsigned-char -ffunction-sections -fdata-sections
        -MMD -MP -c
        $<$<COMPILE_LANGUAGE:ASM>:-x assembler-with-cpp>
        $<$<OR:$<COMPILE_LANGUAGE:C>,$<COMPILE_LANGUAGE:CXX>>:-mno-unaligned-access -Wall -Wextra -Wpointer-arith -Wredundant-decls -Wshadow -Wcast-qual -Wswitch -Wreturn-type -Wshadow -Wunused -pedantic -Wno-format-truncation>
        $<$<COMPILE_LANGUAGE:C>: -Wnested-externs -Wimplicit -Wstrict-prototypes>
        $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti -fno-exceptions>
    )
endfunction()

function(configure_linker TARGET_NAME)
    set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/source/startup/stm32f030f4.ld")

    target_link_options(
        ${TARGET_NAME} PRIVATE
        -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -fmessage-length=0 -fsigned-char
        -ffunction-sections -fdata-sections -Xlinker --gc-sections
        -T ${LINKER_SCRIPT}
        -Wl,-Map=${TARGET_NAME}.map
        -Wl,--print-memory-usage
        -Xlinker --gc-sections
        --entry=Reset_Handler
        -specs=nano.specs
    )
    set_target_properties(${TARGET_NAME} PROPERTIES SUFFIX ".elf")
endfunction()

message(STATUS "GCC ${CMAKE_BUILD_TYPE}")
