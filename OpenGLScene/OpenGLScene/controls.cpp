//
//  controls.cpp
//  OpenGLScene
//
//  Created by Leon Schiedermair on 15/03/2017.
//  Copyright © 2017 Leon Schiedermair. All rights reserved.
//

#include "controls.hpp"
    
glm::mat4 Controls::getViewMatrix() {
    return viewMatrix;
}
    
glm::mat4 Controls::getProjectionMatrix() {
    return projectionMatrix;
}

void Controls::computeMatricesFromInputs() {
    
    currentTime = glfwGetTime();
    deltaTime = float(currentTime - lastTime);
    
    glfwGetCursorPos(window, &xPos, &yPos);
    glfwGetWindowSize(window, &width, &height);
    glfwSetCursorPos(window, width/2, height/2); //set to middle of screen;
    
    horizontalAngle += mouseSpeed * deltaTime * float(width/2 - xPos);
    verticalAngle += mouseSpeed * deltaTime * float(height/2 - yPos);
    
    direction = vec3(
        cos(verticalAngle) * sin(horizontalAngle),
        sin(verticalAngle),
        cos(verticalAngle) * cos(horizontalAngle)
    );
    
    right = vec3(
        sin(horizontalAngle - M_PI/2),
        0,
        cos(horizontalAngle - M_PI/2)
    );
    
    up = cross(right, direction);
    
    //Forwards movement
    if ( glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS ) {
        position += direction * deltaTime * speed;
        std::cerr << "W key pressed." << std::endl;
    }
    
    //Backwards movement
    if ( glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS ) {
        position -= direction * deltaTime * speed;
        std::cerr << "S key pressed." << std::endl;
    }
    
    //Rightwards strafe
    if ( glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS ) {
        position += right * deltaTime * speed;
        std::cerr << "D key pressed." << std::endl;
    }
    
    //Leftwards strafe
    if ( glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS ) {
        position -= right * deltaTime * speed;
        std::cerr << "A key pressed." << std::endl;
    }
    
    FoV = initialFoV;   //Could add potential for mousewheel / button zoom.
    
    projectionMatrix = perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);
    viewMatrix = lookAt(position, position + direction, up);
    
}

Controls::Controls( GLFWwindow *window ) {
    this->window = window;
    this->lastTime = glfwGetTime();
    
}

