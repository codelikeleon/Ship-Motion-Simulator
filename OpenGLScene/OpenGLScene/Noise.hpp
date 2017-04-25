//
//  Noise.hpp
//  OpenGLScene
//
//  Created by Leon Schiedermair on 17/04/2017.
//  Copyright © 2017 Leon Schiedermair. All rights reserved.
//

#ifndef Noise_hpp
#define Noise_hpp

#define MOD 0xff

#include <stdio.h>
#include <stdlib.h>

class Noise {
    
    int permut[256];
    char gradient[32][4] =
    {
        { 1,  1,  1,  0}, { 1,  1,  0,  1}, { 1,  0,  1,  1}, { 0,  1,  1,  1},
        { 1,  1, -1,  0}, { 1,  1,  0, -1}, { 1,  0,  1, -1}, { 0,  1,  1, -1},
        { 1, -1,  1,  0}, { 1, -1,  0,  1}, { 1,  0, -1,  1}, { 0,  1, -1,  1},
        { 1, -1, -1,  0}, { 1, -1,  0, -1}, { 1,  0, -1, -1}, { 0,  1, -1, -1},
        {-1,  1,  1,  0}, {-1,  1,  0,  1}, {-1,  0,  1,  1}, { 0, -1,  1,  1},
        {-1,  1, -1,  0}, {-1,  1,  0, -1}, {-1,  0,  1, -1}, { 0, -1,  1, -1},
        {-1, -1,  1,  0}, {-1, -1,  0,  1}, {-1,  0, -1,  1}, { 0, -1, -1,  1},
        {-1, -1, -1,  0}, {-1, -1,  0, -1}, {-1,  0, -1, -1}, { 0, -1, -1, -1},
    };
    
public:
    Noise();
    int indice( int i, int j, int k, int l);
    float prod( float a, char b );
    float dot_prod ( float x1, char x2, float y1, char y2, float z1, char z2, float t1, char t2 );
    float spline5( float state );
    float linear( float start, float end, float state );
    float genNoise( float x, float y, float z, float t );
};

#endif /* Noise_hpp */
