#version 450

//这个扩展可以问问AI
#extension GL_ARB_separate_shader_objects:enable

layout(location = 0) out vec3 outColor;

vec2 positions[3] = vec2[](vec2(0.0, -1.0), vec2(0.5, 0.0), vec2(-0.5, 0.0));

vec3 colors[3] = vec3[](vec3(1.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0));

void main() {
	//gl_Position是内置变量，表示当前正在处理顶点的索引
	gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);

	outColor = colors[gl_VertexIndex];
}
