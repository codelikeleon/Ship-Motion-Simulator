#version 330 core

in vec2 uv;
in vec3 position_worldspace;
in vec3 normal_cameraspace;
in vec3 eye_direction_cameraspace;
in vec3 light_direction_cameraspace;

out vec3 color;

uniform sampler2D textureSampler;
uniform mat4 MV;
uniform vec3 light_position_worldspace;

void main() {
    vec3 light_color = vec3( 1, 1, 1 );
    float light_power = 50.0f;
    
    vec3 material_diffuse_color = texture( textureSampler, uv ).rgb;
    vec3 material_ambient_color = vec3( 0.1, 0.1, 0.1 ) * material_diffuse_color;
    vec3 material_specular_color = vec3( 0.3, 0.3, 0.3 );
    
    vec3 n = normalize( normal_cameraspace );
    vec3 l = normalize( light_direction_cameraspace );
    
    // Distance to the light
    float distance = length( light_position_worldspace - position_worldspace );

    // Cosine of the angle between the normal and the light direction,
    // clamped above 0
    //  - light is at the vertical of the triangle -> 1
    //  - light is perpendicular to the triangle -> 0
    float cosTheta = clamp( dot( n, l ), 0, 1 );
    
    // Eye vector (towards the camera)
    vec3 E = normalize( eye_direction_cameraspace );
    
    // Direction in which the triangle reflects the light
    vec3 R = reflect( -l, n );
    
    // Cosine of the angle between the Eye vector and the Reflect vector,
    // clamped to 0
    //  - Looking into the reflection -> 1
    //  - Looking elsewhere -> < 1
    float cosAlpha = clamp( dot( E, R ), 0, 1 );
    
    color = material_ambient_color +
            material_diffuse_color  * light_color * light_power * cosTheta / ( distance * distance ) +
            material_specular_color * light_color * light_power * pow( cosAlpha, 5 ) / ( distance * distance )
}
