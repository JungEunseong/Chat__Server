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
    NetworkSection() = default;
    virtual ~NetworkSection() = default;
public:
    void init(int section_id);
    
public:
    static unsigned int generate_section_id();

    unsigned int get_id() const { return m_section_id; }
    HANDLE get_iocp_handle();
    NetworkCore* get_network_core() { return m_owner; }

public:
    ClientSession* find_session(unsigned int session_id);
    
public:
    virtual void enter_section(class ClientSession* session);
    virtual void exit_section(int session_id);
    void push_task(iTask* task);

    void broadcast(std::shared_ptr<Packet> packet);
    void broadcast(class iProtocol protocol);
    
private:
    void section_thread_work();

private:
    unsigned int m_section_id;

    class ServerBase* m_owner; 
    std::thread m_section_thread;
    std::map<unsigned int, class ClientSession*> m_sessions;
    
    Concurrency::concurrent_priority_queue<iTask*, task_cmp> m_task_queue;
};