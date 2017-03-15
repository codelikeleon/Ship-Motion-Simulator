cmake_minimum_required(VERSION 3.6)
project(OpenGLScene)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
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
    OpenGLScene/uvtemplate.DDS
    OpenGLScene.xcodeproj/project.xcworkspace/xcuserdata/Leon.xcuserdatad/UserInterfaceState.xcuserstate
    OpenGLScene.xcodeproj/project.xcworkspace/contents.xcworkspacedata
    OpenGLScene.xcodeproj/xcuserdata/Leon.xcuserdatad/xcdebugger/Breakpoints_v2.xcbkptlist
    OpenGLScene.xcodeproj/xcuserdata/Leon.xcuserdatad/xcschemes/OpenGLScene.xcscheme
    OpenGLScene.xcodeproj/xcuserdata/Leon.xcuserdatad/xcschemes/xcschememanagement.plist
    OpenGLScene.xcodeproj/project.pbxproj)

add_executable(OpenGLScene ${SOURCE_FILES})