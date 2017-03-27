//
//  Render.cpp
//  OpenGLScene
//
//  Created by Leon Schiedermair on 13/03/2017.
//  Copyright © 2017 Leon Schiedermair. All rights reserved.
//

#include "RenderWindow.hpp"
    
/*************************
 *    Other Functions:   *
 ************************/

//Somehow causes any key press to close the window
void RenderWindow::key_callback( GLFWwindow* window, int key, int scancode, int action, int mods ) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        std::cout << "ESC key pressed" << std::endl;
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void RenderWindow::setGLContext() {
    // Set all the required options for GLFW
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    glewExperimental = GL_TRUE; // So GLEW uses a modern approach to retrieving function pointers and extensions
}

void RenderWindow::initBuffers() {
    
    glGenVertexArrays( 1, &VAO );
    glBindVertexArray( VAO );
    
    //Vertex Buffer:
    glGenBuffers( 1, &VBO );
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( g_vertex_buffer_data ), g_vertex_buffer_data, GL_STATIC_DRAW );
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    
    //UV buffer:
    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( GLfloat ), ( GLvoid * ) 0 );
    
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof( GLfloat ), ( GLvoid * ) 0 );
    // This is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    
    //enable Z-Buffer: Requires GL_DEPTH_BUFFER_BIT in render loop.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
//        glBindVertexArray( 0 ); // Unbind VAO - unbind any buffer/array to prevent strange bugs
    
}

RenderWindow::RenderWindow() {
    //Initialise GLFW
    glfwInit();
    setGLContext();
    
    window = glfwCreateWindow( WIDTH, HEIGHT, "OpenGL Scene", nullptr, nullptr);
    glfwGetFramebufferSize( window, &screenWidth, &screenHeight );
    
    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        
        exit( EXIT_FAILURE );
    }
    
    glfwMakeContextCurrent( window );
    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //glfwSetKeyCallback(window, key_callback); //Causes any key press to close window
    
    controls = new Controls( window );
    
    // Initialize GLEW to setup the OpenGL Function pointers
    if ( GLEW_OK != glewInit( ) )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        exit( EXIT_FAILURE );
    }
    
    // Define the viewport dimensions
    glViewport( 0, 0, screenWidth, screenHeight );
    
    /*******************************
     *           Shaders:          *
     ******************************/
    
    shaderProgram = LoadShaders("/Users/Leon/OneDrive - University of Exeter/Ship Motion Simulator/Projects/OpenGLScene/OpenGLScene/OpenGLScene/shaders/VertexShader.glsl", "/Users/Leon/OneDrive - University of Exeter/Ship Motion Simulator/Projects/OpenGLScene/OpenGLScene/OpenGLScene/shaders/FragmentShader.glsl");       //TODO: Change to relative path
    
    //Send to GLSL:
    MatrixID = glGetUniformLocation(shaderProgram, "MVP");
    
    glfwSwapInterval(1);
    initBuffers();
    
    /*******************************
     *           Textures:         *
     ******************************/
    
    texture = LoadDDS("/Users/Leon/OneDrive - University of Exeter/Ship Motion Simulator/Projects/OpenGLScene/OpenGLScene/OpenGLScene/uvtemplate.DDS");
    textureID = glGetUniformLocation(shaderProgram, "textureSampler");
    
    /*********************************
     *          RENDER LOOP:         *      Should be moved elsewhere?
     *********************************/
    while ( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0  ) {
        glfwPollEvents();
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram( shaderProgram );
        
        controls->computeMatricesFromInputs();
        mat4 projectionMatrix = controls->getProjectionMatrix();
        mat4 viewMatrix = controls->getViewMatrix();
        mat4 modelMatrix = mat4( 1.0 );
        mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
        
        // Draw the shape:
        glDrawArrays(GL_TRIANGLES, 0, 12*3);
        
        glfwSwapBuffers( window );
    }
    
    //De-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays( 1, &VAO );
    glDeleteBuffers( 1, &VBO );
    glfwTerminate();
}

