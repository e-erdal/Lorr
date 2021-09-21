shaderc -f %1.v.glsl -o "../../Output/Shader/%1.v.vs3" --type vertex --varyingdef %1.vd --platform windows -p vs_3_0    -O 3
shaderc -f %1.v.glsl -o "../../Output/Shader/%1.v.vs4" --type vertex --varyingdef %1.vd --platform windows -p vs_4_0    -O 3
shaderc -f %1.v.glsl -o "../../Output/Shader/%1.v.vs5" --type vertex --varyingdef %1.vd --platform windows -p vs_5_0    -O 3
shaderc -f %1.v.glsl -o "../../Output/Shader/%1.v.spv" --type vertex --varyingdef %1.vd --platform linux   -p spirv     -O 3
shaderc -f %1.v.glsl -o "../../Output/Shader/%1.v.met" --type vertex --varyingdef %1.vd --platform osx     -p metal     -O 3
shaderc -f %1.v.glsl -o "../../Output/Shader/%1.v.glc" --type vertex --varyingdef %1.vd --platform windows -p 120       -O 3

shaderc -f %1.f.glsl -o "../../Output/Shader/%1.f.vs3" --type fragment --varyingdef %1.vd --platform windows -p ps_3_0  -O 3
shaderc -f %1.f.glsl -o "../../Output/Shader/%1.f.vs4" --type fragment --varyingdef %1.vd --platform windows -p ps_4_0  -O 3
shaderc -f %1.f.glsl -o "../../Output/Shader/%1.f.vs5" --type fragment --varyingdef %1.vd --platform windows -p ps_5_0  -O 3
shaderc -f %1.f.glsl -o "../../Output/Shader/%1.f.spv" --type fragment --varyingdef %1.vd --platform linux   -p spirv   -O 3
shaderc -f %1.f.glsl -o "../../Output/Shader/%1.f.met" --type fragment --varyingdef %1.vd --platform osx     -p metal   -O 3
shaderc -f %1.f.glsl -o "../../Output/Shader/%1.f.glc" --type fragment --varyingdef %1.vd --platform windows -p 120     -O 3