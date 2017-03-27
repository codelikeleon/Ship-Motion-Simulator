//
//  controls.hpp
//  OpenGLScene
//
//  Created by Leon Schiedermair on 15/03/2017.
//  Copyright © 2017 Leon Schiedermair. All rights reserved.
//

#ifndef controls_hpp
#define controls_hpp

#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

class Controls {
    
    GLFWwindow *window;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::vec3 position = glm::vec3( 0, 0, 5 );
    
    float horizontalAngle = 3.14f;
    float verticalAngle = 0.0f;
    float initialFoV = 45.0f;
    
    float speed = 3.0f;         //Movement speed
    float mouseSpeed = 0.1f;   //Camera sensitivity
    int width, height;
    
//    double lastTime;
    
public:
    Controls( GLFWwindow *window );
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
    void computeMatricesFromInputs();
};

#endif /* controls_hpp */
