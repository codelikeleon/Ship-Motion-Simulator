#version 330 core

// Input vertex data, different for all executions of this shader.
layout( location = 0 ) in vec3 vertexPosition_modelspace;

out vec4 fragmentColor;
// Values that stay constant for the whole mesh.
uniform mat4 MVP;

void main(){	

	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  MVP * vec4( vertexPosition_modelspace, 1 );

    fragmentColor = vec4( 0.0, 0.2, 1.0, 0.6 );
}

