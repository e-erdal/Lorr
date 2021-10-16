#include "D3D11RenderBuffer.hh"

namespace Lorr
{
    static D3D11_CPU_ACCESS_FLAG ToD3D11AF(RenderBufferAccess flags)
    {
        int d11Flags = 0;
        if (flags & RB_ACCESS_TYPE_CPUR) d11Flags |= D3D11_CPU_ACCESS_READ;
        if (flags & RB_ACCESS_TYPE_CPUW) d11Flags |= D3D11_CPU_ACCESS_WRITE;

        return (D3D11_CPU_ACCESS_FLAG)d11Flags;
    }

    void D3D11RenderBuffer::Init(void *pData, size_t dataLen, RenderBufferType type, RenderBufferUsage usage,
                                 RenderBufferAccess accessFlags)
    {
        auto *pDevice = D3D11Renderer::Get()->GetDevice();
        D3D11_BUFFER_DESC desc = {};
        desc.ByteWidth = dataLen;
        desc.CPUAccessFlags = ToD3D11AF(accessFlags);

        switch (usage)
        {
            case RenderBufferUsage::Default: desc.Usage = D3D11_USAGE_DEFAULT; break;
            case RenderBufferUsage::Immutable:
                desc.Usage = D3D11_USAGE_IMMUTABLE;
                type = RenderBufferType::ShaderResource;
                break;
            case RenderBufferUsage::Dynamic: desc.Usage = D3D11_USAGE_DYNAMIC; break;
            case RenderBufferUsage::Staging: desc.Usage = D3D11_USAGE_STAGING; break;
        }

        switch (type)
        {
            case RenderBufferType::Vertex: desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; break;
            case RenderBufferType::Index: desc.BindFlags = D3D11_BIND_INDEX_BUFFER; break;
            case RenderBufferType::Constant: desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; break;
            case RenderBufferType::ShaderResource: desc.BindFlags = D3D11_BIND_SHADER_RESOURCE; break;
        }

        // Select the buffer mapping type
        if (usage == RenderBufferUsage::Dynamic && (accessFlags & RB_ACCESS_TYPE_CPUR && accessFlags & RB_ACCESS_TYPE_CPUW))
            m_Mapping = D3D11_MAP_READ_WRITE;
        else if (usage == RenderBufferUsage::Dynamic && accessFlags & RB_ACCESS_TYPE_CPUW)
            m_Mapping = D3D11_MAP_WRITE_DISCARD;
        else if (accessFlags & RB_ACCESS_TYPE_CPUR)
            m_Mapping = D3D11_MAP_READ;
        else if (accessFlags & RB_ACCESS_TYPE_CPUW)
            m_Mapping = D3D11_MAP_WRITE;

        // TODO: D3D11_MAP_WRITE_NO_OVERWRITE Cannot be used on a resource created with the D3D11_BIND_CONSTANT_BUFFER flag.

        if (pData)
        {
            D3D11_SUBRESOURCE_DATA data = {};
            data.pSysMem = pData;

            pDevice->CreateBuffer(&desc, &data, &m_pHandle);
        }
        else
        {
            pDevice->CreateBuffer(&desc, 0, &m_pHandle);
        }

        m_Type = type;
    }

    void D3D11RenderBuffer::SetData(void *pData, size_t dataLen)
    {
        auto *pCtx = D3D11Renderer::Get()->GetDeviceContext();

        D3D11_MAPPED_SUBRESOURCE ms = {};
        if (SUCCEEDED(pCtx->Map(m_pHandle, 0, m_Mapping, 0, &ms)))
        {
            memcpy(ms.pData, pData, dataLen);

            pCtx->Unmap(m_pHandle, 0);
        }
    }

    void D3D11RenderBuffer::Use(uint32_t offset, InputLayout *pLayout)
    {
        auto *pCtx = D3D11Renderer::Get()->GetDeviceContext();

        switch (m_Type)
        {
            case RenderBufferType::Vertex:
            {
                auto stride = pLayout->GetStride();
                pCtx->IASetVertexBuffers(0, 1, &m_pHandle, &stride, &offset);
                break;
            }
            case RenderBufferType::Index: pCtx->IASetIndexBuffer(m_pHandle, DXGI_FORMAT_R32_UINT, offset); break;
            case RenderBufferType::Constant:
                pCtx->VSSetConstantBuffers(0, 1, &m_pHandle);
                pCtx->PSSetConstantBuffers(0, 1, &m_pHandle);
                break;
            default: break;
        }
    }

}  // namespace Lorr