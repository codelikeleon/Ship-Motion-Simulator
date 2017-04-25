//
//  Render.cpp
//  OpenGLScene
//
//  Created by Leon Schiedermair on 13/03/2017.
//  Copyright © 2017 Leon Schiedermair. All rights reserved.
//
//  Notes:
//  - Project working directory for Xcode changed in:
//    Product > Scheme > Edit Scheme > Run > Options > Working Directory.
//  - Rendering problem caused by using shorts instead of ints for the vertex indices.
//    This is fixed by using ints for 'vector<unsigned int> indices' and the buffers.
//

#include "RenderWindow.hpp"

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
    
	//jacob todo you'll need to added another 2 buffers for the other ones. Watch out though as you need to make sure whether buffers always keep the values from the array or need physically reloading each time
	
	
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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    
    //enable Z-Buffer: Requires GL_DEPTH_BUFFER_BIT in render loop.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
}

void RenderWindow::enableArrays() {
    
    glBindVertexArray( VAO );
	
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
    
    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    
}

void RenderWindow::display() {
    
    //Bind to the correct VBOs
    enableArrays();
    glUseProgram( shaderProgram );
    
    //Compute camera view position & movement controls
    controls->computeMatricesFromInputs();
    mat4 projectionMatrix = controls->getProjectionMatrix();
    mat4 viewMatrix = controls->getViewMatrix();
    mat4 modelMatrix = mat4( 1.0 );
    mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
    glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &viewMatrix[0][0]);
    
    //Lighting:
    lightPos = vec3(4, 4, 4);
    glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
    
    //Textures:
    glActiveTexture(GL_TEXTURE0); // Bind texture to Texture Unit 0
    glBindTexture(GL_TEXTURE_2D, texture);
    // Set the "textureSampler" sampler to user Texture Unit 0
    textureID = glGetUniformLocation(shaderProgram, "textureSampler");
    glUniform1i(textureID, 0);
    
    // Draw the vertices
    glDrawElements(
                   GL_TRIANGLES,      // mode
                   indices.size(),    // count
                   GL_UNSIGNED_INT,   // type
                   (void*)0           // element array buffer offset
                   );
    glBindVertexArray(0);
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

RenderWindow::RenderWindow( GLFWwindow* window, Controls* controls ) {
    
    this->window = window;
    this->controls = controls;
    
    //Shaders
    shaderProgram = LoadShaders("shaders/BoatVertexShader.glsl", "shaders/BoatFragmentShader.glsl");
    //Send handles to GLSL:
    MatrixID = glGetUniformLocation(shaderProgram, "MVP");
    ViewMatrixID = glGetUniformLocation(shaderProgram, "V");
    ModelMatrixID = glGetUniformLocation(shaderProgram, "M");
    LightID = glGetUniformLocation(shaderProgram, "light_position_worldspace");
    
    //Textures
    texture = LoadDDS("textures/suzanne.DDS");
    textureID = glGetUniformLocation(shaderProgram, "textureSampler");
    
    //Load .obj files
    bool res = LoadAssImp("obj/suzanne.obj", indices, indexed_vertices, indexed_uvs, indexed_normals);
    /*
     * Objects:
     *
     * cube.obj           - working, texture: cube.DDS
     * suzanne.obj        - working, texture: suzanne.DDS
     * rowBoat.obj        - working, no texture.
     * pirateShip.obj     - working, no texture.
     * yacht.obj          - working, no texture.
     * rubberDingy.obj    - sometimes works, no texture.
     * boat.obj           - not working, causes runtime error.
     * compatibleBoat.obj - not working, causes runtime error.
     * tugBoat.obj        - not working, causes runtime error.
     */

    glfwSwapInterval(1);
    initBuffers();
}

RenderWindow::~RenderWindow() {
    
    //De-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays( 1, &VAO );
    glDeleteBuffers( 1, &VBO );
    glDeleteBuffers(1, &uvBuffer);
    glDeleteBuffers(1, &normalBuffer);
    glDeleteBuffers(1, &elementBuffer);
    glDeleteProgram(shaderProgram);
    glDeleteTextures(1, &texture);
}

