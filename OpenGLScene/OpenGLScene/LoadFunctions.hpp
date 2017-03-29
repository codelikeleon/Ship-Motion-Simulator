//
//  LoadFunctions.hpp
//  OpenGLScene
//
//  Created by Leon Schiedermair on 02/03/2017.
//  Copyright © 2017 Leon Schiedermair. All rights reserved.
//

#ifndef LOADFUNCTIONS_HPP
#define LOADFUNCTIONS_HPP

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <glm/glm.hpp>

// Include Asset Importer
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

using namespace std;

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

GLuint LoadShaders( const char * vertex_file_path,const char * fragment_file_path );
GLuint LoadBMP( const char * imagePath );
GLuint LoadDDS( const char * imagepath );
bool LoadOBJ(
             const char* obj_file_path,
             vector<glm::vec3> &out_vertices,
             vector<glm::vec2> &out_uvs,
             vector<glm::vec3> &out_normals
             );
bool LoadAssImp(
                const char * path,
                std::vector<unsigned int> & indices,
                std::vector<glm::vec3> & vertices,
                std::vector<glm::vec2> & uvs,
                std::vector<glm::vec3> & normals
                );

#endif
