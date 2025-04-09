#pragma once

struct task_cmp
{
    bool operator()(const iTask* a, const iTask* b) const
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
    void init(int section_id);
    
public:
    static unsigned int generate_section_id();

    unsigned int get_id() const { return m_section_id; }
public:
    void enter_section(Session* session);
    void exit_section(int session_id);
    void push_task(iTask* task);
    
private:
    void section_thread_work();

private:
    unsigned int m_section_id;

    class ServerBase* m_owner; 
    std::thread m_section_thread;
    std::map<unsigned int, Session*> m_sessions;
    
    Concurrency::concurrent_priority_queue<iTask*, task_cmp> m_task_queue;
};