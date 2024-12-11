#include "pch.h"
#include "NetworkSection.h"

NetworkSection::NetworkSection()
{
}

NetworkSection::~NetworkSection()
{
}

void NetworkSection::section_thread_work()
{
    while(m_owner->is_running() == true)
    {
        if(m_task_queue.empty()) continue;
        
        std::shared_ptr<iTask> task = nullptr;
        if(false == m_task_queue.try_pop(task)) continue;
        
        if(std::chrono::steady_clock::now() < task->execute_time)
        {
            m_task_queue.push(task);
            continue;
        }
        
        task->func();

        if(task->is_repeat)
        {
            task->execute_time = std::chrono::steady_clock::now() + std::chrono::microseconds(task->delay_time);
            m_task_queue.push(task);
        } 
    }
}
