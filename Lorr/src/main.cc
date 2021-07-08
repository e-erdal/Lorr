#include "Core/Engine/Engine.hh"

Engine *app = 0;

Engine *GetEngine()
{
    return app;
}

int main()
{
    EngineDesc desc;
    desc.uWidth = 1280;
    desc.uHeight = 800;

    app = new Engine( desc );
    app->Run();

    return 0;
}