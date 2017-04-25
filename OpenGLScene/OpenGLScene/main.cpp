//
//  newMain.cpp
//  OpenGLScene
//
//  Created by Leon Schiedermair on 13/03/2017.
//  Copyright © 2017 Leon Schiedermair. All rights reserved.
//

#include <stdio.h>
#include "RenderWindow.hpp"
#include "Water.hpp"
#include "Ground.hpp"

/*
 * Prints to the terminal the time taken to render one frame in milliseconds, averaged over the last second.
 * 16.66.. ms/frame == 60fps
 */
static void showPerformance() {
    
    static double performance_lastTime = glfwGetTime();
    static int nbFrames = 0;
    
    // Measure speed
    double currentTime = glfwGetTime();
    nbFrames++;
    if ( currentTime - performance_lastTime >= 1.0 ) { // If last prinf() was more than 1 sec ago
        // printf and reset timer
        printf( "%f ms/frame\n", 1000.0 / double( nbFrames ) );
        nbFrames = 0;
        performance_lastTime += 1.0;
    }
}

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
    
    //Create window
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "OpenGL Scene", nullptr, nullptr );
    glfwGetFramebufferSize( window, &screenWidth, &screenHeight );
    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        
        exit( EXIT_FAILURE );
    }
    
    glfwMakeContextCurrent( window );
    // Ensure key presses can be captured
    glfwSetInputMode( window, GLFW_STICKY_KEYS, GL_TRUE );
    // Hide the mouse and enable unlimited movement
    glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

    // Initialize GLEW to setup the OpenGL Function pointers
    if ( GLEW_OK != glewInit( ) ) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        exit( EXIT_FAILURE );
    }
    
    // Define the viewport dimensions
    glViewport( 0, 0, screenWidth, screenHeight );
    
    Controls *controls = new Controls( window );
    RenderWindow* renderWindow = new RenderWindow( window, controls );
    Water *water = new Water( window, controls );
    Ground *ground = new Ground( window, controls );
    
    //enable Z-Buffer: Requires GL_DEPTH_BUFFER_BIT in render loop.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    
    //Render loop:
    while ( glfwGetKey( window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose( window ) == 0  ) {
        
        showPerformance();
        
        glfwPollEvents();
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );   //sets background colour
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        renderWindow->display();
        water->display();
        ground->display();
        
        glfwSwapBuffers( window );
    }
    
    glfwTerminate();
    return 0;
}
