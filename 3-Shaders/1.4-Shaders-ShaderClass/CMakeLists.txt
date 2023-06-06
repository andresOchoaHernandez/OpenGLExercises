project(Shaders)

cmake_minimum_required(VERSION 3.8.2)

cmake_policy(SET CMP0072 NEW)

FIND_PACKAGE(OpenGL REQUIRED)
IF(NOT OPENGL_FOUND)
    MESSAGE("ERROR: OpenGL not found")
ENDIF(NOT OPENGL_FOUND)

ADD_EXECUTABLE(shaders ${PROJECT_SOURCE_DIR}/main.cpp)

target_include_directories(
    shaders
    PUBLIC  ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${PROJECT_SOURCE_DIR}/src
)

target_sources(
    shaders
    PUBLIC  ${PROJECT_SOURCE_DIR}/include/shader.hpp
    PRIVATE ${PROJECT_SOURCE_DIR}/src/shader.cpp
)

add_subdirectory(${PROJECT_SOURCE_DIR}/lib/glad)

TARGET_LINK_LIBRARIES(shaders glfw glad)