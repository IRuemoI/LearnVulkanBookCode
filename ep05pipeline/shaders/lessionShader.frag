#version 450

#extension GL_ARB_separate_shader_objects:enable

layout(location = 0) in vec3 inColor;
layout(location = 0) out vec4 outColor;

void main() {
	//输出的颜色直接设置为从顶点着色器传入的颜色组装上透明通道的值
	outColor = vec4(inColor, 1.0);
}