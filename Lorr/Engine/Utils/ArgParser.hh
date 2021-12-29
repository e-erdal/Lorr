//
// Created on August 14th 2021 by e-erdal.
//

#pragma once

#include <cassert>

namespace lr
{
    class ArgParser
    {
    private:
        /* data */
        eastl::vector<eastl::string_view> m_vArgs;
        eastl::unordered_map<eastl::string_view, eastl::string> m_umConfig;

    public:
        ArgParser(size_t iArgCount, char *szArgVal[])
        {
            bool readConfig = false;
            eastl::string_view currentConfig{};

            // Very simple argument parsing logic
            for (size_t i = 0; i < iArgCount; i++)
            {
                auto &_v = szArgVal[i];

                if (_v[0] == '-' || _v[0] == '+') readConfig = false;

                if (readConfig) m_umConfig[currentConfig] += _v;

                if (_v[0] == '-') m_vArgs.push_back(_v + 1);

                if (_v[0] == '+')
                {
                    currentConfig = _v + 1;

                    m_umConfig.insert(eastl::make_pair(currentConfig, eastl::string{}));

                    readConfig = true;
                }
            }
        }

        // E.G -server = true
        bool HasArg(eastl::string_view svName)
        {
            auto it = std::find(m_vArgs.begin(), m_vArgs.end(), svName);
            if (it != m_vArgs.end()) return true;

            return false;
        }

        // E.G +port 27015 = true
        bool HasConfig(eastl::string_view svName)
        {
            if (m_umConfig.find(svName) != m_umConfig.end()) return true;

            return false;
        }

        // E.G +port 27015 = true && *t_pValue = 27015
        bool GetConfig(eastl::string_view svName, eastl::string *spTargetValue)
        {
            assert(spTargetValue != nullptr);

            if (!HasConfig(svName)) return false;

            *spTargetValue = m_umConfig[svName];

            return true;
        }
    };

}  // namespace lr