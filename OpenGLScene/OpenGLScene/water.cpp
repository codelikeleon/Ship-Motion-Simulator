//
//  water.cpp
//  OpenGLScene
//
//  Created by Leon Schiedermair on 17/04/2017.
//  Copyright © 2017 Leon Schiedermair. All rights reserved.
//

#include "water.hpp"


/*
 *  WATER CLASS:
 */

void Water::init_height()
{
    float delta = 2. / RESOLUTION;
    float y, x, x2, y2, xx, yy;
    int i,j;
    for(j = 0; j < RESOLUTION; j++)
        for(i = 0; i <= RESOLUTION; i++)
        {
            y = (j + 1) * delta - 1;
            x = i * delta - 1;
            x2 = x - 3;
            y2 = y + 1;
            xx = x2 * x2;
            yy = y2 * y2;
            height[i][j] = (3 + 2 * sinf (20 * sqrtf (xx + yy) ) + noise->genNoise(10 * x, 10 * y, 0, 0)) / 200;
            height_v[i][j] = 0;
        }
}

float Water::get_height(int i, int j, int t) {
    float delta = 2. / RESOLUTION;
    float y = (j + 1) * delta - 1;
    float x = i * delta - 1;
    return (height[i][j] + noise->genNoise(10 * x, 10 * y, t, 0) / 200);
}

void Water::update_height() {
    int i,j;
    for(j = 0; j < RESOLUTION; j++)
        for(i = 0; i <= RESOLUTION; i++)
        {
            float height_east = height[MIN(i+1,RESOLUTION)][j];
            float height_west = height[MAX(i-1,0)][j];
            float height_south = height[i][MAX(j-1,0)];
            float height_north = height[i][MIN(j+1, RESOLUTION-1)];
            height_v[i][j] += ((height_west+ height_east + height_south + height_north)/4 - height[i][j]);
            height_v[i][j] *= damping;
            height[i][j] += height_v[i][j];
        }
}

void Water::update_normal_vertices() {
    //jacob you'll need to get these from the rest of your code or at least the time variables
    double t = glfwGetTime() * 2.; //currentTime / 1000.;   //should t = deltaTime? I think glutGet(GLUT_ELAPSED_TIME) = the time difference from when the function was last called.
    
    float delta = 2. / RESOLUTION;
    unsigned int length = 2 * (RESOLUTION + 1);
    float xn = (RESOLUTION + 1) * delta + 1;
    unsigned int i;
    unsigned int j;
    float x;
    float y;
    unsigned int indice;
    unsigned int preindice;
    
    GLfloat v1x;
    GLfloat v1y;
    GLfloat v1z;
    
    GLfloat v2x;
    GLfloat v2y;
    GLfloat v2z;
    
    GLfloat v3x;
    GLfloat v3y;
    GLfloat v3z;
    
    GLfloat vax;
    GLfloat vay;
    GLfloat vaz;
    
    GLfloat vbx;
    GLfloat vby;
    GLfloat vbz;
    
    GLfloat nx;
    GLfloat ny;
    GLfloat nz;
    
    GLfloat l;
    
    /*update height*/
    if(t-t_old > (1.f/30)) {
        //printf("UPDATING HEIGHT\n");
        update_height();
        t_old = t;
    }
    
    /* Vertices */
    for (j = 0; j < RESOLUTION; j++)
    {
        y = (j + 1) * delta - 1;
        for (i = 0; i <= RESOLUTION; i++)
        {
            indice = 6 * (i + j * (RESOLUTION + 1));
            
            x = i * delta - 1;
            surface[indice + 3] = x;
            surface[indice + 4] = get_height (i, j, t);
            surface[indice + 5] = y;
            if (j != 0)
            {
                /* Values were computed during the previous loop */
                preindice = 6 * (i + (j - 1) * (RESOLUTION + 1));
                surface[indice] = surface[preindice + 3];
                surface[indice + 1] = surface[preindice + 4];
                surface[indice + 2] = surface[preindice + 5];
            }
            else
            {
                surface[indice] = x;
                surface[indice + 1] = get_height (i, j, t);
                surface[indice + 2] = -1;
            }
        }
    }
    
    /* Normals */
    for (j = 0; j < RESOLUTION; j++)
        for (i = 0; i <= RESOLUTION; i++)
        {
            indice = 6 * (i + j * (RESOLUTION + 1));
            
            v1x = surface[indice + 3];
            v1y = surface[indice + 4];
            v1z = surface[indice + 5];
            
            v2x = v1x;
            v2y = surface[indice + 1];
            v2z = surface[indice + 2];
            
            if (i < RESOLUTION)
            {
                v3x = surface[indice + 9];
                v3y = surface[indice + 10];
                v3z = v1z;
            }
            else
            {
                v3x = xn;
                v3y = get_height (i, j, t);
                v3z = v1z;
            }
            
            vax =  v2x - v1x;
            vay =  v2y - v1y;
            vaz =  v2z - v1z;
            
            vbx = v3x - v1x;
            vby = v3y - v1y;
            vbz = v3z - v1z;
            
            nx = (vby * vaz) - (vbz * vay);
            ny = (vbz * vax) - (vbx * vaz);
            nz = (vbx * vay) - (vby * vax);
            
            l = sqrtf (nx * nx + ny * ny + nz * nz);
            if (l != 0)
            {
                l = 1 / l;
                normal[indice + 3] = nx * l;
                normal[indice + 4] = ny * l;
                normal[indice + 5] = nz * l;
            }
            else
            {
                normal[indice + 3] = 0;
                normal[indice + 4] = 1;
                normal[indice + 5] = 0;
            }
            
            
            if (j != 0)
            {
                /* Values were computed during the previous loop */
                preindice = 6 * (i + (j - 1) * (RESOLUTION + 1));
                normal[indice] = normal[preindice + 3];
                normal[indice + 1] = normal[preindice + 4];
                normal[indice + 2] = normal[preindice + 5];
            }
            else
            {
                /* 	    v1x = v1x; */
                v1y = get_height (i, j, t);
                v1z = (j - 1) * delta - 1;
                
                /* 	    v3x = v3x; */
                v3y = get_height (i, j, t);
                v3z = v2z;
                
                vax = v1x - v2x;
                vay = v1y - v2y;
                vaz = v1z - v2z;
                
                vbx = v3x - v2x;
                vby = v3y - v2y;
                vbz = v3z - v2z;
                
                nx = (vby * vaz) - (vbz * vay);
                ny = (vbz * vax) - (vbx * vaz);
                nz = (vbx * vay) - (vby * vax);
                
                l = sqrtf (nx * nx + ny * ny + nz * nz);
                if (l != 0)
                {
                    l = 1 / l;
                    normal[indice] = nx * l;
                    normal[indice + 1] = ny * l;
                    normal[indice + 2] = nz * l;
                }
                else
                {
                    normal[indice] = 0;
                    normal[indice + 1] = 1;
                    normal[indice + 2] = 0;
                }
            }
        }
    t_old = t;
}

void Water::droplet(void)
{
    int i, j;
    for (j = 20 ; j < 40; j++)
        for (i = 20; i <= 40; i++)
        {
            height[i][j] *= 4;
            height_v[i][j] = 0;
        }
}

void Water::display() {
    
    const unsigned int length = 2 * (RESOLUTION + 1);
    update_normal_vertices();
    
    //Bind to the water VAO
    glBindVertexArray( VAO );
    //Use shader for water
    glUseProgram(shaderProgram);
    
    //Compute camera view position & movement controls
    controls->computeMatricesFromInputs();
    mat4 projectionMatrix = controls->getProjectionMatrix();
    mat4 viewMatrix = controls->getViewMatrix();
    mat4 modelMatrix = mat4( 1.0 );
    mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;
    glUniformMatrix4fv( MatrixID, 1, GL_FALSE, &mvp[0][0] );
    glUniformMatrix4fv( ModelMatrixID, 1, GL_FALSE, &modelMatrix[0][0] );
    glUniformMatrix4fv( ViewMatrixID, 1, GL_FALSE, &viewMatrix[0][0] );
    
    //Vertex Buffer: Update the vertices
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof(surface), surface, GL_STATIC_DRAW );    //incorrect? // sizeof(surface), surface
    
    if (wire_frame) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  //Wireframe mode
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(
                          0,                  // attribute 0. Must match the layout in the shader.
                          3,                  // size
                          GL_FLOAT,           // type
                          GL_FALSE,           // normalized?
                          0,                  // stride
                          (void*)0            // array buffer offset
                          );
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, (6 * RESOLUTION * (RESOLUTION + 1))/3 ); //incorrect?
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  //Return to normal non-wireframe rendering
    glDisableVertexAttribArray(0);
}


Water::Water( GLFWwindow* window, Controls* controls ) {
    
    this->window = window;
    this->controls = controls;
    
    wire_frame = true;
    normals = 0;
    xold = 0;
    yold = 0;
    noise = new Noise();
    
    init_height();
    
    shaderProgram = LoadShaders( "shaders/WaterVertexShader.glsl", "shaders/WaterFragmentShader.glsl" );    //may only need 1 fragment shader
    //Send handles to GLSL:
    MatrixID = glGetUniformLocation( shaderProgram, "MVP" );
    ViewMatrixID = glGetUniformLocation( shaderProgram, "V" );
    ModelMatrixID = glGetUniformLocation( shaderProgram, "M" );
    
    //Water VAO / VBOs:
    glGenVertexArrays( 1, &VAO );
    glBindVertexArray( VAO );
    
    //Vertex Buffer:
    glGenBuffers( 1, &VBO );
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof(surface), surface, GL_STATIC_DRAW );    //incorrect? // sizeof(surface), surface
    
    //Normal buffer:
    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normal), normal, GL_STATIC_DRAW);          //sizeof(normal), normal
    

}
