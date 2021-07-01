#include "VertexBatcher.hh"

static uint32_t g_uMaxQuads = 80000;

VertexBatcher::VertexBatcher()
{
}

VertexBatcher::~VertexBatcher()
{
}

void VertexBatcher::Init()
{
    glGenVertexArrays( 1, &m_VAO );
    glBindVertexArray( m_VAO );

    glGenBuffers( 1, &m_VBO );
    glBindBuffer( GL_ARRAY_BUFFER, m_VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( VertexInfo ) * g_uMaxQuads, 0, GL_DYNAMIC_DRAW );

    glEnableVertexArrayAttrib( m_VBO, 0 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( VertexInfo ), (const void *) offsetof( VertexInfo, Pos ) );

    glEnableVertexArrayAttrib( m_VBO, 1 );
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof( VertexInfo ), (const void *) offsetof( VertexInfo, UV ) );

    glEnableVertexArrayAttrib( m_VBO, 2 );
    glVertexAttribPointer( 2, 4, GL_FLOAT, GL_FALSE, sizeof( VertexInfo ), (const void *) offsetof( VertexInfo, Color ) );

    glBindVertexArray( 0 );

    // We pre-alloc our index buffer to save some time in rendering loop
    uint32_t *quads = new uint32_t[g_uMaxQuads * 6];
    uint32_t offset = 0;

    for ( uint32_t i = 0; i < ( g_uMaxQuads * 6 ); i += 6 )
    {
        quads[i + 0] = offset + 0;
        quads[i + 1] = offset + 1;
        quads[i + 2] = offset + 2;

        quads[i + 3] = offset + 2;
        quads[i + 4] = offset + 3;
        quads[i + 5] = offset + 0;

        offset += 4;
    }

    glGenBuffers( 1, &m_IBO );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_IBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, g_uMaxQuads * 6 * sizeof( uint32_t ), quads, GL_STATIC_DRAW );

    delete[] quads;

    // Initialize default textures here
}

void VertexBatcher::BeginFrame()
{
}

void VertexBatcher::EndFrame()
{
    Reset();
}

void VertexBatcher::Flush()
{
    for ( auto &&event : m_vBatchEvents )
    {
        auto &vertexes = event.vertices;
        auto &indexes = event.indexes;

        if ( vertexes.size() > 0 )
        {
            glBindBuffer( GL_ARRAY_BUFFER, m_VBO );
            glBufferSubData( GL_ARRAY_BUFFER, 0, vertexes.size() * sizeof( VertexInfo ), &vertexes[0] );

            glBindVertexArray( m_VAO );
            glDrawElements( GL_TRIANGLES, vertexes.size(), GL_UNSIGNED_INT, 0 );
        }
    }
}

void VertexBatcher::Reset()
{
    Flush();

    // clear the map
    m_vBatchEvents.clear();
}

BatchEvent &VertexBatcher::GetVertexData()
{
    if ( m_vBatchEvents.size() > 0 ) return m_vBatchEvents.back();

    m_vBatchEvents.push_back( BatchEvent{} );
    m_vBatchEvents.back().vertices.reserve( 256 );  // Preallocate 256 vertices

    return m_vBatchEvents.back();
}