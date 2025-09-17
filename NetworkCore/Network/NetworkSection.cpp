#include "pch.h"
#include "NetworkSection.h"

void NetworkSection::init(ServerBase* owner, int section_id)
{
    m_owner = owner;
    m_section_id = section_id;
    
    // FPS 측정 초기화
    m_last_frame_time = std::chrono::high_resolution_clock::now();
    m_frame_count = 0;
    m_current_fps = 0;
    
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

void NetworkSection::broadcast(std::shared_ptr<Packet> packet, Session* exception_session)
{
    for (auto& session : m_sessions)
    {
        if (session.second->get_id() == exception_session->get_id()) continue;
        session.second->do_send(packet);
    }
}

void NetworkSection::broadcast(class iProtocol& protocol)
{
    std::shared_ptr<Packet> packet = std::make_shared<Packet>();
    packet->reserve_packet_buffer(sizeof(protocol) + sizeof(PACKET_HEADER_SIZEOF));
    packet->initialize();
    protocol.Write(*packet.get());
    packet->finalize();

    broadcast(packet);
}

void NetworkSection::broadcast(class iProtocol& protocol, Session* exception_session)
{
    std::shared_ptr<Packet> packet = std::make_shared<Packet>();
    packet->reserve_packet_buffer(sizeof(protocol) + sizeof(PACKET_HEADER_SIZEOF));
    packet->initialize();
    protocol.Write(*packet.get());
    packet->finalize();

    broadcast(packet, exception_session);
}

void NetworkSection::section_thread_work()
{
    while(m_owner->is_running() == true)
    {
        if(m_task_queue.empty()) 
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;
        }
        
        iTask* task = nullptr;
        if(false == m_task_queue.try_pop(task)) continue;
        
        if(std::chrono::steady_clock::now() < task->execute_time)
        {
            m_task_queue.push(task);
            continue;
        }

        task->func();

        // 실제 작업을 처리했을 때만 FPS 카운트
        if (performance_check_mode)
            update_fps_info();

        if(task->is_repeat)
        {
            task->execute_time = std::chrono::steady_clock::now() + std::chrono::microseconds(task->delay_time);
            m_task_queue.push(task);
        }
        else
            xdelete task;
            
    }
}

void NetworkSection::update_fps_info()
{
    m_frame_count++;
    
    auto current_time = std::chrono::high_resolution_clock::now();
    auto delta_time = std::chrono::duration<double>(current_time - m_last_frame_time).count();
        
    if (delta_time >= 1.0) // 1초마다 FPS 업데이트
    {
        m_current_fps = m_frame_count / delta_time;
        m_frame_count = 0;
        m_last_frame_time = current_time;
    }
}
