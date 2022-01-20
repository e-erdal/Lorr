#include "ResourceMeta.hh"

#include "Engine/Utils/FileStream.hh"

constexpr eastl::string_view kDescription = "# Auto-generated binary file by Resource Compiler, DO NOT CHANGE!\n"
                                            "# This file contains all info about resources.\n\n";

namespace lr
{
    void ResourceMeta::PushNewCategory(const eastl::string &category, const eastl::string &resourceName, const eastl::string &path)
    {
        m_CategoryMap[category].push_back(eastl::make_pair(resourceName, path));
    }

    void ResourceMeta::SerializeAndWrite(const eastl::string &path)
    {
        FileStream fs(path, true);
        fs.WriteString(Format(kDescription.data(), m_GenerationTime, m_GenerationVersion));
        fs.Write(m_GenerationVersion);
        fs.Write(m_GenerationTime);
        fs.Write(m_Flags);
        fs.Write<u32>(m_CategoryMap.size());

        for (auto &pair : m_CategoryMap)
        {
            fs.Write<u8>(pair.first.length());
            fs.WriteString(pair.first);
            fs.Write<u32>(pair.second.size());

            for (auto &resources : pair.second)
            {
                fs.Write<u8>(resources.first.length());
                fs.WriteString(resources.first);
                fs.Write<u16>(resources.second.length());
                fs.WriteString(resources.second);
            }
        }

        fs.Close();
    }

    bool ResourceMeta::Read(const eastl::string &path)
    {
        FileStream fs(path, false);

        if (!fs.IsOK())
        {
            LOG_ERROR(".resmeta file doesn't exist.");
            return false;
        }

        BufferStream buf;
        buf = fs;
        fs.Close();
        buf.Seek(SEEK_CUR, kDescription.length());

        m_GenerationVersion = buf.Get<u16>();
        m_GenerationTime = buf.Get<u32>();
        m_Flags = buf.Get<u32>();
        u32 categorySize = buf.Get<u32>();

        for (u32 i = 0; i < categorySize; i++)
        {
            eastl::string categoryName = buf.GetString<u8>();
            auto &resourceVector = m_CategoryMap[categoryName];
            u32 resourceCount = buf.Get<u32>();

            for (u32 j = 0; j < resourceCount; j++)
            {
                eastl::string resourceName = buf.GetString<u8>();
                eastl::string resourcePath = buf.GetString<u16>();
                resourceVector.push_back(eastl::make_pair(resourceName, resourcePath));
            }
        }

        return true;
    }

    eastl::string ResourceMeta::GetResourcePath(const eastl::string &resMeta)
    {
        u32 categoryEndPos = resMeta.find_first_of(":");
        if (categoryEndPos != -1)
        {
            ResourceIdentifierData resIdent;

            eastl::string categoryName = resMeta.substr(0, categoryEndPos);
            if (GetCategory(categoryName, resIdent))
            {
                eastl::string resourceName = resMeta.substr(categoryEndPos + 1, resMeta.length() - categoryEndPos);
                for (auto &resourceInfo : resIdent)
                {
                    if (resourceInfo.first == resourceName) return resourceInfo.second;
                }
            }
        }

        return "";
    }

    bool ResourceMeta::GetCategory(const eastl::string &category, ResourceIdentifierData &resIdent)
    {
        auto categoryIt = m_CategoryMap.find(category);
        if (categoryIt != m_CategoryMap.end())
        {
            resIdent = categoryIt->second;
            return true;
        }

        return false;
    }

}  // namespace lr