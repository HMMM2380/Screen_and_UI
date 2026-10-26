cmake_minimum_required(VERSION 3.20)

set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_CXX_COMPILER_WORKS TRUE)

# Setup C compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

# Setup Cpp compiler settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Define the build type
set(CMAKE_BUILD_TYPE "Debug")

# Set the project name
set(CMAKE_PROJECT_NAME Screen_and_UI)

# Core project settings
project(${CMAKE_PROJECT_NAME} LANGUAGES C ASM CXX)
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Enable CMake support languages
enable_language(ASM C CXX)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set_property(
        TARGET ${CMAKE_PROJECT_NAME}
        PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE
    )
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)

set(Drivers_Src_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Drivers/Src")
set(fonts_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/fonts")
set(UI_Src_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/UI/Src")

file(
    GLOB_RECURSE Driver_Src_SOURCE_FILES
    "${Drivers_Src_SOURCE_DIR}/*.c"
    "${Drivers_Src_SOURCE_DIR}/*.cpp"
)

file(GLOB_RECURSE fonts_SOURCE_FILES "${fonts_SOURCE_DIR}/*.c")

file(
    GLOB_RECURSE UI_Src_SOURCE_FILES
    "${UI_Src_SOURCE_DIR}/*.c"
    "${UI_Src_SOURCE_DIR}/*.cpp"
)

# Link directories setup
target_link_directories(
    ${CMAKE_PROJECT_NAME}
    PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
target_sources(
    ${CMAKE_PROJECT_NAME}
    PRIVATE
        # Add user sources here
        ${Driver_Src_SOURCE_FILES}
        ${UI_Src_SOURCE_FILES}
        ${fonts_SOURCE_FILES}
)

# Add include pathsb
target_include_directories(
    ${CMAKE_PROJECT_NAME}
    PRIVATE
        Drivers/Inc
        fonts/
        UI/Inc
        Libc/DataStruct/Inc
        # Add user sources here
)

# Add project symbols (macros)
target_compile_definitions(
    ${CMAKE_PROJECT_NAME}
    PRIVATE
    # Add user defined symbols
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(
    ${CMAKE_PROJECT_NAME}
    # Add user link libraries
)
