#include "IPostProcess.hh"

#include "LensFlare/LensFlare.hh"

namespace lr
{
    void PostProcessManager::Init()
    {
        // Register<PostProcess::LensFlare>();
    }

    void PostProcessManager::Draw()
    {
        for (auto pass : m_Passes) pass.second->Draw();
    }

}  // namespace lr