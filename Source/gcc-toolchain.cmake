set(CMAKE_SYSTEM_NAME Generic)

set(TOOL_PREFIX "arm-none-eabi")

if (WIN32)
    find_program (C_EXECUTABLE "${TOOL_PREFIX}-gcc.exe")
    find_program (CXX_EXECUTABLE "${TOOL_PREFIX}-g++.exe")
    find_program (LINK_EXECUTABLE "${TOOL_PREFIX}-ld.exe")
    find_program (SIZE_EXECUTABLE "${TOOL_PREFIX}-size.exe")
    find_program (OBJCOPY_EXECUTABLE "${TOOL_PREFIX}-objcopy.exe")
    find_program (OBJDUMP_EXECUTABLE "${TOOL_PREFIX}-objdump.exe")
elseif(LINUX)
    find_program (C_EXECUTABLE "${TOOL_PREFIX}-gcc")
    find_program (CXX_EXECUTABLE "${TOOL_PREFIX}-g++")
    find_program (LINK_EXECUTABLE "${TOOL_PREFIX}-ld")
    find_program (SIZE_EXECUTABLE "${TOOL_PREFIX}-size")
    find_program (OBJCOPY_EXECUTABLE "${TOOL_PREFIX}-objcopy")
    find_program (OBJDUMP_EXECUTABLE "${TOOL_PREFIX}-objdump")
endif()

if (NOT C_EXECUTABLE)
    message(FATAL_ERROR "C_EXECUTABLE executable not found!")
endif()
if (NOT CXX_EXECUTABLE)
    message(FATAL_ERROR "CXX_EXECUTABLE executable not found!")
endif()
if (NOT LINK_EXECUTABLE)
    message(FATAL_ERROR "LINK_EXECUTABLE executable not found!")
endif()
if (NOT SIZE_EXECUTABLE)
    message(FATAL_ERROR "SIZE_EXECUTABLE executable not found!")
endif()
if (NOT OBJCOPY_EXECUTABLE)
    message(FATAL_ERROR "OBJCOPY_EXECUTABLE executable not found!")
endif()
if (NOT OBJDUMP_EXECUTABLE)
    message(FATAL_ERROR "OBJDUMP_EXECUTABLE executable not found!")
endif()

set(CMAKE_C_COMPILER ${C_EXECUTABLE} CACHE FILEPATH "GCC ARM C compiler")
set(CMAKE_CXX_COMPILER ${CXX_EXECUTABLE} CACHE FILEPATH "GCC ARM C++ compiler")
set(CMAKE_ASM_COMPILER ${C_EXECUTABLE} CACHE FILEPATH "GCC ARM Asm compiler")


# Clear flags by default.
set(CMAKE_ASM_FLAGS "" CACHE STRING "")
set(CMAKE_C_FLAGS "" CACHE STRING "")
set(CMAKE_CXX_FLAGS "" CACHE STRING "")
set(CMAKE_EXE_LINKER_FLAGS "" CACHE STRING "")

set(CMAKE_ASM_FLAGS_DEBUG "" CACHE STRING "")
set(CMAKE_C_FLAGS_DEBUG "" CACHE STRING "")
set(CMAKE_CXX_FLAGS_DEBUG "" CACHE STRING "")
set(CMAKE_EXE_LINKER_DEBUG "" CACHE STRING "")

set(CMAKE_ASM_FLAGS_RELEASE "" CACHE STRING "")
set(CMAKE_C_FLAGS_RELEASE "" CACHE STRING "")
set(CMAKE_CXX_FLAGS_RELEASE "" CACHE STRING "")
set(CMAKE_EXE_LINKER_RELEASE "" CACHE STRING "")

add_link_options(--specs=nosys.specs)

# Replace linker with g++.
set(CMAKE_CUSTOM_LINKER ${CMAKE_CXX_COMPILER})
set(CMAKE_CXX_LINK_EXECUTABLE
    "${CMAKE_CUSTOM_LINKER} <FLAGS> <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
