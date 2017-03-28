//
//  Render.cpp
//  OpenGLScene
//
//  Created by Leon Schiedermair on 13/03/2017.
//  Copyright © 2017 Leon Schiedermair. All rights reserved.
//
//  Notes:
//  - Project working directory changed in:
//    Product > Scheme > Edit Scheme > Run > Options > Working Directory.
//
//

#include "RenderWindow.hpp"
    
/*************************
 *    Other Functions:   *
 ************************/

void RenderWindow::setGLContext() {
    // Set all the required options for GLFW
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    glewExperimental = GL_TRUE; // So GLEW uses a modern approach to retrieving function pointers and extensions
}

/*
 * Prints to the terminal the time taken to render one frame in milliseconds, averaged over the last second.
 * 16.66.. ms/frame == 60fps
 */
void RenderWindow::showPerformance() {
    
    static double performance_lastTime = glfwGetTime();
    static int nbFrames = 0;
    
    // Measure speed
    double currentTime = glfwGetTime();
    nbFrames++;
    if ( currentTime - performance_lastTime >= 1.0 ) { // If last prinf() was more than 1 sec ago
        // printf and reset timer
        printf("%f ms/frame\n", 1000.0/double(nbFrames));
        nbFrames = 0;
        performance_lastTime += 1.0;
    }
}

void RenderWindow::initBuffers() {
    
    glGenVertexArrays( 1, &VAO );
    glBindVertexArray( VAO );
    
    //Vertex Buffer:
    glGenBuffers( 1, &VBO );
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW );
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    
    //UV buffer:
    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec3), &indexed_uvs[0], GL_STATIC_DRAW);
    
    //Normal buffer:
    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);
    
    //Element buffer:
    glGenBuffers(1, &elementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
    
    //enable Z-Buffer: Requires GL_DEPTH_BUFFER_BIT in render loop.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

void RenderWindow::enableArrays() {
    
    //Vertex
    glEnableVertexAttribArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, ( GLvoid * ) 0 );   //Try setting stride to 0 instead of 3 * sizeof( GLfloat )
    
    //UV
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, ( GLvoid * ) 0 );
    
    //Normal
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE, 0, ( GLvoid * ) 0 );
    
}

RenderWindow::RenderWindow() {
    //Initialise GLFW
    glfwInit();
    setGLContext();
    
    window = glfwCreateWindow( WIDTH, HEIGHT, "OpenGL Scene", nullptr, nullptr );
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
    // Hide the mouse and enable unlimited movement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //glfwSetKeyCallback(window, key_callback); //Causes any key press to close window
    
    controls = new Controls( window );
    
    // Initialize GLEW to setup the OpenGL Function pointers
    if ( GLEW_OK != glewInit( ) ) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        exit( EXIT_FAILURE );
    }
    
    // Define the viewport dimensions
    glViewport( 0, 0, screenWidth, screenHeight );
    
    /*******************************
     *           Shaders:          *
     ******************************/
    
    shaderProgram = LoadShaders("shaders/VertexShader.glsl", "shaders/FragmentShader.glsl");
    //Send handles to GLSL:
    MatrixID = glGetUniformLocation(shaderProgram, "MVP");
    ViewMatrixID = glGetUniformLocation(shaderProgram, "V");
    ModelMatrixID = glGetUniformLocation(shaderProgram, "M");
    LightID = glGetUniformLocation(shaderProgram, "light_position_worldspace");
    
    glUseProgram( shaderProgram );
    
    /*******************************
     *           Textures:         *
     ******************************/
    
    texture = LoadDDS("textures/suzanne.DDS");
    textureID = glGetUniformLocation(shaderProgram, "textureSampler");
    
    /*******************************
     *           OBJ files:         *
     ******************************/
    
//    res = LoadAssImp("/Users/Leon/OneDrive - University of Exeter/Ship Motion Simulator/Projects/OpenGLScene/OpenGLScene/OpenGLScene/obj/compatibleBoat3.obj", vertices, uvs, normals);
    
    // Read our .obj file
    bool res = LoadAssImp("obj/simpleRowBoat.obj", indices, indexed_vertices, indexed_uvs, indexed_normals);
    
    glfwSwapInterval(1);
    initBuffers();
    /*********************************
     *          RENDER LOOP:         *      Should be moved elsewhere?
     *********************************/
    while ( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0  ) {
        glfwPollEvents();
//        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );   //sets background colour
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        showPerformance();
        controls->computeMatricesFromInputs();
        mat4 projectionMatrix = controls->getProjectionMatrix();
        mat4 viewMatrix = controls->getViewMatrix();
        mat4 modelMatrix = mat4( 1.0 );
        mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &viewMatrix[0][0]);
        
        lightPos = vec3(4, 4, 4);
        glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
        
        // Bind texture to Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        // Set the "textureSampler" sampler to user Texture Unit 0
        glUniform1i(textureID, 0);
        
        // Draw the shape:
        //glDrawArrays(GL_TRIANGLES, 0, vertices.size() );
        
        enableArrays();
        
        // Index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);

        // Draw the triangles !
        glDrawElements(
                       GL_TRIANGLES,      // mode
                       indices.size(),    // count
                       GL_UNSIGNED_SHORT,   // type
                       (void*)0           // element array buffer offset
                       );
        
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        glfwSwapBuffers( window );
    }
    
    //De-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays( 1, &VAO );
    glDeleteBuffers( 1, &VBO );
    glfwTerminate();
}

