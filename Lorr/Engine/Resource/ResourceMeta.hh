//
// Created on Friday 7th January 2022 by e-erdal
//

#pragma once

#include "ResourceFile.hh"

namespace lr
{   
    ///                                               RESOURCE NAME  RESOURCE REAL PATH
    using ResourceIdentifierData = eastl::vector<eastl::pair<eastl::string, eastl::string>>;
    struct ResourceMeta
    {
        u16 m_GenerationVersion = kResourceMinVersion;
        u32 m_GenerationTime = 0;  // Unix Time
        u32 m_Flags = 0;

        ///                  CATEGORY NAME
        eastl::unordered_map<eastl::string, ResourceIdentifierData> m_CategoryMap;

        void PushNewCategory(const eastl::string &category, const eastl::string &resourceName, const eastl::string &path);
        void SerializeAndWrite(const eastl::string &path);
        bool Read(const eastl::string &path);

        eastl::string GetResourcePath(const eastl::string &resMeta);
        bool GetCategory(const eastl::string &category, ResourceIdentifierData &resIdent);
    };

}  // namespace lr