//
//  newMain.cpp
//  OpenGLScene
//
//  Created by Leon Schiedermair on 13/03/2017.
//  Copyright © 2017 Leon Schiedermair. All rights reserved.
//

#include <stdio.h>
#include "RenderWindow.hpp"

int main() {
    
    const GLuint WIDTH = 1024, HEIGHT = 768;
    int screenWidth, screenHeight;
    
    //Initialise GLFW
    glfwInit();
    
    // Set all the required options for OpenGL
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    glewExperimental = GL_TRUE; // So GLEW uses a modern approach to retrieving function pointers and extensions
    
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "OpenGL Scene", nullptr, nullptr );
    glfwGetFramebufferSize( window, &screenWidth, &screenHeight );
    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        
        exit( EXIT_FAILURE );
    }
    
    glfwMakeContextCurrent( window );
    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode( window, GLFW_STICKY_KEYS, GL_TRUE );
    // Hide the mouse and enable unlimited movement
    glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

    Controls *controls = new Controls( window );
    
    // Initialize GLEW to setup the OpenGL Function pointers
    if ( GLEW_OK != glewInit( ) ) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        exit( EXIT_FAILURE );
    }
    
    // Define the viewport dimensions
    glViewport( 0, 0, screenWidth, screenHeight );
    
    RenderWindow* renderWindow = new RenderWindow( window, controls );
    
    /*********************************
     *          RENDER LOOP:         *
     *********************************/
    while ( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0  ) {

        glfwPollEvents();
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );   //sets background colour
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        renderWindow->display();
        
        
        glfwSwapBuffers( window );
    }
    
    
    glfwTerminate();
    return 0;
}
