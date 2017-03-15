//
//  controls.cpp
//  OpenGLScene
//
//  Created by Leon Schiedermair on 15/03/2017.
//  Copyright © 2017 Leon Schiedermair. All rights reserved.
//

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "controls.hpp"

using namespace glm;
    
glm::mat4 Controls::getViewMatrix() {
    return viewMatrix;
}
    
glm::mat4 Controls::getProjectionMatrix() {
    return projectionMatrix;
}

Controls::Controls( GLFWwindow *window ) {
    this->window = window;
}
