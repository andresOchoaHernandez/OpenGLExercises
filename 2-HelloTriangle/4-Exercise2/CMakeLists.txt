project(Exercise2)

cmake_minimum_required(VERSION 3.8.2)

cmake_policy(SET CMP0072 NEW)

FIND_PACKAGE(OpenGL REQUIRED)
IF(NOT OPENGL_FOUND)
    MESSAGE("ERROR: OpenGL not found")
ENDIF(NOT OPENGL_FOUND)

ADD_EXECUTABLE(exercise2 ${PROJECT_SOURCE_DIR}/main.cpp)

add_subdirectory(${PROJECT_SOURCE_DIR}/lib/glad)

TARGET_LINK_LIBRARIES(exercise2 glfw glad)