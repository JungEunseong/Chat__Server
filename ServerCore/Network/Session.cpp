#include "pch.h"
#include "Session.h"

int Session::generate_session_id()
{
    static int session_id = 0;

    return ++session_id;
}

void Session::init()
{
    m_recv_io.m_session = this;
    m_disconnect_io.m_session = this;
}

bool Session::do_recieve()
{
    if(false == NetworkUtil::receive(get_socket(), &m_recv_io))
    {
        do_disconnect();
        // TODO:로그
        return false;
    }
    
    return true;
}

bool Session::do_send(Packet* packet)
{
    m_multi_sender.register_packet(packet);
    return true;
}

bool Session::do_disconnect()
{
    if (false == NetworkUtil::disconnect(m_connecting_socket, &m_disconnect_io))
        return false;
    
    return true;
}

void Session::complete_recieve(int recv_size)
{
    if (recv_size == 0)
    {
        // TODO: LOG
        do_disconnect();
        return;
    }

    if (false == m_recv_buffer.OnWrite(recv_size))
    {
        // TODO: LOG
        do_disconnect();
        return;
    }
}

void Session::complete_send(int send_size)
{
    on_send(send_size);
    m_multi_sender.on_send();
}

void Session::complete_disconnect()
{
    on_disconnected();
    
    m_section->exit_section(get_id());
    m_section = nullptr;
}

unsigned int Session::get_section_id()
{
    return m_section->get_id();
}
