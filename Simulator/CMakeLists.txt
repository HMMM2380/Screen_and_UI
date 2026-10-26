cmake_minimum_required(VERSION 3.20)
message("Build type: " ${CMAKE_BUILD_TYPE})

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/Core/Src SIM_CORE_SRC)

add_executable(Simulator ${SIM_CORE_SRC})

target_include_directories(Simulator PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc
)

target_link_libraries(Simulator PRIVATE
    PkgConfig::SDL2
    screen_ui_core
)

set_target_properties(Simulator PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)