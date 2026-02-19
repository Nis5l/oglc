#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
out vec2 TexCoord;
uniform mat4 view;
uniform mat4 transform;
uniform mat4 projection;
void main() {
	gl_Position = projection * view * transform * vec4(aPos, 1.0);
	TexCoord = aUV;
//	gl_Position = projection * transform * vec4(aPos, 1.0);
//	gl_Position = transform * vec4(aPos, 1.0);
//	gl_Position = vec4(aPos, 1.0);
}
