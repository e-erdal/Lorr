#include "GameApp/GameApp.hh"

static GameApp *app = nullptr;

int main()
{
    ZoneScoped;

    Lorr::ApplicationDesc desc;
    desc.Title = "Lorr: Game";
    desc.Width = 1280;
    desc.Height = 720;
    desc.Flags |= Lorr::WindowFlags::Resizable | Lorr::WindowFlags::Centered;

    app = new GameApp;
    app->Start(desc);
    app->Run();

    return 0;
}

Lorr::BaseApp *Lorr::GetApp()
{
    return app;
}