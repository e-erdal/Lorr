//
// Created on August 19th 2021 by e-erdal.
//

#pragma once

#include "Utils/Identifier.hh"

namespace Lorr
{
    enum class VertexAttribType
    {
        None = 0,
        Float,
        Vec2,
        Vec3,
        Vec4,
        Mat4,
        Int,
        IVec2,
        IVec3,
        IVec4
    };

    static size_t VertexAttribTypeSize(VertexAttribType eAttrib)
    {
        switch (eAttrib)
        {
        case VertexAttribType::Float: return sizeof(float);
        case VertexAttribType::Vec2: return sizeof(glm::vec2);
        case VertexAttribType::Vec3: return sizeof(glm::vec3);
        case VertexAttribType::Vec4: return sizeof(glm::vec4);
        case VertexAttribType::Mat4: return sizeof(glm::mat4);
        case VertexAttribType::Int: return sizeof(int);
        case VertexAttribType::IVec2: return sizeof(glm::ivec2);
        case VertexAttribType::IVec3: return sizeof(glm::ivec3);
        case VertexAttribType::IVec4: return sizeof(glm::ivec4);
        default: return 0;
        }
    }

    struct VertexAttrib
    {
        std::string Name;
        VertexAttribType Type;
        uint32_t Size;
        uint32_t Offset;
        bool IsNormalized;

        VertexAttrib() = default;

        VertexAttrib(VertexAttribType eType, const std::string &sName, bool bIsNormalized = false):
            Name(sName), Type(eType), Size(VertexAttribTypeSize(eType)), Offset(0), IsNormalized(bIsNormalized)
        {
        }

        uint32_t GetComponentCount() const
        {
            switch (Type)
            {
            case VertexAttribType::Float: return 1;
            case VertexAttribType::Vec2: return 2;
            case VertexAttribType::Vec3: return 3;
            case VertexAttribType::Vec4: return 4;
            case VertexAttribType::Mat4: return 4;
            case VertexAttribType::Int: return 1;
            case VertexAttribType::IVec2: return 2;
            case VertexAttribType::IVec3: return 3;
            case VertexAttribType::IVec4: return 4;
            default: return 0;
            }

            return 0;
        }
    };

    class VertexLayout
    {
    public:
        VertexLayout()
        {
        }

        VertexLayout(std::initializer_list<VertexAttrib> elements): m_Elements(elements)
        {
            CalculateOffsetsAndStride();
        }

        uint32_t GetStride() const
        {
            return m_Stride;
        }
        const std::vector<VertexAttrib> &GetElements() const
        {
            return m_Elements;
        }

        std::vector<VertexAttrib>::iterator begin()
        {
            return m_Elements.begin();
        }
        std::vector<VertexAttrib>::iterator end()
        {
            return m_Elements.end();
        }
        std::vector<VertexAttrib>::const_iterator begin() const
        {
            return m_Elements.begin();
        }
        std::vector<VertexAttrib>::const_iterator end() const
        {
            return m_Elements.end();
        }

    private:
        void CalculateOffsetsAndStride()
        {
            size_t offset = 0;
            m_Stride = 0;
            for (auto &element : m_Elements)
            {
                element.Offset = offset;
                offset += element.Size;
                m_Stride += element.Size;
            }
        }

    private:
        std::vector<VertexAttrib> m_Elements;
        uint32_t m_Stride = 0;
    };

    enum ShaderType
    {
        VERTEX_SHADER,
        GEOMETRY_SHADER,
        HULL_SHADER,
        PIXEL_SHADER
    };

    struct SHADER_DESC
    {
        ShaderType Type;
        VertexLayout Layout;
        std::string Path = "";
    };

    struct SHADER_DESC_SUBRESC
    {
        uint8_t *Data = 0;
        size_t DataSize = 0;
    };

    class Shader
    {
    public:
        Shader() = default;

        virtual void Init(const Identifier &identifier, SHADER_DESC *pShaderD, SHADER_DESC_SUBRESC *pShaderSRES = 0) = 0;

        virtual void Use() = 0;
    };
}  // namespace Lorr