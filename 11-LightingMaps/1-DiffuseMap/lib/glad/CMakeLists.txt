project(glad)

cmake_minimum_required(VERSION 3.8.2)

add_library(glad SHARED)

target_include_directories(
    glad
    PUBLIC  ${PROJECT_SOURCE_DIR}/include/
    PRIVATE  ${PROJECT_SOURCE_DIR}/src/
)

target_sources(
    glad
    PUBLIC  ${PROJECT_SOURCE_DIR}/include/glad/glad.h
    PUBLIC  ${PROJECT_SOURCE_DIR}/include/KHR/khrplatform.h
    PRIVATE  ${PROJECT_SOURCE_DIR}/src/glad.c
)