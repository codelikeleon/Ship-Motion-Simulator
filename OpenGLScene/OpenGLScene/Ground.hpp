//
//  Ground.hpp
//  OpenGLScene
//
//  Created by Leon Schiedermair on 25/04/2017.
//  Copyright © 2017 Leon Schiedermair. All rights reserved.
//

#ifndef Ground_hpp
#define Ground_hpp

#include <stdio.h>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "controls.hpp"
#include "LoadFunctions.hpp"


class Ground {
    
    GLFWwindow* window;
    Controls* controls;
    
    GLuint VAO, VBO;
    GLuint shaderProgram;
    GLuint MatrixID;
    GLuint ModelMatrixID;
    GLuint ViewMatrixID;
    glm::mat4 Projection, View, Model, MVP;
    
public:
    void display();
    Ground( GLFWwindow* window, Controls* controls );
    
};

#endif /* Ground_hpp */
