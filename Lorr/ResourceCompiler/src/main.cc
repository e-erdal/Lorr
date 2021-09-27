#include "Engine/App/BaseApp.hh"
#include "Engine/Utils/ArgParser.hh"

#include "Compilers/ShaderCompiler.hh"
#include "Compilers/TextureCompiler.hh"
#include "Engine/Stream/BufferStream.hh"

class RCApp : public Lorr::BaseApp
{
public:
    void Init() override{};
    void Tick(float fDelta) override{};
    void Draw() override{};
};

RCApp *app;

namespace Lorr
{
    BaseApp *GetApp()
    {
        return app;
    }

}  // namespace Lorr

#define PERROR(...) printf("ERROR: " __VA_ARGS__);

int main(int argc, char **argv)
{
    Lorr::ApplicationDesc desc;
    desc.Title = "Lorr: Resource compiler";
    desc.Width = 10;
    desc.Height = 10;
    desc.Flags |= Lorr::WindowFlags::Resizable | Lorr::WindowFlags::Centered;

    app = new RCApp;
    app->Start(desc);

    Lorr::BufferStream buf;
    Lorr::GetEngine()->GetResourceMan()->ExportResource(Lorr::ResourceType::Audio, "theme.ogg", buf);

    Lorr::FileStream w("test.lr", true);
    w.WritePtr(buf.GetData(), buf.GetSize());
    w.Close();

    Lorr::AudioData data;
    Lorr::GetEngine()->GetResourceMan()->ImportAudioData("test.lr", data);

    return 1;

    Lorr::ArgParser parser(argc, argv);

    std::string path = "";
    if (parser.GetConfig("f", &path))
    {
        if (parser.HasArg("shader"))
        {
            std::string targetPath = "";
            if (parser.GetConfig("o", &targetPath))
            {
                ShaderCompiler c(path, targetPath);
            }
            else
            {
                PERROR("You have to specifiy the output path.");
            }
        }
        else if (parser.HasArg("texture"))
        {
            std::string targetPath = "";
            if (parser.GetConfig("o", &targetPath))
            {
                TextureCompiler c(path, targetPath);
            }
            else
            {
                PERROR("You have to specifiy the output path.");
            }
        }
        else
        {
            PERROR("You have to specifiy the resource type.");
        }
    }
    else
    {
        PERROR("You have to specifiy the resource path.");
    }

    return 0;
}
