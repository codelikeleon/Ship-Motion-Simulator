//
//  shader.hpp
//  OpenGLScene
//
//  Created by Leon Schiedermair on 02/03/2017.
//  Copyright © 2017 Leon Schiedermair. All rights reserved.
//

#ifndef SHADER_HPP
#define SHADER_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

#include <GL/glew.h>

GLuint LoadShaders( const char * vertex_file_path,const char * fragment_file_path );

#endif
