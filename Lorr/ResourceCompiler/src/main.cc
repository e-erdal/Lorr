#include "Engine/App/BaseApp.hh"
#include "Engine/Utils/ArgParser.hh"
#include "Engine/Stream/BufferStream.hh"

#include "Compilers/ShaderCompiler.hh"
#include "Compilers/TextureCompiler.hh"
#include "Compilers/AudioCompiler.hh"

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
    desc.ConsoleApp = true;

    app = new RCApp;
    app->Start(desc);

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
        else if (parser.HasArg("audio"))
        {
            std::string targetPath = "";
            if (parser.GetConfig("o", &targetPath))
            {
                AudioCompiler c(path, targetPath);
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
