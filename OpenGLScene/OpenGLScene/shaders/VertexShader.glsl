#version 330 core

layout (location = 0) in vec3 position;
layout(location = 1) in vec2 vertexUV;

uniform mat4 MVP;

out vec2 uv;


void main() {
    uv = vertexUV;
    gl_Position = MVP * vec4(position.x, position.y, position.z, 1.0);
}
