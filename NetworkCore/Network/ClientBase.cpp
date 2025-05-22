#include "pch.h"
#include "ClientBase.h"

#include "ServerSession.h"

void ClientBase::open(std::string connecting_ip, int connecting_port, std::function<ServerSession*()> session_factory, int session_count)
{
    m_session_factory = session_factory;

    for (int i = 0; i < session_count; ++i)
    {
        ServerSession* session = m_session_factory();
        session->set_id(Session::generate_session_id());
        session->set_socket(NetworkUtil::create_socket());
        session->set_remote_ip(connecting_ip.c_str());
        session->set_remote_port(connecting_port);

        session->do_connect();

        m_sessions.emplace(session->get_id(), session);
    }
}

void ClientBase::on_iocp_io(NetworkIO* io, int bytes_transferred)
{
    Session* session = io->get_session();
    
    switch(io->get_type())
    {
    case IoType::CONNECT:
        session->complete_connect();
        break;
    case IoType::DISCONNECT:
        session->complete_disconnect();
        break;
    case IoType::RECV:
        session->complete_recieve(bytes_transferred);
        break;
    case IoType::SEND:
        session->complete_send(bytes_transferred);
        break;
    default:
        // TODO: error log
        break;
    }}

void ClientBase::job_thread_work()
{
    while(is_running() == true)
    {
        Packet* packet;

        if(false == m_packet_queue.try_pop(packet))
            continue;
        
        Session* session = packet->get_owner();
        
        session->execute_packet(packet);

        xdelete packet;
    }
}
