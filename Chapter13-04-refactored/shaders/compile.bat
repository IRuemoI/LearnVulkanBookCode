@echo off
echo Compiling shaders...

glslc shader.vert -o shader.vert.spv
glslc shader.frag -o shader.frag.spv
glslc cubemap.vert -o cubemap.vert.spv
glslc cubemap.frag -o cubemap.frag.spv

echo Shader compilation complete!
pause
