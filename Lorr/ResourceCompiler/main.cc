#include "Engine/App/BaseApp.hh"
#include "Engine/App/Engine.hh"
#include "Engine/Core/FileSystem.hh"
#include "Engine/Utils/BufferStream.hh"
#include "Engine/Scripting/Lua.hh"

#include "Engine/Resource/ResourceMeta.hh"

class RCApp : public lr::BaseApp
{
public:
    void Init() override{};
    void Tick(float fDelta) override{};
    void Draw() override{};
};

RCApp *app;

lr::BaseApp *lr::GetApp()
{
    return app;
}

const char *CurrentCategory = "";
const char *CurrentOutputPath = "";
lr::ResourceMeta ResourceMetaData;

void compileResource(const char *pPath, const char *pName, const char *pExt, int type)
{
    LOG_WARN("Compiling %s/%s.%s to %s/%s.lr.", pPath, pName, pExt, CurrentOutputPath, pName);

    lr::BufferStream buf;
    eastl::string fullPath = lr::Format("%s/%s.%s", pPath, pName, pExt);
    eastl::string fullOutputPath = lr::Format("%s/%s.lr", CurrentOutputPath, pName);
    if (lr::GetEngine()->GetResourceMan()->ExportResource((lr::ResourceType)type, fullPath, buf))
    {
        lr::FileSystem::WriteBinaryFile(fullOutputPath, buf);

        ResourceMetaData.PushNewCategory(CurrentCategory, pName, fullOutputPath);
    }
}

void writeMeta()
{
    LOG_TRACE("Output .resmeta tree:");
    for (auto &metaPair : ResourceMetaData.m_CategoryMap)
    {
        LOG_TRACE("%s", metaPair.first.c_str());

        for (auto &name : metaPair.second)
        {
            LOG_TRACE("\t .%s = %s", name.first.c_str(), name.second.c_str());
        }
    }

    ResourceMetaData.SerializeAndWrite(".resmeta");
}

int main()
{
    lr::ApplicationDesc desc;
    desc.Title = "Lorr: Resource compiler";
    desc.ConsoleApp = true;

    app = new RCApp;
    app->Start(desc);

    lr::Lua::Init();
    luabridge::getGlobalNamespace(lr::Lua::L)
        .beginNamespace("rc")
        .addProperty("CurrentOutputPath", &CurrentOutputPath)
        .addProperty("CurrentCategory", &CurrentCategory)
        .addFunction("compileResource", &compileResource)
        .addFunction("writeMeta", &writeMeta)
        .endNamespace();

    lr::FileStream scriptFile("rc_main.lua", false);
    if (!scriptFile.IsOK())
    {
        LOG_ERROR("Failed to load 'rc_main.lua'.");
        return 0;
    }

    const char *pScript = scriptFile.ReadAll<char>();
    u32 size = scriptFile.Size();

    scriptFile.Close();

    lr::Lua::Execute(eastl::string(pScript, size));

    return 0;
}
