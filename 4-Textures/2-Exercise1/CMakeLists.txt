project(Exercise1)

cmake_minimum_required(VERSION 3.8.2)

cmake_policy(SET CMP0072 NEW)

FIND_PACKAGE(OpenGL REQUIRED)
IF(NOT OPENGL_FOUND)
    MESSAGE("ERROR: OpenGL not found")
ENDIF(NOT OPENGL_FOUND)

ADD_EXECUTABLE(exercise1 ${PROJECT_SOURCE_DIR}/main.cpp)

target_include_directories(
    exercise1
    PUBLIC  ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${PROJECT_SOURCE_DIR}/src
)

target_sources(
    exercise1
    PUBLIC  ${PROJECT_SOURCE_DIR}/include/shader.hpp
    PRIVATE ${PROJECT_SOURCE_DIR}/src/shader.cpp
)

add_subdirectory(${PROJECT_SOURCE_DIR}/lib/glad)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/stb_image)

TARGET_LINK_LIBRARIES(exercise1 glfw glad stb_image)