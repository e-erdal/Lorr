//
// Created on Friday 10th September 2021 by e-erdal
//

#pragma once

#include <random>

namespace lr
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

        static inline u32 UInt(u32 rangeMin, u32 rangeMax)
        {
            std::uniform_int_distribution<u32> distribution(rangeMin, rangeMax);
            return distribution(m_Device);
        }

    private:
        static inline std::mt19937 m_Device;
    };

}  // namespace lr