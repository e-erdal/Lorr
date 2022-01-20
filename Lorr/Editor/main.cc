#include "Engine/App/Engine.hh"
#include "D3D11/D3D11Texture.hh"

#include "App/EditorApp.hh"
static EditorApp *app = nullptr;

int main(int argc, char **argv)
{
    ZoneScoped;

    loguru::init(argc, argv);

    lr::ApplicationDesc desc;
    desc.Title = "Lorr: Editor";
    desc.Width = 1280;
    desc.Height = 720;
    desc.Flags |= lr::WindowFlags::Resizable | lr::WindowFlags::Centered;  // WindowFlags::Resizable | WindowFlags::Centered

    app = new EditorApp;
    app->Start(desc);
    app->Run();

    return 0;
}

lr::BaseApp *lr::GetApp()
{
    return app;
}