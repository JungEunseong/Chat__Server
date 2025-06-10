#include "pch.h"
#include "NetworkSection.h"

void NetworkSection::init(ServerBase* owner, int section_id)
{
    m_owner = owner;
    m_section_thread= std::thread([this](){ section_thread_work(); });
}

unsigned int NetworkSection::generate_section_id()
{
    static unsigned int id_generator = 0;
    return ++id_generator;
}

HANDLE NetworkSection::get_iocp_handle()
{
    return m_owner->get_iocp_handle();
}

ClientSession* NetworkSection::find_session(unsigned int session_id)
{
    return m_sessions.at(session_id);
}

void NetworkSection::enter_section(ClientSession* session)
{
    if(m_sessions.count(session->get_id()) != 0)
    {
        session->do_disconnect();
        return;
    }
    
    m_sessions.emplace(session->get_id(), session);
   session->set_section(this);
}

void NetworkSection::exit_section(int session_id)
{
    if(m_sessions.count(session_id) == 0)
        return;
    
    ClientSession* session = m_sessions.at(session_id);
    session->set_section(nullptr);
    m_sessions.erase(session_id);
}

void NetworkSection::push_task(iTask* task)
{
    task->execute_time = std::chrono::steady_clock::now() + std::chrono::milliseconds(task->delay_time);
    m_task_queue.push(task);
}

void NetworkSection::broadcast(std::shared_ptr<Packet> packet)
{
    for (auto& session : m_sessions)
        session.second->do_send(packet);
}

void NetworkSection::broadcast(class iProtocol protocol)
{
    std::shared_ptr<Packet> packet = std::make_shared<Packet>();
    protocol.Write(*packet.get());
    packet->finalize();

    broadcast(packet);
}

void NetworkSection::section_thread_work()
{
    while(m_owner->is_running() == true)
    {
        if(m_task_queue.empty()) continue;
        
        iTask* task = nullptr;
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
