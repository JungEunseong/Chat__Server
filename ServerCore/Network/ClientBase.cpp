#include "pch.h"
#include "ClientBase.h"

void ClientBase::on_connect(int bytes_transferred, NetworkIO* io)
{
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
