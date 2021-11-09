#include "Engine/App/Engine.hh"

#include "Layers/Main/MainLayer.hh"

using namespace Lorr;

class EditorApp : public BaseApp
{
public:
    void Init() override
    {
        m_MainLayer->Init();
    }

    void Tick(float fDelta) override
    {
    }

    void Draw() override
    {
        m_MainLayer->Update();
    }

private:
    MainLayer *m_MainLayer = new MainLayer;
};

static EditorApp *app = nullptr;

int main()
{
    ZoneScoped;

    ApplicationDesc desc;
    desc.Title = "Lorr: Editor";
    desc.Width = 1280;
    desc.Height = 720;
    desc.Flags |= Lorr::WindowFlags::Resizable | Lorr::WindowFlags::Centered;  // WindowFlags::Resizable | WindowFlags::Centered

    app = new EditorApp;
    app->Start(desc);
    app->Run();

    return 0;
}

Lorr::BaseApp *Lorr::GetApp()
{
    return app;
}