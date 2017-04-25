//
//  water.hpp
//  OpenGLScene
//
//  Created by Leon Schiedermair on 17/04/2017.
//  Copyright © 2017 Leon Schiedermair. All rights reserved.
//

#ifndef water_hpp
#define water_hpp

#define RESOLUTION 64
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#include <stdio.h>
#include <fstream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "LoadFunctions.hpp"
#include "controls.hpp"
#include "Noise.hpp"

class Water {
    
    GLFWwindow* window;
    Controls* controls;
    Noise* noise;
    
    GLuint VAO, VBO, normalBuffer;
    GLuint VAO_ground, VBO_ground;
    GLuint shaderProgram;
    GLuint groundShaderProgram;
    glm::mat4 Projection, View, Model, MVP;
    GLuint MatrixID;
    GLuint ViewMatrixID;
    GLuint ModelMatrixID;
    GLuint LightID;

    bool wire_frame;
    int normals;
    int xold;
    int yold;
    float t_old = 0;
    
    float surface[6 * RESOLUTION * (RESOLUTION + 1)];
    float normal[6 * RESOLUTION * (RESOLUTION + 1)];
    float height[RESOLUTION+1][RESOLUTION];
    float height_v[RESOLUTION+1][RESOLUTION];
    const float damping = 0.66;
    
public:
    double deltaTime;
    double currentTime;
    
    Water( GLFWwindow* window, Controls* controls );
    void init_height();
    float get_height( int i, int j, int t );
    void update_height();
    void update_normal_vertices();
    void droplet();
    void display();
};

#endif /* water_hpp */
