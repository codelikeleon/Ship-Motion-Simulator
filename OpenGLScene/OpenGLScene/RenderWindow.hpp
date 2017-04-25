//
//  Render.hpp
//  OpenGLScene
//
//  Created by Leon Schiedermair on 13/03/2017.
//  Copyright © 2017 Leon Schiedermair. All rights reserved.
//

#ifndef RenderWindow_hpp
#define RenderWindow_hpp

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "controls.hpp"
#include "LoadFunctions.hpp"
#include "Skybox.hpp"

using namespace glm;

class RenderWindow {
    
    const GLuint WIDTH = 1024, HEIGHT = 768;
    int screenWidth, screenHeight;
    GLFWwindow* window;
    Controls* controls;
    Skybox* skybox;
    
    GLuint shaderProgram;
    GLuint texture;
    GLuint textureID;
    
    //Vertex Array/Buffer Objects:
    GLuint VAO, VBO;
    GLuint uvBuffer;
    GLuint normalBuffer;
    GLuint elementBuffer; //For VBO indexing
    
    //For OBJ files:
    bool res;
    std::vector<glm::vec3> indexed_vertices;
    std::vector<glm::vec2> indexed_uvs;
    std::vector<glm::vec3> indexed_normals;
    std::vector<unsigned int> indices;  //used for VBO indexing
    
    mat4 Projection;
    mat4 View;
    mat4 Model;
    mat4 mvp;
    GLuint MatrixID;
    GLuint ViewMatrixID;
    GLuint ModelMatrixID;
    
    GLuint LightID;
    vec3 lightPos;
    
//TODO: Make appropriate member functions private
public:
    RenderWindow( GLFWwindow* window, Controls* controls );
    ~RenderWindow();
    
    void display();
    void initBuffers();
    void enableArrays();
};

#endif /* RenderWindow_hpp */
