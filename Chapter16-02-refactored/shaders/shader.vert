#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
} ubo;

layout(push_constant) uniform ObjectUniform {
    mat4 model;
} objectUbo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

void main() {
    fragColor = inColor;
    gl_Position = ubo.proj * ubo.view * objectUbo.model * vec4(inPosition, 1.0);
}