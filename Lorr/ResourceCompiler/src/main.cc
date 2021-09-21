#include "Engine/App/BaseApp.hh"
#include "Engine/Utils/ArgParser.hh"

#include "Compilers/ShaderCompiler.hh"

namespace Lorr
{
    BaseApp *GetApp()
    {
        return 0;
    }

}  // namespace Lorr

#define PERROR(...) printf("ERROR: " __VA_ARGS__);

int main(int argc, char **argv)
{
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
