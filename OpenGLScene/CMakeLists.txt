cmake_minimum_required(VERSION 3.6)
project(OpenGLScene)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
    cmake-build-debug/CMakeFiles/3.6.2/CompilerIdC/CMakeCCompilerId.c
    cmake-build-debug/CMakeFiles/3.6.2/CompilerIdCXX/CMakeCXXCompilerId.cpp
    cmake-build-debug/CMakeFiles/feature_tests.c
    cmake-build-debug/CMakeFiles/feature_tests.cxx
    OpenGLScene/shaders/FragmentShader.glsl
    OpenGLScene/shaders/VertexShader.glsl
    OpenGLScene/controls.cpp
    OpenGLScene/controls.hpp
    OpenGLScene/main.cpp
    OpenGLScene/RenderFunctions.h
    OpenGLScene/RenderWindow.cpp
    OpenGLScene/RenderWindow.hpp
    OpenGLScene/shader.cpp
    OpenGLScene/shader.hpp
    OpenGLScene/uvtemplate.DDS)

add_executable(OpenGLScene ${SOURCE_FILES})