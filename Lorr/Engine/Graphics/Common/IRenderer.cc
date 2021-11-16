#include "IRenderer.hh"

namespace Lorr
{
    RendererType IRenderer::m_CurrentAPI = RendererType::None;

    void IRenderer::InitParent()
    {
        m_PostProcessMan.Init();
    }

    void IRenderer::PollPostProcess()
    {
        m_PostProcessMan.Draw();
    }

}  // namespace Lorr