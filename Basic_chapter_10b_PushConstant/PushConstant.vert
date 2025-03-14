#version 450

layout (std140, binding = 0) uniform bufferVals { // UNIFORM_BLOCK_BINDING_INDEX
    mat4 mvp;
} myBufferVals;

layout (location = 0) in vec4 pos;
layout (location = 1) in vec4 inColor;
layout (location = 0) out vec4 outColor;

void main()
{
    outColor = inColor;
    gl_Position = myBufferVals.mvp * pos;
    gl_Position.z = (gl_Position.z + gl_Position.w) / 2.0;
}
