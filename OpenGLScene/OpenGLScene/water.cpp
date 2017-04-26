//
//  water.cpp
//  OpenGLScene
//
//  Created by Leon Schiedermair on 17/04/2017.
//  Copyright © 2017 Leon Schiedermair. All rights reserved.
//

#include "water.hpp"

void Water::initHeight() {
    
    float delta = 2. / RES;
    float y, x, x2, y2, xx, yy;
    int i,j;
    for( j = 0; j < RES; j++ )
        for( i = 0; i <= RES; i++ )
        {
            y = ( j + 1 ) * delta - 1;
            x = i * delta - 1;
            x2 = x - 3;
            y2 = y + 1;
            xx = x2 * x2;
            yy = y2 * y2;
            height[i][j] = ( 5 + 2 * sinf ( 20 * sqrtf ( xx + yy ) ) + noise->genNoise( 10 * x, 10 * y, 0, 0 ) ) / 200;
            height_v[i][j] = 0;
        }
}

float Water::getHeight( int i, int j, int t ) {
    
    float delta = 2. / RES;
    float y = ( j + 1 ) * delta - 1;
    float x = i * delta - 1;
    return height[i][j] + noise->genNoise(10 * x, 10 * y, t, 0) / 200;
}

void Water::updateHeight() {
    
    for( int j = 0; j < RES; j++ )
        for( int i = 0; i <= RES; i++ ) {
            float eastHeight = height[MIN( i + 1, RES )][j];
            float westHeight = height[MAX( i - 1, 0 )][j];
            float southHeight = height[i][MAX( j - 1, 0 )];
            float northHeight = height[i][MIN( j + 1, RES - 1 )];
            height_v[i][j] += ( ( westHeight + eastHeight + southHeight + northHeight ) / 4 - height[i][j] );
            height_v[i][j] *= damping;
            height[i][j] += height_v[i][j];
        }
}

void Water::updateVertices() {
    
    double t = glfwGetTime() * 3.; //Increase multiplier to increase sim speed. 3 is a normal speed.
    
    float delta = 2. / RES;
    unsigned int length = 2 * ( RES + 1 );
    float xn = ( RES + 1 ) * delta + 1;
    unsigned int i;
    unsigned int j;
    float x;
    float y;
    unsigned int index;
    unsigned int indexPrev;
    
    GLfloat l;
    
    GLfloat nx;
    GLfloat ny;
    GLfloat nz;
    
    GLfloat vax;
    GLfloat vay;
    GLfloat vaz;
    
    GLfloat vbx;
    GLfloat vby;
    GLfloat vbz;
    
    GLfloat v1x;
    GLfloat v1y;
    GLfloat v1z;
    
    GLfloat v2x;
    GLfloat v2y;
    GLfloat v2z;
    
    GLfloat v3x;
    GLfloat v3y;
    GLfloat v3z;
    
    //update height
    if( t - t_old > ( 1.f / 30 ) ) {
        updateHeight();
        t_old = t;
    }
    
    //Vertices
    for ( j = 0; j < RES; j++ ) {
        y = ( j + 1 ) * delta - 1;
        for ( i = 0; i <= RES; i++ ) {
            index = 6 * ( i + j * ( RES + 1 ) );
            
            x = i * delta - 1;
            surface[index + 3] = x;
            surface[index + 4] = getHeight( i, j, t );
            surface[index + 5] = y;
            if ( j != 0 ) {
                /* Values were computed during the previous loop */
                indexPrev = 6 * ( i + ( j - 1 ) * ( RES + 1 ) );
                surface[index] = surface[indexPrev + 3];
                surface[index + 1] = surface[indexPrev + 4];
                surface[index + 2] = surface[indexPrev + 5];
            }
            else
            {
                surface[index] = x;
                surface[index + 1] = getHeight( i, j, t );
                surface[index + 2] = -1;
            }
        }
    }
    
    /* Normals */
    for ( j = 0; j < RES; j++ )
        for ( i = 0; i <= RES; i++ ) {
            index = 6 * ( i + j * ( RES + 1 ) );
            
            v1x = surface[index + 3];
            v1y = surface[index + 4];
            v1z = surface[index + 5];
            
            v2x = v1x;
            v2y = surface[index + 1];
            v2z = surface[index + 2];
            
            if ( i < RES ) {
                v3x = surface[index + 9];
                v3y = surface[index + 10];
                v3z = v1z;
            }
            else {
                v3x = xn;
                v3y = getHeight( i, j, t );
                v3z = v1z;
            }
            
            vax =  v2x - v1x;
            vay =  v2y - v1y;
            vaz =  v2z - v1z;
            
            vbx = v3x - v1x;
            vby = v3y - v1y;
            vbz = v3z - v1z;
            
            nx = ( vby * vaz ) - ( vbz * vay );
            ny = ( vbz * vax ) - ( vbx * vaz );
            nz = ( vbx * vay ) - ( vby * vax );
            
            l = sqrtf( nx * nx + ny * ny + nz * nz );
            if ( l != 0 ) {
                l = 1 / l;
                normal[index + 3] = nx * l;
                normal[index + 4] = ny * l;
                normal[index + 5] = nz * l;
            }
            else {
                normal[index + 3] = 0;
                normal[index + 4] = 1;
                normal[index + 5] = 0;
            }
            
            if ( j != 0 ) {
                //Values were camlculated in the previous loop
                indexPrev = 6 * ( i + ( j - 1 ) * ( RES + 1 ) );
                normal[index] = normal[indexPrev + 3];
                normal[index + 1] = normal[indexPrev + 4];
                normal[index + 2] = normal[indexPrev + 5];
            }
            else {
                //v1x stays the same
                v1y = getHeight( i, j, t );
                v1z = ( j - 1 ) * delta - 1;
                
                //v3x stays the same
                v3y = getHeight( i, j, t );
                v3z = v2z;
                
                vax = v1x - v2x;
                vay = v1y - v2y;
                vaz = v1z - v2z;
                
                vbx = v3x - v2x;
                vby = v3y - v2y;
                vbz = v3z - v2z;
                
                nx = ( vby * vaz ) - ( vbz * vay );
                ny = ( vbz * vax ) - ( vbx * vaz );
                nz = ( vbx * vay ) - ( vby * vax );
                
                l = sqrtf( nx * nx + ny * ny + nz * nz );
                if ( l != 0 ) {
                    l = 1 / l;
                    normal[index] = nx * l;
                    normal[index + 1] = ny * l;
                    normal[index + 2] = nz * l;
                }
                else {
                    normal[index] = 0;
                    normal[index + 1] = 1;
                    normal[index + 2] = 0;
                }
            }
        }
    t_old = t;
}

void Water::wave() {
    int i, j;
    for ( j = 0; j < 5; j++ )
        for ( i=0; i < RES+1; i++ ) {
            height[i][j] /= 3;
        }
    for ( j = 5; j < 15; j++ )
        for ( i=0; i < RES+1; i++ ) {
            height[i][j] *= 5;
        }
    for ( j = 15; j < RES+1; j++ )
        for ( i=0; i < RES+1; i++ ) {
            height[i][j] /= 3;
        }
}

void Water::droplet(void)
{
    int i, j;
    for ( j = 20; j < 40; j++ )
        for ( i = 20; i <= 40; i++ ) {
            height[i][j] *= 5;
            //height_v[i][j] = 0;
        }
    //Reduce height of the rest of grid to stop water level rising
    for ( j = 0; j < 20; j++ )
        for ( i=0; i < 20; i++ ) {
            height[i][j] /= 2;
        }
    for ( j = 40; j <= 64; j++ )
        for ( i=41; i<=65; i++ ) {
            height[i][j] /= 2;
        }
}

void Water::waterControls() {
    
    //Create wave on spacebar press
    if ( glfwGetKey( window, GLFW_KEY_SPACE ) == GLFW_PRESS && lastWave == 0 ) {
        printf( "~~~~~~~~~~~~~WAVE~~~~~~~~~~~~~\n" );
        wave();
        lastWave = 50;
    }
    if ( lastWave > 0 ) lastWave--;
    
    //Create droplet on 'E' press
    if ( glfwGetKey( window, GLFW_KEY_E ) == GLFW_PRESS && lastDroplet == 0 ) {
        printf("============DROPLET===========\n");
        droplet();
        lastDroplet = 50;
    }
    if ( lastDroplet > 0 ) lastDroplet--;
    
    //Toggle wireframe mode on 'F' press
    if ( glfwGetKey( window, GLFW_KEY_F ) == GLFW_PRESS && lastWireframe == 0 ) {
        printf( "Wireframe %s\n", wireframeMode ? "OFF" : "ON" );
        wireframeMode = wireframeMode ? false : true;
        lastWireframe = 20;
    }
    if ( lastWireframe > 0 ) lastWireframe--;
}

void Water::display() {
    
    const unsigned int length = 2 * (RES + 1);
    updateVertices();
    
    waterControls();
    
    //Bind to the water VAO
    glBindVertexArray( VAO );
    //Use shader for water
    glUseProgram( shaderProgram );
    
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
    glBufferData( GL_ARRAY_BUFFER, sizeof( surface ), surface, GL_STATIC_DRAW );    //incorrect? // sizeof(surface), surface
    
    if ( wireframeMode ) glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );  //Wireframe mode
    
    //Allows transparency.
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glDisable( GL_CULL_FACE );
    
    glEnableVertexAttribArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glVertexAttribPointer(
                          0,                  // attribute 0. Must match the layout in the shader.
                          3,                  // size
                          GL_FLOAT,           // type
                          GL_FALSE,           // normalized?
                          0,                  // stride
                          ( void* )0            // array buffer offset
                          );
    
    glDrawArrays( GL_TRIANGLE_STRIP, 0, ( 6 * RES * ( RES + 1 ) ) / 3 ); //incorrect?
    
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );  //Return to normal non-wireframe rendering
    glEnable( GL_CULL_FACE );
    glDisable( GL_BLEND );
    glDisableVertexAttribArray( 0 );
}

Water::Water( GLFWwindow* window, Controls* controls ) {
    
    this->window = window;
    this->controls = controls;
    
    wireframeMode = false;
    noise = new Noise();
    
    initHeight();
    
    shaderProgram = LoadShaders( "shaders/WaterVertexShader.glsl", "shaders/WaterFragmentShader.glsl" );
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
    glBufferData( GL_ARRAY_BUFFER, sizeof( surface ), surface, GL_STATIC_DRAW );
    
    //Normal buffer:
    glGenBuffers( 1, &normalBuffer );
    glBindBuffer( GL_ARRAY_BUFFER, normalBuffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof( normal ), normal, GL_STATIC_DRAW );
}
