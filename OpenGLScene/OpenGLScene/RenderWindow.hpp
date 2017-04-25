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
    
    bool initiliased;
    int screenWidth, screenHeight;
    
    GLFWwindow *window;
    const GLuint WIDTH = 1024, HEIGHT = 768;
    
    Controls *controls;
    
    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint shaderProgram;
    
    //Vertex Array/Buffer Objects:
    GLuint VAO, VBO;
    GLuint uvBuffer;
    GLuint normalBuffer;
    GLuint elementBuffer; //For VBO indexing
    
    mat4 Projection;
    mat4 View;
    mat4 Model;
    mat4 mvp;
    GLuint MatrixID;
    GLuint ViewMatrixID;
    GLuint ModelMatrixID;
    
    GLuint LightID;
    vec3 lightPos;
    
    GLuint texture;
    GLuint textureID;
    
    Skybox *skybox;
    
    //For OBJ files:
    bool res;
    std::vector<glm::vec3> indexed_vertices;
    std::vector<glm::vec2> indexed_uvs;
    std::vector<glm::vec3> indexed_normals;
    std::vector<unsigned int> indices;  //used for VBO indexing

//TODO: Make appropriate member functions private
public:
    RenderWindow( GLFWwindow* window, Controls* controls );
    ~RenderWindow();
    void display();
    
    
    
    void setGLContext();
    void initBuffers();
    void enableArrays();
    void showPerformance();
};

#endif /* RenderWindow_hpp */
