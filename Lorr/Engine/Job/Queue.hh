//
// Created on Sunday 7th November 2021 by e-erdal
//

#pragma once

namespace lr::Job  // Special module for Lorr
{
    using JobFn = std::function<void()>;

    class Queue
    {
    public:
        Queue(size_t maxJobs);

        void Push(JobFn job);
        void Clear();
        void Execute();

        JobFn Front();
        JobFn Back();

    private:
        std::atomic_size_t m_JobSize;
        std::atomic_size_t m_CompletedJobs;

        eastl::vector<JobFn> m_Jobs;
        size_t m_MaxJobsSize;
    };

}  // namespace lr::Job