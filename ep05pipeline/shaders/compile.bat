REM 需要设置环境变量VULKAN_SDK，也可以手动设置SDK路径
%VULKAN_SDK%\Bin\glslangValidator.exe  -V lessionShader.vert -o vs.spv

%VULKAN_SDK%\Bin\glslangValidator.exe  -V lessionShader.frag -o fs.spv

pause