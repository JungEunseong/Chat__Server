#include "pch.h"
#include "ServerBase.h"

#include <memory>
#include <memory>

void ServerBase::init(int iocp_thread_count, int section_count)
{
    NetworkCore::init(iocp_thread_count);
    
    for(int i = 0; i < section_count; ++i)
    {
        std::shared_ptr<NetworkSection> section = std::make_shared<NetworkSection>();
        int section_id = NetworkSection::generate_section_id();
        section->init(section_id);
        m_sections.emplace(section_id, section);
    }
}
void ServerBase::open(std::string open_ip, int open_port, std::function<std::shared_ptr<Session>()> session_factory, int accept_back_log)
{
    m_session_factory = session_factory;
    register_socket_in_iocp_handle(m_listen_socket);
    NetworkUtil::bind(m_listen_socket, open_ip.c_str(), open_port);
    NetworkUtil::listen(m_listen_socket, 1);

    for(int i = 0; i < accept_back_log; ++i)
    {
        AcceptIO* io = new AcceptIO;
        if(false == NetworkUtil::accept(m_listen_socket, io))
        {
            // 서버 정지
            return; 
        }
           
    }
}

void ServerBase::on_accept(int bytes_transferred, NetworkIO* io) {
    
    AcceptIO* accept_io = reinterpret_cast<AcceptIO*>(io);

    std::shared_ptr<Session> session = m_session_factory();
    session->set_id(Session::generate_session_id());

    // receive 완성 후 호출
    NetworkUtil::receive();

    // 로드 밸런싱
    m_sections[0]->enter_section(session);

    accept_io->Init();
    
    if(false == NetworkUtil::accept(m_listen_socket, accept_io))
    {
        // 서버 중지
        return;
    }
}

void ServerBase::on_recv(int bytes_transferred, NetworkIO* io)
{
    RecvIO* recv_io = static_cast<RecvIO*>(io);
    std::shared_ptr<Session> session = recv_io->m_session;

    int complete_byte_length = 0;
    while(true)
    {
        int remain_len = bytes_transferred - complete_byte_length;
        if(remain_len > PACKET_HEADER_SIZEOF) break;

        PacketHeader header = *(reinterpret_cast<PacketHeader*>(session->get_recv_buffer().GetReadPos() + complete_byte_length));
        
        if(header.packet_size > remain_len) break;

        std::shared_ptr<Packet> packet = std::shared_ptr<Packet>();
        packet->set_packet(session->get_recv_buffer().GetReadPos() + complete_byte_length, header.packet_size);
        packet->set_owner(session);

        m_packet_queue.push(packet);

        complete_byte_length += header.packet_size;
    }
}

void ServerBase::central_thread_work()
{
    while(is_running() == true)
    {
        std::shared_ptr<Packet> packet;

        if(m_packet_queue.try_pop(packet) == false) continue;
        if(packet == nullptr) continue;
        std::shared_ptr<Session> session = packet->get_owner();
        
        std::shared_ptr<iTask> task;
        task->func = [&]() { session->execute_packet(packet); };

        m_sections[session->get_section_id()]->push_task(task);
    }
}
