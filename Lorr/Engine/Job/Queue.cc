#include "Queue.hh"

namespace lr::Job
{
    Queue::Queue(size_t maxJobs) : m_MaxJobsSize(maxJobs)
    {
        m_Jobs.resize(maxJobs);

        m_JobSize = 0;
        m_CompletedJobs = 0;
    }

    void Queue::Push(JobFn job)
    {
        size_t jobSize = m_JobSize.load(std::memory_order_seq_cst);
        m_JobSize.store(jobSize + 1, std::memory_order_seq_cst);

        m_Jobs[jobSize] = job;
    }

    void Queue::Clear()
    {
        m_CompletedJobs = 0;
        m_JobSize = 0;
    }

    void Queue::Execute()
    {
        size_t firstJob = m_CompletedJobs.load(std::memory_order_seq_cst);
        m_CompletedJobs.store(firstJob + 1, std::memory_order_seq_cst);

        m_Jobs[firstJob]();
    }

    JobFn Queue::Front()
    {
        return m_Jobs[m_CompletedJobs];
    }

    JobFn Queue::Back()
    {
        return m_Jobs[m_JobSize];
    }

}  // namespace lr::Job