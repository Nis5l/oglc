#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

out vec2 TexCoord;
out float vWave;

uniform mat4 view;
uniform mat4 transform;
uniform mat4 projection;
uniform float uTime;

void main() {
    // Wave along Y based on X + time
    float wave = sin(aPos.x * 6.0 + uTime * 2.0) * 0.05;
    vec3 displacedPos = aPos + vec3(0.0, wave, 0.0);

    gl_Position = projection * view * transform * vec4(displacedPos, 1.0);
    TexCoord = aUV;
    vWave = wave;
}
