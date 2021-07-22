#include "Engine.hh"

#include "Layers/Main/MainLayer.hh"

class EditorApp : public Lorr::BaseApp
{
public:
    void Init() override
    {
        m_MainLayer->Init();
    }

    void Tick( float fDelta ) override
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

    Lorr::ApplicationDesc desc;
    desc.sTitle = "Lorr: Editor";
    desc.uWidth = 1280;
    desc.uHeight = 720;
    desc.eFlags |= Lorr::WindowFlags::Resizable | Lorr::WindowFlags::Centered;  // WindowFlags::Resizable | WindowFlags::Centered

    app = new EditorApp;
    app->Start( desc );
    app->Run();

    return 0;
}

Lorr::BaseApp *Lorr::GetApp()
{
    return app;
}