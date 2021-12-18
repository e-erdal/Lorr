//
// Created on August 14th 2021 by e-erdal.
//

#pragma once

#include <algorithm>
#include <cassert>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace lr
{
    class ArgParser
    {
    private:
        /* data */
        std::vector<std::string_view> m_vArgs;
        std::unordered_map<std::string_view, std::string> m_umConfig;

    public:
        ArgParser(size_t iArgCount, char *szArgVal[])
        {
            bool readConfig = false;
            std::string_view currentConfig{};

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

                    m_umConfig.insert(std::make_pair(currentConfig, std::string{}));

                    readConfig = true;
                }
            }
        }

        // E.G -server = true
        bool HasArg(std::string_view svName)
        {
            auto it = std::find(m_vArgs.begin(), m_vArgs.end(), svName);
            if (it != m_vArgs.end()) return true;

            return false;
        }

        // E.G +port 27015 = true
        bool HasConfig(std::string_view svName)
        {
            if (m_umConfig.find(svName) != m_umConfig.end()) return true;

            return false;
        }

        // E.G +port 27015 = true && *t_pValue = 27015
        bool GetConfig(std::string_view svName, std::string *spTargetValue)
        {
            assert(spTargetValue != nullptr);

            if (!HasConfig(svName)) return false;

            *spTargetValue = m_umConfig[svName];

            return true;
        }
    };

}  // namespace lr