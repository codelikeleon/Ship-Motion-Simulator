//
//  main.cpp
//  OpenGLScene
//
//  Created by Leon Schiedermair on 04/03/2017.
//  Copyright © 2017 Leon Schiedermair. All rights reserved.
//

#include <stdio.h>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

/*
 *  Draws a multicoloured triangle
 */
void render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    glBegin(GL_TRIANGLES);
    {
        glColor3f(1.0, 0.0, 0.0);
        glVertex2f(0, 1);
        glColor3f(0.0, 1.0, 0.0);
        glVertex2f(-1, -1);
        glColor3f(0.0, 0.0, 1.0);
        glVertex2f(1, -1);
    }
    glEnd();
}

int main2() {
    
    
    //First initialise GLFW:
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialise GLFW!/n");
        return -1;
    }

    // Window dimensions
    const GLuint WIDTH = 800, HEIGHT = 600;
    
    // Set all the required options for GLFW
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );

    
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "Triangle", nullptr, nullptr );

    int screenWidth, screenHeight;
    glfwGetFramebufferSize( window, &screenWidth, &screenHeight );
    
    if ( window == nullptr ) {
        fprintf(stderr, "Failed to open GLFW window. Intel GPUs are not 3.3 compatible - version 2.1 is required\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    if ( GLEW_OK != glewInit( ) )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define the viewport dimensions
    glViewport( 0, 0, screenWidth, screenHeight );
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    glfwMakeContextCurrent(window);
    
    fprintf(stderr, "Using OpenGL Version: %s\n", glGetString(GL_VERSION));
    
    
    //Vertex Object Array:
//    GLuint VertexArrayID;
//    glGenVertexArrays(1, &VertexArrayID);         // causes EXC_BAD_ACCESS
//    glBindVertexArray(VertexArrayID);
//    glEnableVertexAttribArray(VertexArrayID);
    
    while (!glfwWindowShouldClose(window)) {
//        float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
//        float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
//        float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
//        glClearColor(r, g, b, 1.0);
        render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    
    return 0;
}
