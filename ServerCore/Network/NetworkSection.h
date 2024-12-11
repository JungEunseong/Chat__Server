#pragma once

struct task_cmp
{
    bool operator()(const std::shared_ptr<iTask> a, const std::shared_ptr<iTask> b) const
    {
        return a->execute_time > b->execute_time;
    }
};

class NetworkSection
{
public:
    NetworkSection();
    ~NetworkSection();
public:
    void section_thread_work();
    
private:

    std::shared_ptr<class ServerBase> m_owner; 
    std::thread m_section_thread;
    std::map<int, std::shared_ptr<Session>> m_sessions;
    
    Concurrency::concurrent_priority_queue<std::shared_ptr<iTask>, task_cmp> m_task_queue;
};