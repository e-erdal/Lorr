//
// Created on Friday 24th September 2021 by e-erdal
//

#pragma once

#if BX_PLATFORM_WINDOWS
    #include <Windows.h>
#elif BX_PLATFORM_LINUX || BX_PLATFORM_ANDROID
    #include <sys/stat.h>
    #include <dirent.h>
    #include <fcntl.h>
    #include <unistd.h>
    #include <cstdlib>
#endif

#if defined(CreateDirectory)
    #undef CreateDirectory
#endif

#include "Engine/Stream/BufferStream.hh"
#include "Engine/Stream/FileStream.hh"

namespace lr::FileSystem
{
    constexpr std::string_view GetFileName(std::string_view svPath) noexcept
    {
        size_t a = svPath.find_last_of('/');
        size_t b = svPath.find_last_of('\\');

        size_t c = a + b + 2;

        if (c > 0) return std::string_view(svPath.data() + c);

        return svPath;  // Not found...
    }

    constexpr bool HasExtension(std::string_view svPath, std::string_view svExt) noexcept
    {
        if (svPath.length() < svExt.length()) return false;

        // Check if the last characters match svExt
        return (0 == svPath.compare(svPath.length() - svExt.length(), svExt.length(), svExt));
    }

    inline bool CreateDirectory(std::string_view svPath)
    {
#if BX_PLATFORM_WINDOWS
        return CreateDirectoryA(svPath.data(), NULL);
#elif BX_PLATFORM_LINUX || BX_PLATFORM_ANDROID
        if (mkdir(svPath.data(), umask(0755)))
        {
            chmod(svPath.data(),
                  umask(0755));  // set the correct permissions
                                 // cause it's wrong for some reason
            return true;
        }
#else
    #error "Platform not implemented!"
#endif
        return false;
    }

    inline std::string JoinPath(std::string_view svPath1, std::string_view svPath2)
    {
        // Make sure our paths are valid
        if (svPath1.length() <= 0) return svPath2.data();
        if (svPath2.length() <= 0) return svPath1.data();

        if (svPath1[svPath1.length() - 1] == '/' || svPath1[svPath1.length() - 1] == '\\') return std::string(svPath1.data()) + svPath2.data();

        return std::string(svPath1) + "/" + svPath2.data();
    }

    inline bool Exists(std::string_view svPath)
    {
#if BX_PLATFORM_WINDOWS
        const auto dir_type = GetFileAttributesA(svPath.data());

        return dir_type != INVALID_FILE_ATTRIBUTES;
#elif BX_PLATFORM_LINUX || BX_PLATFORM_ANDROID
        return (access(svPath.data(), F_OK) != -1);
#else
    #error "Platform not implemented!"
#endif
        return false;
    }

    inline bool IsDirectory(std::string_view svPath)
    {
        if (!Exists(svPath)) return false;

#if BX_PLATFORM_WINDOWS
        return GetFileAttributes(svPath.data()) & FILE_ATTRIBUTE_DIRECTORY;
#elif BX_PLATFORM_LINUX || BX_PLATFORM_ANDROID
        struct stat st
        {
        };
        stat(svPath.data(), &st);
        return S_ISDIR(st.st_mode);
#else
    #error "Platform not implemented!"
#endif
    }

    inline std::vector<std::string> ReadDirectory(std::string_view svPath, bool bRecursive = false)
    {
        std::vector<std::string> directories;

#if BX_PLATFORM_WINDOWS
        HANDLE hFind;
        WIN32_FIND_DATA FindFileData;

        // Requires a * wildcard for some reason
        if ((hFind = FindFirstFile(FileSystem::JoinPath(svPath, "./*").data(), &FindFileData)) != INVALID_HANDLE_VALUE)
        {
            do
            {
                if (std::string_view(FindFileData.cFileName) == "." || std::string_view(FindFileData.cFileName) == "..") continue;

                directories.push_back(FileSystem::JoinPath(svPath, FindFileData.cFileName));
            } while (FindNextFile(hFind, &FindFileData));
            FindClose(hFind);
        }
#elif BX_PLATFORM_LINUX || BX_PLATFORM_ANDROID
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir(svPath.data())) != nullptr)
        {
            while ((ent = readdir(dir)) != nullptr)
            {
                if (std::string_view(ent->d_name) == "." || std::string_view(ent->d_name) == "..") continue;

                directories.push_back(FileSystem::JoinPath(svPath, ent->d_name));
            }
            closedir(dir);
        }
#else
    #error "Platform not implemented!"
#endif

        if (bRecursive && !directories.empty())
        {
            for (const auto &_dir : directories)
            {
                if (FileSystem::IsDirectory(_dir))
                {
                    for (const auto &__dir : ReadDirectory(_dir, true))
                    {
                        directories.push_back(__dir);
                    }
                }
            }
        }

        return directories;
    }

    inline std::string ResolveFullPath(std::string_view svPath)
    {
#if BX_PLATFORM_WINDOWS
        char buffer[4096];
        if (IS_ERROR(GetFullPathNameA(svPath.data(), 4096, buffer, nullptr)))
        {
            LOG_ERROR("Failed to resolve full path of {}", svPath.data());
            return "";
        }

        return buffer;
#elif BX_PLATFORM_LINUX || BX_PLATFORM_ANDROID
        char buffer[PATH_MAX + 1];

        if (realpath(svPath.data(), buffer) == nullptr)
        {
            LOG_ERROR("Failed to resolve full path of {}", svPath.data());
            return "";
        }

        return std::string(buffer);
#else
    #error "Platform not implemented!"
#endif
    }

    inline bool ReadBinaryFile(std::string_view path, BufferStream &buf)
    {
        FileStream f(path.data(), false);
        if (!f.IsOK())
        {
            return false;
        }

        u8 *fileData = f.ReadAll<u8>();
        size_t fileLen = f.Size();
        f.Close();

        buf.Reset(fileData, fileLen);

        free(fileData);

        return true;
    }

    inline void WriteBinaryFile(std::string_view path, BufferStream &buf)
    {
        FileStream w(path.data(), true);
        w.WritePtr(buf.GetData(), buf.GetSize());
        w.Close();
    }
}  // namespace lr::FileSystem