#include "Chunk.hh"

namespace lr
{
    InputLayout ChunkDataPacked_P2x8_1x16::m_Layout = {
        { lr::VertexAttribType::UInt, "BLENDINDICES" },
        { lr::VertexAttribType::Float, "PSIZE" },
    };

    InputLayout ChunkDataDetailed::m_Layout = {
        { lr::VertexAttribType::Vec3, "POSITION" },
        { lr::VertexAttribType::Vec2, "TEXCOORD" },
    };
}