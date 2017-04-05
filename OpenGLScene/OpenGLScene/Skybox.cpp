//
//  Skybox.cpp
//  OpenGLScene
//
//  Created by Leon Schiedermair on 04/04/2017.
//  Copyright © 2017 Leon Schiedermair. All rights reserved.
//

#include "Skybox.hpp"

void Skybox::initSkyboxBuffers() {
    
    glGenVertexArrays( 1, &skyboxVAO );
    glGenBuffers( 1, &skyboxVBO );
    glBindVertexArray( skyboxVAO );
    glBindBuffer( GL_ARRAY_BUFFER, skyboxVBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( skyboxVertices ), &skyboxVertices, GL_STATIC_DRAW );
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( GLfloat ), ( GLvoid * ) 0 );
    glBindVertexArray(0);
    
}

Skybox::Skybox() {
    
    
    skyboxShaderProgram = LoadShaders("shaders/SkyboxVertexShader.glsl", "shaders/SkyboxFragmentShader.glsl");
    initSkyboxBuffers();
    
    skyboxFaces.push_back( "textures/skybox/sunny/sunny_right.bmp" );  //textures/skybox/cloudtop_right.bmp
    skyboxFaces.push_back( "textures/skybox/sunny/sunny_left.bmp" );   //textures/skybox/cloudtop_left.bmp
    skyboxFaces.push_back( "textures/skybox/sunny/sunny_up.bmp" );     //textures/skybox/cloudtop_up.bmp
    skyboxFaces.push_back( "textures/skybox/sunny/sunny_down.bmp" );   //textures/skybox/cloudtop_down.bmp
    skyboxFaces.push_back( "textures/skybox/sunny/sunny_back.bmp" );   //textures/skybox/cloudtop_back.bmp
    skyboxFaces.push_back( "textures/skybox/sunny/sunny_front.bmp" );  //textures/skybox/cloudtop_front.bmp
    skyboxTexture = LoadBMPSkybox(skyboxFaces);
    
}
