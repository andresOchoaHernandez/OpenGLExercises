project(stb_image)

cmake_minimum_required(VERSION 3.8.2)

add_library(stb_image SHARED)

set_target_properties(stb_image PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(
    stb_image
    PUBLIC   ${PROJECT_SOURCE_DIR}/include/
)

target_sources(
    stb_image
    PUBLIC  ${PROJECT_SOURCE_DIR}/include/stb_image.hpp
)
