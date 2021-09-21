#include "VertexBatcher.hh"

void VertexBatcher::Init()
{
}

void VertexBatcher::Begin(uint32_t preAllocRect)
{
    m_Active = true;

    AllocRectangle(preAllocRect);
}

void VertexBatcher::End()
{
    m_Active = false;
}

void VertexBatcher::AllocRectangle(uint32_t count)
{
    size_t sizeBefore = m_Indices.size();
    m_Indices.resize(sizeBefore + count * 6);

    for (int i = sizeBefore; i < sizeBefore + count; count++)
    {
        m_Indices[i + 0] = m_Indexes + 0;
        m_Indices[i + 1] = m_Indexes + 1;
        m_Indices[i + 2] = m_Indexes + 2;

        m_Indices[i + 3] = m_Indexes + 2;
        m_Indices[i + 4] = m_Indexes + 3;
        m_Indices[i + 5] = m_Indexes + 0;

        m_Indexes += 4;
    }

    m_Vertices.reserve(m_Vertices.capacity() + (count * 4) * sizeof(VBInput));
    m_Vertices.resize(m_Vertices.size() + count * 4);
}