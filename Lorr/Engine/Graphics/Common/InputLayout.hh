//
// Created on Thursday 14th October 2021 by e-erdal
//

#pragma once

namespace lr
{
    enum class VertexAttribType
    {
        None = 0,
        Float,
        Vec2,
        Vec3,
        Vec4,
        Mat4,
        UInt,
    };

    static u32 VertexAttribSize(VertexAttribType type)
    {
        switch (type)
        {
            case VertexAttribType::Float: return sizeof(float);
            case VertexAttribType::Vec2: return sizeof(glm::vec2);
            case VertexAttribType::Vec3: return sizeof(glm::vec3);
            case VertexAttribType::Vec4: return sizeof(glm::vec4);
            case VertexAttribType::Mat4: return sizeof(glm::mat4);
            case VertexAttribType::UInt: return sizeof(u32);
            default: return 0;
        }
    }

    struct VertexAttrib
    {
        VertexAttrib() = default;

        VertexAttrib(VertexAttribType type, const eastl::string &name, bool isNormalized = false)
            : m_Name(name), m_Type(type), m_Size(VertexAttribSize(type)), m_Offset(0), m_IsNormalized(isNormalized){};

        eastl::string m_Name;
        VertexAttribType m_Type;
        u32 m_Size;
        u32 m_Offset;
        bool m_IsNormalized;
    };

    class InputLayout
    {
    public:
        InputLayout() = default;
        InputLayout(std::initializer_list<VertexAttrib> elements);

    public:
        u32 GetStride() const
        {
            return m_Stride;
        }

        const eastl::vector<VertexAttrib> &GetElements() const
        {
            return m_Elements;
        }

        eastl::vector<VertexAttrib>::iterator begin()
        {
            return m_Elements.begin();
        }

        eastl::vector<VertexAttrib>::iterator end()
        {
            return m_Elements.end();
        }

        eastl::vector<VertexAttrib>::const_iterator begin() const
        {
            return m_Elements.begin();
        }
        
        eastl::vector<VertexAttrib>::const_iterator end() const
        {
            return m_Elements.end();
        }

    private:
        eastl::vector<VertexAttrib> m_Elements;
        u32 m_Stride = 0;
    };

}  // namespace lr