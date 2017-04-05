//
//  Skybox.hpp
//  OpenGLScene
//
//  Created by Leon Schiedermair on 04/04/2017.
//  Copyright © 2017 Leon Schiedermair. All rights reserved.
//

#ifndef Skybox_hpp
#define Skybox_hpp

#include <stdio.h>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "LoadFunctions.hpp"

static const GLfloat skyboxVertices[] = {
    // Positions
    
    //right
    -1.0f,  1.0f, -1.0f,    //-1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,    //-1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,     //1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,     //1.0f, -1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,     //1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,    //-1.0f,  1.0f, -1.0f,
    
    //left
    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    
    //up
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    
    //down
    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    
    //back
    -1.0f,  1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,
    
    //front
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f,  1.0f
};

class Skybox {
    
public:
    GLuint skyboxShaderProgram;
    GLuint skyboxVAO, skyboxVBO;
    std::vector<const GLchar*> skyboxFaces;
    GLuint skyboxTexture;
    
    Skybox();
    void initSkyboxBuffers();
};

#endif /* Skybox_hpp */
