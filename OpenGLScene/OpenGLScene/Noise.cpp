//
//  Noise.cpp
//  OpenGLScene
//
//  Created by Leon Schiedermair on 17/04/2017.
//  Copyright © 2017 Leon Schiedermair. All rights reserved.
//

#include "Noise.hpp"

Noise::Noise() {
    
    unsigned int i = 0;
    while ( i < 256 )
        permut[i++] = rand() & MOD;
}

/*
 ** Function finding out the gradient corresponding to the coordinates
 */
int Noise::index( int i, int j, int k, int l ) {
    return ( permut[( l + permut[( k + permut[( j + permut[i & MOD] ) & MOD] ) & MOD] ) & MOD] & 0x1f );
}

/*
 ** Functions computing the dot product of the vector and the gradient
 */
float Noise::product( float a, char b ) {
    if ( b > 0 ) return a;
    if ( b < 0 ) return -a;
    return 0;
}

float Noise::dotProduct( float x1, char x2, float y1, char y2, float z1, char z2, float t1, char t2 ) {
    return ( product ( x1, x2 ) + product ( y1, y2 ) + product ( z1, z2 ) + product ( t1, t2 ) );
}

/*
 ** Interpolation functions:
 */
float Noise::spline5 ( float state ) {
    
    //improved spline :
    //(6x^5 - 15x^4 + 10x^3) is better than (3x^2 + 2x^3)
    float sqr = state * state;
    return state * sqr * ( 6 * sqr - 15 * state + 10 );
}

float Noise::linear ( float start, float end, float state ) {
    return start + ( end - start ) * state;
}

float Noise::genNoise( float x, float y, float z, float t ) {
    /* The unit hypercube containing the point */
    int x1 = ( int ) ( x > 0 ? x : x - 1 );
    int y1 = ( int ) ( y > 0 ? y : y - 1 );
    int z1 = ( int ) ( z > 0 ? z : z - 1 );
    int t1 = ( int ) ( t > 0 ? t : t - 1 );
    int x2 = x1 + 1;
    int y2 = y1 + 1;
    int z2 = z1 + 1;
    int t2 = t1 + 1;
    
    /* The 16 corresponding gradients */
    char* g0000 = gradient[index ( x1, y1, z1, t1 )];
    char* g0001 = gradient[index ( x1, y1, z1, t2 )];
    char* g0010 = gradient[index ( x1, y1, z2, t1 )];
    char* g0011 = gradient[index ( x1, y1, z2, t2 )];
    char* g0100 = gradient[index ( x1, y2, z1, t1 )];
    char* g0101 = gradient[index ( x1, y2, z1, t2 )];
    char* g0110 = gradient[index ( x1, y2, z2, t1 )];
    char* g0111 = gradient[index ( x1, y2, z2, t2 )];
    char* g1000 = gradient[index ( x2, y1, z1, t1 )];
    char* g1001 = gradient[index ( x2, y1, z1, t2 )];
    char* g1010 = gradient[index ( x2, y1, z2, t1 )];
    char* g1011 = gradient[index ( x2, y1, z2, t2 )];
    char* g1100 = gradient[index ( x2, y2, z1, t1 )];
    char* g1101 = gradient[index ( x2, y2, z1, t2 )];
    char* g1110 = gradient[index ( x2, y2, z2, t1 )];
    char* g1111 = gradient[index ( x2, y2, z2, t2 )];
    
    //Vectors
    float dx1 = x - x1;
    float dx2 = x - x2;
    float dy1 = y - y1;
    float dy2 = y - y2;
    float dz1 = z - z1;
    float dz2 = z - z2;
    float dt1 = t - t1;
    float dt2 = t - t2;
    
    //Dot products
    float b0000 = dotProduct( dx1, g0000[0], dy1, g0000[1], dz1, g0000[2], dt1, g0000[3] );
    float b0001 = dotProduct( dx1, g0001[0], dy1, g0001[1], dz1, g0001[2], dt2, g0001[3] );
    float b0010 = dotProduct( dx1, g0010[0], dy1, g0010[1], dz2, g0010[2], dt1, g0010[3] );
    float b0011 = dotProduct( dx1, g0011[0], dy1, g0011[1], dz2, g0011[2], dt2, g0011[3] );
    float b0100 = dotProduct( dx1, g0100[0], dy2, g0100[1], dz1, g0100[2], dt1, g0100[3] );
    float b0101 = dotProduct( dx1, g0101[0], dy2, g0101[1], dz1, g0101[2], dt2, g0101[3] );
    float b0110 = dotProduct( dx1, g0110[0], dy2, g0110[1], dz2, g0110[2], dt1, g0110[3] );
    float b0111 = dotProduct( dx1, g0111[0], dy2, g0111[1], dz2, g0111[2], dt2, g0111[3] );
    float b1000 = dotProduct( dx2, g1000[0], dy1, g1000[1], dz1, g1000[2], dt1, g1000[3] );
    float b1001 = dotProduct( dx2, g1001[0], dy1, g1001[1], dz1, g1001[2], dt2, g1001[3] );
    float b1010 = dotProduct( dx2, g1010[0], dy1, g1010[1], dz2, g1010[2], dt1, g1010[3] );
    float b1011 = dotProduct( dx2, g1011[0], dy1, g1011[1], dz2, g1011[2], dt2, g1011[3] );
    float b1100 = dotProduct( dx2, g1100[0], dy2, g1100[1], dz1, g1100[2], dt1, g1100[3] );
    float b1101 = dotProduct( dx2, g1101[0], dy2, g1101[1], dz1, g1101[2], dt2, g1101[3] );
    float b1110 = dotProduct( dx2, g1110[0], dy2, g1110[1], dz2, g1110[2], dt1, g1110[3] );
    float b1111 = dotProduct( dx2, g1111[0], dy2, g1111[1], dz2, g1111[2], dt2, g1111[3] );
    
    //Interpolation
    float idx1 = spline5 ( dx1 );
    float idy1 = spline5 ( dy1 );
    float idz1 = spline5 ( dz1 );
    float idt1 = spline5 ( dt1 );
    
    float b111 = linear ( b1110, b1111, idt1 );
    float b110 = linear ( b1100, b1101, idt1 );
    float b101 = linear ( b1010, b1011, idt1 );
    float b100 = linear ( b1000, b1001, idt1 );
    float b011 = linear ( b0110, b0111, idt1 );
    float b010 = linear ( b0100, b0101, idt1 );
    float b001 = linear ( b0010, b0011, idt1 );
    float b000 = linear ( b0000, b0001, idt1 );

    float b11 = linear ( b110, b111, idz1 );
    float b10 = linear ( b100, b101, idz1 );
    float b01 = linear ( b010, b011, idz1 );
    float b00 = linear ( b000, b001, idz1 );
    
    float b1 = linear ( b10, b11, idy1 );
    float b0 = linear ( b00, b01, idy1 );
    
    return linear ( b0, b1, idx1 );
}




