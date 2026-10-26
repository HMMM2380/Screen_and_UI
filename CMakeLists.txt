cmake_minimum_required(VERSION 3.20)
project(Screen_and_UI LANGUAGES C ASM CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

message("Build type: " ${CMAKE_BUILD_TYPE})
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/UI/Src UI_SRC)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/Drivers/Src DRIVERS_SRC)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/fonts FONTS_SRC)

add_library(screen_ui_core STATIC
    ${UI_SRC}
    ${DRIVERS_SRC}
    ${FONTS_SRC}
)

target_include_directories(screen_ui_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/fonts
    ${CMAKE_CURRENT_SOURCE_DIR}/UI/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Libc/DataStruct/Inc
)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set_property(TARGET screen_ui_core PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Simulator)
