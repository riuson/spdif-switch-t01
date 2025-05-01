cmake_minimum_required(VERSION 3.29)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/Source/gcc-toolchain.cmake)

project(firmware LANGUAGES ASM C CXX)

set(CMAKE_VERBOSE_MAKEFILE off)
include(CMakePrintHelpers)

include(${PROJECT_SOURCE_DIR}/Source/gcc.cmake)
set(TARGET_NAME firmware)

set(allowableBuildTypes Debug DebugApp Release ReleaseApp)
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "${allowableBuildTypes}")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
elseif()
    if(NOT CMAKE_BUILD_TYPE IN_LIST allowableBuildTypes)
        message(FATAL_ERROR "Invalid build type was specified: ${CMAKE_BUILD_TYPE}")
    endif()
endif()

include_directories(${PROJECT_SOURCE_DIR})
include(${PROJECT_SOURCE_DIR}/Source/headerdirs.cmake)
include(${PROJECT_SOURCE_DIR}/Source/sourcefiles.cmake)

if(CMAKE_BUILD_TYPE MATCHES "^Debug" OR CMAKE_BUILD_TYPE MATCHES "^Release")
    if(CMAKE_BUILD_TYPE MATCHES "^Release")
        option(ACTUAL_BUILD_TIMESTAMP "Actualize build timestamp" ON)
    endif()

    if(CMAKE_BUILD_TYPE MATCHES "^Debug")
        option(IS_DEBUG "Is debug version" ON)
        option(IS_RELEASE "Is release version" OFF)
    elseif(CMAKE_BUILD_TYPE MATCHES "^Release")
        option(IS_DEBUG "Is debug version" OFF)
        option(IS_RELEASE "Is release version" ON)
    endif()

    set(LIST_SOURCES
        ${CPP_SOURCES}
    )

    add_executable(${TARGET_NAME}
        ${LIST_SOURCES})

    configure_compiler(${TARGET_NAME})
    configure_linker(${TARGET_NAME})

    add_custom_command(TARGET ${TARGET_NAME}
        POST_BUILD
        COMMAND ${SIZE_EXECUTABLE} --format=berkeley ${PROJECT_NAME}.elf
        COMMAND ${OBJDUMP_EXECUTABLE} --source --all-headers --demangle --line-numbers --wide ${PROJECT_NAME}.elf > ${PROJECT_NAME}.lst
    )
endif()
