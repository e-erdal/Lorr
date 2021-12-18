#include "Engine/App/Engine.hh"

#include "Layers/Main/MainLayer.hh"

using namespace lr;

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
    desc.Flags |= WindowFlags::Resizable | WindowFlags::Centered;  // WindowFlags::Resizable | WindowFlags::Centered

    app = new EditorApp;
    app->Start(desc);
    app->Run();

    return 0;
}

lr::BaseApp *lr::GetApp()
{
    return app;
}