#include "InputLayout.hh"

namespace Lorr
{
    InputLayout::InputLayout(std::initializer_list<VertexAttrib> elements) : m_Elements(elements)
    {
        size_t offset = 0;
        m_Stride = 0;
        for (auto &element : m_Elements)
        {
            element.m_Offset = offset;
            offset += element.m_Size;
            m_Stride += element.m_Size;
        }
    }

}  // namespace Lorr