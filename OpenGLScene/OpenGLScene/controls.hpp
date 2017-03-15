//
//  controls.hpp
//  OpenGLScene
//
//  Created by Leon Schiedermair on 15/03/2017.
//  Copyright © 2017 Leon Schiedermair. All rights reserved.
//

#ifndef controls_hpp
#define controls_hpp

#include <stdio.h>

class Controls {
    
    GLFWwindow *window;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    
public:
    Controls( GLFWwindow *window );
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
    
};


#endif /* controls_hpp */
