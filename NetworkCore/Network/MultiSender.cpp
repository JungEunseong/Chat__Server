#include "pch.h"
#include "MultiSender.h"

MultiSender::MultiSender(Session* session) : m_owner(session)
{
    m_send_io.set_session(session);
}

MultiSender::~MultiSender()
{
    
}

bool MultiSender::register_packet(Packet* packet)
{
    m_register_packet.push(packet);
    
    bool compare_to = false;
    if(true == m_sending_flag.compare_exchange_strong(compare_to, true))
        return send();


    return true;
}

bool MultiSender::on_send()
{
    while(false == m_sending_packet.empty())
    {
        Packet* packet = m_sending_packet.front();
        m_sending_packet.pop();
        xdelete packet;
    }

    if(false == is_register_queue_empty())
        send();
    else
        m_sending_flag.store(false);
    
    return true;
}

bool MultiSender::send()
{
    m_send_io.Clear();

    while(false == m_register_packet.empty())
    {
        Packet* packet = nullptr;
        
        if(false == m_register_packet.try_pop(packet))
            continue;
            
        WSABUF buf;
        buf.buf = packet->get_buffer().data();
        buf.len = packet->get_size();
        
        m_send_io.m_buffers.emplace_back(buf);
        m_sending_packet.push(packet);
    }

    bool is_not_pending = false;
    DWORD send_byte_size = 0;
    
    if(false == NetworkUtil::send(&m_send_io, is_not_pending, send_byte_size))
        return false;

    if(true == is_not_pending)
    {
        NetworkCore* network_core = m_owner->get_network_core();
        if(nullptr == network_core)
        {
            // TODO:LOG
            m_owner->do_disconnect();
            return false;
        }
        
        ULONG_PTR key = 0;
        if(0 == ::PostQueuedCompletionStatus(network_core->get_iocp_handle(), send_byte_size, key, &m_send_io))
        {
            //TODO: LOG
            m_owner->do_disconnect();
            return false;
        }
    }
    
    return true;
}
