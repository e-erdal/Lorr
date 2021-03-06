//
// Created on Friday 10th September 2021 by e-erdal
//

#pragma once

#include <signals.h>

namespace lr
{
    class Counter
    {
    public:
        Counter(u32 counterID) : m_ID(counterID), m_Finished(true){};
        Counter(u32 counterID, u32 timeMS, bool looping) : m_ID(counterID), m_Target(timeMS), m_Looping(looping){};

        inline void Reset(u32 timeMS, bool looping)
        {
            m_Total = 0;
            m_TotalInSeconds = 0;
            m_Target = timeMS;
            m_Looping = looping;
            m_Finished = false;
            m_Seconds = 0;

            OnReset(m_ID, timeMS / 1000);
        }

        inline void Tick(float delta)
        {
            if (m_Finished) return;

            m_Total += delta;

            if (m_Total >= (float)m_Target / 1000.f)
            {
                // If we want it to tick continuously
                if (m_Looping)
                    m_Total = 0;
                else
                    m_Finished = true;

                OnCounterFinished(m_ID);
            }

            if (m_TotalInSeconds >= 1)
            {
                m_TotalInSeconds = 0;
                m_Seconds++;

                OnTick(m_ID, m_Seconds);
            }
            else
            {
                m_TotalInSeconds += delta;
            }
        }

        inline void Stop()
        {
            m_Total = 0;
            m_Finished = true;
        }

        inline u32 GetSeconds()
        {
            return m_Seconds;
        }

        inline u32 GetTargetSeconds()
        {
            return m_Target / 1000;
        }

        // Tick on each second
        sig::signal<void(u32, u32)> OnTick;
        sig::signal<void(u32)> OnCounterFinished;
        sig::signal<void(u32, u32)> OnReset;

    private:
        u32 m_ID = 0;
        float m_Total = 0;
        u32 m_Target = 0;
        u32 m_Seconds = 0;
        float m_TotalInSeconds = 0;
        bool m_Looping = false;
        bool m_Finished = false;
    };

}  // namespace lr