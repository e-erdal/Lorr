//
// Created on Sunday 7th November 2021 by e-erdal
//

#pragma once

#include "Queue.hh"

namespace lr::Job
{
    class Worker
    {
    public:
        Worker();

        void Do(JobFn job);

        void Run();

    private:
        eastl::vector<Queue *> m_Queues;

        std::thread *m_pThread = 0;
        std::atomic_flag m_Lock = ATOMIC_FLAG_INIT;
        bool m_Running = false;

    };

}  // namespace lr::Job