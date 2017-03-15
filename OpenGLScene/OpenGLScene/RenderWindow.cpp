//
//  Render.cpp
//  OpenGLScene
//
//  Created by Leon Schiedermair on 13/03/2017.
//  Copyright © 2017 Leon Schiedermair. All rights reserved.
//

#include "RenderWindow.hpp"
    
/*************************
 *    Other Functions:   *
 ************************/

void RenderWindow::setGLContext() {
    // Set all the required options for GLFW
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    glewExperimental = GL_TRUE; // So GLEW uses a modern approach to retrieving function pointers and extensions
}


//Sets the controls for the window:
void RenderWindow::key_callback( GLFWwindow* window, int key, int scancode, int action, int mods ) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        std::cout << "ESC key pressed" << std::endl;
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void RenderWindow::initBuffers() {
    
    glGenVertexArrays( 1, &VAO );
    glBindVertexArray( VAO );
    
    //Vertex Buffer:
    glGenBuffers( 1, &VBO );
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( g_vertex_buffer_data ), g_vertex_buffer_data, GL_STATIC_DRAW );
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    
    //UV buffer:
    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( GLfloat ), ( GLvoid * ) 0 );
    
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof( GLfloat ), ( GLvoid * ) 0 );
    // This is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    
    //enable Z-Buffer: Requires GL_DEPTH_BUFFER_BIT in render loop.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
//        glBindVertexArray( 0 ); // Unbind VAO - unbind any buffer/array to prevent strange bugs
    
}

GLuint RenderWindow::loadBMP( const char * imagePath ) {
    
    unsigned char header[54];   //BMP files begin with a 54 byte header
    unsigned int dataPos;       //Position in file where the actual data begins
    unsigned int width, height;
    unsigned int imageSize;
    unsigned char * data;
    
    FILE * file = fopen(imagePath, "rb");
    if (!file ) {
        fprintf(stderr, "Unable to open file %s\n", imagePath);
        return -1;
    }
    
    if ( fread(header, 1, 54, file) != 54 ) { // If not 54 bytes read : problem
        printf("Not a correct BMP file\n");
        return false;
    }
    
    if ( header[0]!='B' || header[1]!='M' ) {   //Ensure it is actually a BMP
        fprintf(stderr, "Not a correct BMP file %s\n", imagePath);
        return -1;
    }
    
    // Read ints from the byte array
    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    width      = *(int*)&(header[0x12]);
    height     = *(int*)&(header[0x16]);
    
    // Some BMP files are misformatted, guess missing information
    if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
    if (dataPos==0)      dataPos=54; // The BMP header is done that way
    
    // Create a buffer
    data = new unsigned char [imageSize];
    
    // Read the actual data from the file into the buffer
    fread(data,1,imageSize,file);
    
    //Everything is in memory now, the file can be closed
    fclose(file);
    
    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);
    
    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // When MAGnifying the image (no bigger mipmap available), use LINEAR filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // When MINifying the image, use a LINEAR blend of two mipmaps, each filtered LINEARLY too
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // Generate mipmaps, by the way.
    glGenerateMipmap(GL_TEXTURE_2D);
    delete [] data;
    
    return textureID;
}

GLuint RenderWindow::loadDDS(const char * imagepath){
    
    unsigned char header[124];
    
    FILE *fp;
    
    /* try to open the file */
    fp = fopen(imagepath, "rb");
    if (fp == NULL)
        return 0;
    
    /* verify the type of file */
    char filecode[4];
    fread(filecode, 1, 4, fp);
    if (strncmp(filecode, "DDS ", 4) != 0) {
        fclose(fp);
        return 0;
    }
    
    /* get the surface desc */
    fread(&header, 124, 1, fp);
    
    unsigned int height      = *(unsigned int*)&(header[8 ]);
    unsigned int width         = *(unsigned int*)&(header[12]);
    unsigned int linearSize     = *(unsigned int*)&(header[16]);
    unsigned int mipMapCount = *(unsigned int*)&(header[24]);
    unsigned int fourCC      = *(unsigned int*)&(header[80]);
    
    unsigned char * buffer;
    unsigned int bufsize;
    /* how big is it going to be including all mipmaps? */
    bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
    buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
    fread(buffer, 1, bufsize, fp);
    /* close the file pointer */
    fclose(fp);
    
    unsigned int components  = (fourCC == FOURCC_DXT1) ? 3 : 4;
    unsigned int format;
    switch(fourCC)
    {
        case FOURCC_DXT1:
            format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
            break;
        case FOURCC_DXT3:
            format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
            break;
        case FOURCC_DXT5:
            format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
            break;
        default:
            free(buffer);
            return 0;
    }
    
    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);
    
    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
    unsigned int offset = 0;
    
    /* load the mipmaps */
    for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
    {
        unsigned int size = ((width+3)/4)*((height+3)/4)*blockSize;
        glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
                               0, size, buffer + offset);
        
        offset += size;
        width  /= 2;
        height /= 2;
    }
    free(buffer); 
    
    return textureID;
}

RenderWindow::RenderWindow() {
    //Initialise GLFW
    glfwInit();
    
    setGLContext();
    
    window = glfwCreateWindow( WIDTH, HEIGHT, "OpenGL Scene", nullptr, nullptr);
    glfwGetFramebufferSize( window, &screenWidth, &screenHeight );
    
    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        
        exit( EXIT_FAILURE );
    }
    
    glfwMakeContextCurrent( window );
    
    
    glfwSetKeyCallback(window, key_callback);
    
    // Initialize GLEW to setup the OpenGL Function pointers
    if ( GLEW_OK != glewInit( ) )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        exit( EXIT_FAILURE );
    }
    
    // Define the viewport dimensions
    glViewport( 0, 0, screenWidth, screenHeight );
    
    GLuint shaderProgram = LoadShaders("/Users/Leon/OneDrive - UniversityofExeter/Ship Motion Simulator/Projects/OpenGLScene/OpenGLScene/OpenGLScene/shaders/VertexShader.glsl", "/Users/Leon/OneDrive - UniversityofExeter/Ship Motion Simulator/Projects/OpenGLScene/OpenGLScene/OpenGLScene/shaders/FragmentShader.glsl");       //TODO: Change to relative path
    
    glfwSwapInterval(1);
    
    initBuffers();
    
    
    /*******************************
     *   Matrix transformations:   *
     ******************************/
    Projection = perspective( radians( 45.0f ), ( float ) screenWidth / ( float ) screenHeight, 0.1f, 100.0f );
    
    //Camera matrix:
    View = lookAt(
                  vec3(4, 3, 3),
                  vec3(0,0,0),
                  vec3(0,1,0)
                  );
    
    Model = mat4(1.0f);
    mvp = Projection * View * Model;
    
    //Send to GLSL:
    MatrixID = glGetUniformLocation(shaderProgram, "MVP");
    
    
    /*******************************
     *           Textures:         *
     ******************************/
    
    texture = loadDDS("/Users/Leon/OneDrive - UniversityofExeter/Ship Motion Simulator/Projects/OpenGLScene/OpenGLScene/OpenGLScene/uvtemplate.DDS");
    
    
    /*********************************
     *          RENDER LOOP:         *      Should be moved elsewhere?
     *********************************/
    while ( !glfwWindowShouldClose( window )) {
        glfwPollEvents();
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram( shaderProgram );
        
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
        
        // Draw the shape:
        glDrawArrays(GL_TRIANGLES, 0, 12*3);
        
        glfwSwapBuffers( window );
    }
    
    //De-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays( 1, &VAO );
    glDeleteBuffers( 1, &VBO );
    glfwTerminate();
}

