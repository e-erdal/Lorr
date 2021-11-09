#include "Worker.hh"

namespace Lorr::Job
{
    Worker::Worker()
    {
        m_Running = true;
        m_pThread = new std::thread(&Worker::Run, this);

        Queue *queue = new Queue(256);
        m_Queues.push_back(queue);
    }

    void Worker::Do(JobFn job)
    {
        m_Queues.back()->Push(job);
    }

    void Worker::Run()
    {
        while (!m_Running)
        {
            bool noWorkToDo = true;

            LOG_TRACE("Thread message");
        }
    }

}  // namespace Lorr::Job