//
// Created on Friday 10th September 2021 by e-erdal
//

#pragma once

#include <random>

namespace Lorr
{
    class Random
    {
    public:
        Random() = default;

        static inline void Seed()
        {
            m_Device.seed(time_t(0));
        }

        static inline int Int(int rangeMin, int rangeMax)
        {
            std::uniform_int_distribution<int> distribution(rangeMin, rangeMax);
            return distribution(m_Device);
        }

        static inline uint32_t UInt(uint32_t rangeMin, uint32_t rangeMax)
        {
            std::uniform_int_distribution<uint32_t> distribution(rangeMin, rangeMax);
            return distribution(m_Device);
        }

    private:
        static inline std::mt19937 m_Device;
    };

}  // namespace Lorr