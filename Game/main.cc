#include "GameApp/GameApp.hh"

static GameApp *app = nullptr;

int main()
{
    ZoneScoped;

    lr::ApplicationDesc desc;
    desc.Title = "GameApp";
    desc.Width = 1280;
    desc.Height = 720;
    desc.Flags |= lr::WindowFlags::Resizable | lr::WindowFlags::Centered;

    app = new GameApp;
    app->Start(desc);
    app->Run();

    return 0;
}

lr::BaseApp *lr::GetApp()
{
    return app;
}