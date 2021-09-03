cmake_minimum_required(VERSION 3.0.0)
project(opengl_app VERSION 0.1.0)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/.cmake")
set(CMAKE_BUILD_TYPE Release)
set(APP_SOURCES 
    src/App.cpp
    src/Camera.cpp 
    src/DynamicModel.cpp 
    src/Entity.cpp 
    src/Model.cpp 
    src/Path.cpp 
    src/Renderer.cpp 
    src/ShaderProgram.cpp 
    src/StaticModel.cpp
)
add_executable(opengl_app ${APP_SOURCES})
set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)

find_package(GLFW3 REQUIRED)
target_include_directories(opengl_app PRIVATE ${GLFW3_INCLUDE_DIR})
target_link_libraries(opengl_app ${GLFW3_LIBRARY})

find_package(GLEW REQUIRED)
target_include_directories(opengl_app PRIVATE ${GLEW_INCLUDE_DIRS})
target_link_libraries(opengl_app ${GLEW_LIBRARIES})

target_link_libraries(opengl_app libGL.so)
target_link_libraries(opengl_app libX11.so)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
