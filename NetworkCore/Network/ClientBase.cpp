#include "pch.h"
#include "ClientBase.h"

#include "ServerSession.h"

void ClientBase::connect(std::string connecting_ip, int connecting_port, std::function<ServerSession*()> session_factory)
{
    m_session_factory = session_factory;
    
    if (NULL == NetworkUtil::register_socket(m_iocp_handle, m_socket))
    {
        // TODO: LOG
        // TODO: STOP CLIENT
        return;
    }

    m_connect_io.m_ip = connecting_ip;
    m_connect_io.m_port = connecting_port;
    if (false == NetworkUtil::connect(m_socket, &m_connect_io))
    {
        // TODO: LOG
        // TODO: STOP CLIENT
        return;
    }

}

void ClientBase::on_connect(int bytes_transferred, NetworkIO* io)
{
    ServerSession* session = m_session_factory();
    session->set_id(Session::generate_session_id());
    session->set_socket(m_socket);

    session->on_connected();

    if (false == NetworkUtil::receive(m_socket, &session->get_recv_io()))
    {
        // TODO: LOG
        // TODO: STOP CLIENT
        return;
    }
    
    m_session = session;
}

void ClientBase::on_iocp_io(NetworkIO* io, int bytes_transferred)
{
    Session* session = io->get_session();
    
    switch(io->get_type())
    {
    case IoType::CONNECT:
        session->complete_connect(bytes_transferred);
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
    }
}
