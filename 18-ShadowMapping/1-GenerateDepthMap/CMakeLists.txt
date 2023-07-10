project(ShadowMapping)

cmake_minimum_required(VERSION 3.8.2)

cmake_policy(SET CMP0072 NEW)

FIND_PACKAGE(OpenGL REQUIRED)
IF(NOT OPENGL_FOUND)
    MESSAGE("ERROR: OpenGL not found")
ENDIF(NOT OPENGL_FOUND)

ADD_EXECUTABLE(shadowMapping ${PROJECT_SOURCE_DIR}/main.cpp)

target_include_directories(
    shadowMapping
    PUBLIC  ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${PROJECT_SOURCE_DIR}/src
)

target_sources(
    shadowMapping
    PUBLIC  ${PROJECT_SOURCE_DIR}/include/shader.hpp
    PRIVATE ${PROJECT_SOURCE_DIR}/src/shader.cpp

    PUBLIC  ${PROJECT_SOURCE_DIR}/include/camera.hpp
    PRIVATE ${PROJECT_SOURCE_DIR}/src/camera.cpp

    PUBLIC  ${PROJECT_SOURCE_DIR}/include/mesh.hpp
    PRIVATE ${PROJECT_SOURCE_DIR}/src/mesh.cpp

    PUBLIC  ${PROJECT_SOURCE_DIR}/include/model.hpp
    PRIVATE ${PROJECT_SOURCE_DIR}/src/model.cpp
)

add_subdirectory(${PROJECT_SOURCE_DIR}/lib/glad)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/stb_image)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/glm)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/assimp)

TARGET_LINK_LIBRARIES(shadowMapping glfw glad stb_image glm::glm assimp)
