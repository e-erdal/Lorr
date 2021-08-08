#include "GameApp/GameApp.hh"

static GameApp *app = nullptr;

int main()
{
    ZoneScoped;

    Lorr::ApplicationDesc desc;
    desc.sTitle = "Lorr: Game";
    desc.uWidth = 1280;
    desc.uHeight = 720;
    desc.eFlags |= Lorr::WindowFlags::Resizable | Lorr::WindowFlags::Centered;

    app = new GameApp;
    app->Start( desc );
    app->Run();

    return 0;
}

Lorr::BaseApp *Lorr::GetApp()
{
    return app;
}