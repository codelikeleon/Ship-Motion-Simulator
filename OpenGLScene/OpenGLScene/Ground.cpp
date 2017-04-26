//
//  Ground.cpp
//  OpenGLScene
//
//  Created by Leon Schiedermair on 25/04/2017.
//  Copyright © 2017 Leon Schiedermair. All rights reserved.
//

#include "Ground.hpp"


void Ground::display() {
    
    glBindVertexArray(VAO);
    glUseProgram(shaderProgram);
    
    //Compute camera view position & movement controls
    controls->computeMatricesFromInputs();
    mat4 projectionMatrix = controls->getProjectionMatrix();
    mat4 viewMatrix = controls->getViewMatrix();
    mat4 modelMatrix = mat4( 1.0 );
    mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;
    glUniformMatrix4fv( MatrixID, 1, GL_FALSE, &mvp[0][0] );
    glUniformMatrix4fv( ModelMatrixID, 1, GL_FALSE, &modelMatrix[0][0] );
    glUniformMatrix4fv( ViewMatrixID, 1, GL_FALSE, &viewMatrix[0][0] );
    
    //Draw ground:
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glVertexAttribPointer(
                          0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                          3,                  // size
                          GL_FLOAT,           // type
                          GL_FALSE,           // normalized?
                          0,                  // stride
                          (void*)0            // array buffer offset
                          );
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDisableVertexAttribArray(0);

}

Ground::Ground( GLFWwindow* window, Controls* controls ) {
    
    this->window = window;
    this->controls = controls;
    
    const GLfloat ground_vertices[] = {
        -1.0f, -0.1f, -1.0f,
        -1.0f, -0.1f,  1.0f,
        1.0f, -0.1f,  1.0f,
        1.0f, -0.1f, -1.0f
    };
    
    //shaders:
    shaderProgram = LoadShaders( "shaders/GroundVertexShader.glsl", "shaders/GroundFragmentShader.glsl" );
    MatrixID = glGetUniformLocation(shaderProgram, "MVP");
    
    //VAO / VBOs:
    glGenVertexArrays( 1, &VAO );
    glBindVertexArray( VAO );
    
    //Vertex Buffer:
    glGenBuffers( 1, &VBO );
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof(ground_vertices), ground_vertices, GL_STATIC_DRAW );
    
}
