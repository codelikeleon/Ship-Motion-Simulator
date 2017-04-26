//
//  water.hpp
//  OpenGLScene
//
//  Created by Leon Schiedermair on 17/04/2017.
//  Copyright © 2017 Leon Schiedermair. All rights reserved.
//

#ifndef water_hpp
#define water_hpp

#define RES 128
#define MIN(a,b) a<b ? a:b
#define MAX(a,b) a>b ? a:b

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
    GLuint shaderProgram;
    GLuint MatrixID;
    GLuint ViewMatrixID;
    GLuint ModelMatrixID;

    bool wireframeMode;
    float t_old = 0;
    int lastDroplet = 0;
    int lastWave = 0;
    int lastWireframe = 0;
    
    float surface[6 * RES * ( RES + 1 )];
    float normal[6 * RES * ( RES + 1 )];
    float height[RES+1][RES];
    float height_v[RES+1][RES];
    const float damping = 0.66;
    
public:
    double deltaTime;
    double currentTime;
    
    Water( GLFWwindow* window, Controls* controls );
    void initHeight();
    float getHeight( int i, int j, int t );
    void updateHeight();
    void updateVertices();
    void droplet();
    void wave();
    void waterControls();
    void display();
};

#endif /* water_hpp */
