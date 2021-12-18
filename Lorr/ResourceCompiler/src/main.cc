#include "Engine/App/BaseApp.hh"
#include "Engine/Utils/ArgParser.hh"
#include "Engine/Stream/BufferStream.hh"

#include "Compilers/AudioCompiler.hh"
#include "Compilers/FontCompiler.hh"
#include "Compilers/ShaderCompiler.hh"
#include "Compilers/TextureCompiler.hh"
#include "Compilers/ModelCompiler.hh"

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

#define PERROR(...) printf("ERROR: " __VA_ARGS__);

int main(int argc, char **argv)
{
    lr::ApplicationDesc desc;
    desc.Title = "Lorr: Resource compiler";
    desc.ConsoleApp = true;

    app = new RCApp;
    app->Start(desc);

    lr::ArgParser parser(argc, argv);

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
        else if (parser.HasArg("font"))
        {
            std::string targetPath = "";
            if (parser.GetConfig("o", &targetPath))
            {
                FontCompiler c(path, targetPath);
            }
            else
            {
                PERROR("You have to specifiy the output path.");
            }
        }
        else if (parser.HasArg("model"))
        {
            std::string targetPath = "";
            if (parser.GetConfig("o", &targetPath))
            {
                ModelCompiler c(path, targetPath);
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
