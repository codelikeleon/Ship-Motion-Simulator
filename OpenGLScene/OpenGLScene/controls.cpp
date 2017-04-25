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
    
    static double lastTime = glfwGetTime();
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);
    
    //store current cursor position
    double xPos, yPos;
    glfwGetCursorPos( window, &xPos, &yPos );
    glfwGetWindowSize( window, &width, &height );
    
    //reset cursor for the next frame
    glfwSetCursorPos( window, width/2, height/2 ); //set to middle of screen;
    
    horizontalAngle += mouseSpeed * deltaTime * float( width/2 - xPos );
    verticalAngle += mouseSpeed * deltaTime * float( height/2 - yPos );
    
    vec3 direction = vec3(
        cos( verticalAngle ) * sin( horizontalAngle ),
        sin( verticalAngle ),
        cos( verticalAngle ) * cos( horizontalAngle )
    );
    
    vec3 right = vec3(
        sin( horizontalAngle - M_PI/2 ),
        0,
        cos( horizontalAngle - M_PI/2 )
    );
    
    vec3 up = cross( right, direction );
    
    //Forwards movement
    if ( glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS ) {
        position += ( direction * deltaTime * speed );
    }
    
    //Backwards movement
    if ( glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS ) {
        position -= ( direction * deltaTime * speed );
    }
    
    //Rightwards strafe
    if ( glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS ) {
        position += ( right * deltaTime * speed );
    }
    
    //Leftwards strafe
    if ( glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS ) {
        position -= ( right * deltaTime * speed );
    }
    
    float FoV = initialFoV;   //Could add potential for mousewheel / button zoom.
    
    projectionMatrix = perspective( FoV, 4.0f / 3.0f, 0.1f, 100.0f );
    viewMatrix = lookAt( position, position + direction, up );
    
    //reset lastime to current time for next frame
    lastTime = currentTime;
}

Controls::Controls( GLFWwindow *window ) {
    this->window = window;
}

