#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in float vWave;

uniform sampler2D uTexture;
uniform float uTime;

void main() {
    // Center UVs
    vec2 uv = TexCoord - 0.5;

    // Swirl effect
    float angle = sin(length(uv) * 10.0 - uTime * 3.0) * 0.3;
    float s = sin(angle);
    float c = cos(angle);
    uv = mat2(c, -s, s, c) * uv;

    uv += 0.5;

    vec4 texColor = texture(uTexture, uv);

    // Color cycling
    float pulse = sin(uTime * 2.0) * 0.5 + 0.5;
    vec3 colorShift = vec3(
        texColor.r + pulse * 0.2,
        texColor.g + vWave * 2.0,
        texColor.b + (1.0 - pulse) * 0.2
    );

    FragColor = vec4(colorShift, texColor.a);
}
