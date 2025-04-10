#include "pch.h"
#include "ServerBase.h"

#include <memory>
#include <memory>

void ServerBase::init(int iocp_thread_count, int section_count)
{
    NetworkCore::init(iocp_thread_count);
    
    for(int i = 0; i < section_count; ++i)
    {
        NetworkSection* section = xnew NetworkSection;
        int section_id = NetworkSection::generate_section_id();
        section->init(section_id);
        m_sections.emplace(section_id, section);
    }
}
void ServerBase::open(std::string open_ip, int open_port, std::function<Session*()> session_factory, int accept_back_log)
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

    Session* session = m_session_factory();
    session->set_id(Session::generate_session_id());
    session->set_socket(accept_io->m_socket);

    register_socket_in_iocp_handle(session->get_socket());

    if(true == session->do_recieve())
        m_sections[0]->enter_section(session); // TODO: 로드 밸런싱 로직
    
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
    Session* session = recv_io->m_session;

    int complete_byte_length = 0;
    while(true)
    {
        int remain_len = bytes_transferred - complete_byte_length;
        if(remain_len > PACKET_HEADER_SIZEOF) break;

        PacketHeader header = *(reinterpret_cast<PacketHeader*>(session->get_recv_buffer().GetReadPos() + complete_byte_length));
        
        if(header.packet_size > remain_len) break;

        Packet* packet = xnew Packet; 
        packet->set_packet(session->get_recv_buffer().GetReadPos() + complete_byte_length, header.packet_size);
        packet->set_owner(session);

        m_packet_queue.push(packet);

        complete_byte_length += header.packet_size;
    }

    session->get_recv_buffer().Clean();

    if(false == session->do_recieve())
    {
        // TODO:로그
        return;
    }
    
}

void ServerBase::on_send(int bytes_transferred, NetworkIO* io)
{
    SendIO* send_io = static_cast<SendIO*>(io);

    send_io->m_session->on_send(bytes_transferred);
}

void ServerBase::central_thread_work()
{
    while(is_running() == true)
    {
        Packet* packet;

        if(false == m_packet_queue.try_pop(packet))
            continue;
        
        Session* session = packet->get_owner();
       
        iTask* task = xnew iTask;
        task->func = [&]() { session->execute_packet(packet); };

        session->get_section()->push_task(task);
    }
}
