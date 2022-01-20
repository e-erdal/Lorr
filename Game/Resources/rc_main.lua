-- This is an example file for Resource Compiler Meta.

-- Main resource folder
local rawResourcePath = "../../Game/Resources/"

-- 
function compile(pathList, resourceType)
    rc.CurrentOutputPath = "data/_r" .. resourceType
    for i, val in ipairs(pathList) do
        rc.compileResource(val[1], val[2], val[3], resourceType)
    end
end

-- ResourceType enum --
RT_Invalid = 0
RT_Texture = 1
RT_Audio   = 2
RT_Shader  = 3
RT_Font    = 4
RT_Model   = 5
-- -- -- -- -- -- -- --

-- SHADERS --
rc.CurrentCategory = "shader"
local shaderFiles = {
--   RAW RESOURCE PATH              REOURCE NAME  EXTENSION
    { rawResourcePath .. "Shaders", "terrain.v",  "hlsl" },
    { rawResourcePath .. "Shaders", "terrain.p",  "hlsl" },
    { rawResourcePath .. "Shaders", "model.v",    "hlsl" },
    { rawResourcePath .. "Shaders", "model.p",    "hlsl" },
    { rawResourcePath .. "Shaders", "batch.v",    "hlsl" },
    { rawResourcePath .. "Shaders", "batch.p",    "hlsl" },
    { rawResourcePath .. "Shaders", "font.v",     "hlsl" },
    { rawResourcePath .. "Shaders", "font.p",     "hlsl" },
    { rawResourcePath .. "Shaders", "fullscreen.v", "hlsl" },
    { rawResourcePath .. "Shaders", "fullscreen.p", "hlsl" },
    { rawResourcePath .. "Shaders", "imgui.v",    "hlsl" },
    { rawResourcePath .. "Shaders", "imgui.p",    "hlsl" },
}

compile(shaderFiles, RT_Shader)

-- TEXTURES --
rc.CurrentCategory = "texture"
local textureFiles = {
    { rawResourcePath .. "Textures", "grass", "png" },
}

compile(textureFiles, RT_Texture)

-- FONTS --
rc.CurrentCategory = "font"
local fontFiles = {
    { rawResourcePath .. "Fonts", "font", "ttf" },
    { rawResourcePath .. "Fonts", "segoeui", "ttf" },
    { rawResourcePath .. "Fonts", "segoeuib", "ttf" },
}

compile(fontFiles, RT_Font)

-- Write resource meta
rc.writeMeta()