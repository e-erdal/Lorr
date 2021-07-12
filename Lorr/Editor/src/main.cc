#include "Engine.hh"

class EditorApp : public Lorr::BaseApp
{
public:
    void Init() override
    {
    }

    void Tick( float fDelta ) override
    {
    }

    void Draw() override
    {
    }
};

static EditorApp *app = nullptr;

int main()
{
    Lorr::ApplicationDesc desc;
    desc.sTitle = "Lorr: Editor";
    desc.uWidth = 1280;
    desc.uHeight = 720;

    app = new EditorApp;
    app->Start( desc );
    app->Run();

    return 0;
}

Lorr::BaseApp *Lorr::GetApp()
{
    return app;
}