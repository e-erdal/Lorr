#include "BaseRenderer.hh"

namespace lr
{
    RendererType BaseRenderer::m_CurrentAPI = RendererType::None;

    void BaseRenderer::InitParent()
    {
        m_PostProcessMan.Init();
    }

    void BaseRenderer::PollPostProcess()
    {
        m_PostProcessMan.Draw();
    }

}  // namespace lr