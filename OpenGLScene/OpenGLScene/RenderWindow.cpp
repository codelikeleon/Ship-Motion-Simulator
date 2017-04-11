//
//  Render.cpp
//  OpenGLScene
//
//  Created by Leon Schiedermair on 13/03/2017.
//  Copyright © 2017 Leon Schiedermair. All rights reserved.
//
//  Notes:
//  - Project working directory for Xcode changed in:
//    Product > Scheme > Edit Scheme > Run > Options > Working Directory.
//  - Rendering problem caused by using shorts instead of ints for the vertex indices.
//    This is fixed by using ints for 'vector<unsigned int> indices' and the buffers.
//

#include "RenderWindow.hpp"


static GLfloat ground_vertices[] = {
    -1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
};


void RenderWindow::init_height()
{
	const float delta = 2. / RESOLUTION;
	float y,x,x2,y2,xx,yy;
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
			//jacob this is the height variable i added in the header file if it doesn't recognise it
			height[i][j] = (3 + 2 * sinf (20 * sqrtf (xx + yy) ) + Noise (10 * x, 10 * y, 0, 0)) / 200;
			height_v[i][j] = 0;
		}
}

//unsure if this is correct...
static float get_height(const float x, const float y, const float t)
{
    const float x2 = x - 3;
    const float y2 = y + 1;
    const float xx = x2 * x2;
    const float yy = y2 * y2;
    return ((2 * sinf (20 * sqrtf (xx + yy) - 4 * t) +
             Noise (10 * x, 10 * y, t, 0)) / 200);
}

/*
float RenderWindow::get_height(int i, int j) {
    return height[i][j];
}
*/
void RenderWindow::update_height(void)
{
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


void RenderWindow::update_normal_vertices()
{
	//jacob you'll need to get these from the rest of your code or at least the time variables
	
    static double t_old = glfwGetTime();
    const double t = glfwGetTime();     //should be divided by 1000?
    float deltaTime = float(t - t_old);
	const float delta = 2. / RESOLUTION;
	const unsigned int length = 2 * (RESOLUTION + 1);
	const float xn = (RESOLUTION + 1) * delta + 1;
	unsigned int i;
	unsigned int j;
	float x;
	float y;
	unsigned int indice;
	unsigned int preindice;

	/* Yes, I know, this is quite ugly... */
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

/*
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity ();
	glTranslatef (0, 0, -translate_z);
	glRotatef (rotate_y, 1, 0, 0);
	glRotatef (rotate_x, 0, 1, 0);
*/
	/*update height*/
    if(t-t_old > (1.f/30)) {
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

void RenderWindow::setGLContext() {
    // Set all the required options for GLFW
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    glewExperimental = GL_TRUE; // So GLEW uses a modern approach to retrieving function pointers and extensions
}

/*
 * Prints to the terminal the time taken to render one frame in milliseconds, averaged over the last second.
 * 16.66.. ms/frame == 60fps
 */
void RenderWindow::showPerformance() {
    
    static double performance_lastTime = glfwGetTime();
    static int nbFrames = 0;
    
    // Measure speed
    double currentTime = glfwGetTime();
    nbFrames++;
    if ( currentTime - performance_lastTime >= 1.0 ) { // If last prinf() was more than 1 sec ago
        // printf and reset timer
        printf("%f ms/frame\n", 1000.0/double(nbFrames));
        nbFrames = 0;
        performance_lastTime += 1.0;
    }
}

void RenderWindow::initBuffers() {
    
    glGenVertexArrays( 1, &VAO );
    glBindVertexArray( VAO );
    
	//jacob todo you'll need to added another 2 buffers for the other ones. Watch out though as you need to make sure whether buffers always keep the values from the array or need physically reloading each time
	
	
    //Vertex Buffer:
    glGenBuffers( 1, &VBO );
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW );
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    
    //UV buffer:
    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec3), &indexed_uvs[0], GL_STATIC_DRAW);
    
    //Normal buffer:
    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);
    
    //Element buffer:
    glGenBuffers(1, &elementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    
    
    //ground:
    glGenVertexArrays( 1, &VAO_ground );
    glBindVertexArray( VAO_ground );
    
    //Ground Buffer:
    glGenBuffers( 1, &VBO_ground );
    glBindBuffer( GL_ARRAY_BUFFER, VBO_ground );
    glBufferData( GL_ARRAY_BUFFER, sizeof(ground_vertices), ground_vertices, GL_STATIC_DRAW );
    
    
    //enable Z-Buffer: Requires GL_DEPTH_BUFFER_BIT in render loop.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
}

void RenderWindow::enableArrays() {
    
    glBindVertexArray( VAO );
    
	//jacob todo: same here you need a copy for your new arrays
	
    //Vertex
    glEnableVertexAttribArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, ( GLvoid * ) 0 );   //Try setting stride to 0 instead of 3 * sizeof( GLfloat )
    
    //UV
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, ( GLvoid * ) 0 );
    
    //Normal
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE, 0, ( GLvoid * ) 0 );
    
    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    
    
}

RenderWindow::RenderWindow() {
    //Initialise GLFW
    glfwInit();
    setGLContext();
	init_height();
    
    window = glfwCreateWindow( WIDTH, HEIGHT, "OpenGL Scene", nullptr, nullptr );
    glfwGetFramebufferSize( window, &screenWidth, &screenHeight );
    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        
        exit( EXIT_FAILURE );
    }
    
    glfwMakeContextCurrent( window );
    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited movement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //glfwSetKeyCallback(window, key_callback); //Causes any key press to close window
    
    controls = new Controls( window );
    
    // Initialize GLEW to setup the OpenGL Function pointers
    if ( GLEW_OK != glewInit( ) ) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        exit( EXIT_FAILURE );
    }
    
    // Define the viewport dimensions
    glViewport( 0, 0, screenWidth, screenHeight );
    
    /*******************************
     *           Shaders:          *
     ******************************/
    
    shaderProgram = LoadShaders("shaders/VertexShader.glsl", "shaders/FragmentShader.glsl");
    //Send handles to GLSL:
    MatrixID = glGetUniformLocation(shaderProgram, "MVP");
    ViewMatrixID = glGetUniformLocation(shaderProgram, "V");
    ModelMatrixID = glGetUniformLocation(shaderProgram, "M");
    LightID = glGetUniformLocation(shaderProgram, "light_position_worldspace");
    
    glUseProgram( shaderProgram );
    
    /*******************************
     *           Textures:         *
     ******************************/
    
    texture = LoadDDS("textures/suzanne.DDS");
    textureID = glGetUniformLocation(shaderProgram, "textureSampler");
    
    /*******************************
     *           OBJ files:        *
     ******************************/
    
    // Read our .obj file
    bool res = LoadAssImp("obj/cube.obj", indices, indexed_vertices, indexed_uvs, indexed_normals); //Put in if statement?
    /* 
     * Objects:
     *
     * cube.obj           - working, texture: cube.DDS
     * suzanne.obj        - working, texture: suzanne.DDS
     * rowBoat.obj        - working, no texture.
     * pirateShip.obj     - working, no texture.
     * yacht.obj          - working, no texture.
     * rubberDingy.obj    - sometimes works, no texture.
     * boat.obj           - not working, causes runtime error.
     * compatibleBoat.obj - not working, causes runtime error.
     * tugBoat.obj        - not working, causes runtime error.
     */
    
    glfwSwapInterval(1);
    initBuffers();
    
//    skybox = new Skybox();
    
    /*********************************
     *          RENDER LOOP:         *
     *********************************/
    while ( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0  ) {
        //TODO: Add delta time calculator here?
        glfwPollEvents();
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );   //sets background colour
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//jacob this is where vertices etc are updating from
		
		update_normal_vertices();
        
        showPerformance();
        
        //Shader:
        glUseProgram( shaderProgram );
        MatrixID = glGetUniformLocation(shaderProgram, "MVP");
        ViewMatrixID = glGetUniformLocation(shaderProgram, "V");
        ModelMatrixID = glGetUniformLocation(shaderProgram, "M");
        LightID = glGetUniformLocation(shaderProgram, "light_position_worldspace");
        
        //Camera controls:
        controls->computeMatricesFromInputs();
        mat4 projectionMatrix = controls->getProjectionMatrix();
        mat4 viewMatrix = controls->getViewMatrix();
        mat4 modelMatrix = mat4( 1.0 );
        mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &viewMatrix[0][0]);
        
        //Lighting:
        lightPos = vec3(4, 4, 4);
        glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
        
        //Textures:
        glActiveTexture(GL_TEXTURE0); // Bind texture to Texture Unit 0
        glBindTexture(GL_TEXTURE_2D, texture);
        // Set the "textureSampler" sampler to user Texture Unit 0
        textureID = glGetUniformLocation(shaderProgram, "textureSampler");
        glUniform1i(textureID, 0);
        
        
        
        //ground
        glBindVertexArray(VAO_ground);
        glEnableVertexAttribArray( 3 );
        glBindBuffer( GL_ARRAY_BUFFER, VBO_ground );
        glVertexAttribPointer( 3, 3, GL_FLOAT, GL_FALSE, 0, ( GLvoid * ) 0 );
        
        glDrawArrays(GL_TRIANGLES, 0, 12*3);
        
        
        
        enableArrays();
        
        
        // Draw the vertices
        glDrawElements(
                       GL_TRIANGLES,      // mode
                       indices.size(),    // count
                       GL_UNSIGNED_INT,   // type
                       (void*)0           // element array buffer offset
                       );
        glBindVertexArray(0);
        
        
        //Draw skybox last:
//        glDepthFunc( GL_LEQUAL );
//        glUseProgram( skybox->skyboxShaderProgram );
//        viewMatrix = glm::mat4( glm::mat3( controls->getViewMatrix( ) ) );
//        
//        glUniformMatrix4fv( glGetUniformLocation( skybox->skyboxShaderProgram, "view" ), 1, GL_FALSE, glm::value_ptr( viewMatrix ) );
//        glUniformMatrix4fv( glGetUniformLocation( skybox->skyboxShaderProgram, "projection" ), 1, GL_FALSE, glm::value_ptr( projectionMatrix ) );
//        
//        // skybox cube
//        glBindVertexArray( skybox->skyboxVAO );
//        glBindTexture( GL_TEXTURE_CUBE_MAP, skybox->skyboxTexture );
//        glDrawArrays( GL_TRIANGLES, 0, 36 );
//        glBindVertexArray( 0 );
//        glDepthFunc( GL_LESS ); // Set depth function back to default
        
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        glDisableVertexAttribArray(3);
        glfwSwapBuffers( window );
    }
    
    //De-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays( 1, &VAO );
    glDeleteBuffers( 1, &VBO );
    glDeleteBuffers(1, &uvBuffer);
    glDeleteBuffers(1, &normalBuffer);
    glDeleteBuffers(1, &elementBuffer);
    glDeleteProgram(shaderProgram);
    glDeleteTextures(1, &texture);
    
    glfwTerminate();
}

