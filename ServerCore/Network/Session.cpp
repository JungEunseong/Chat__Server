#include "pch.h"
#include "Session.h"

int Session::generate_session_id()
{
    static int session_id = 0;

    return ++session_id;
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
    return true;
}

void Session::complete_send(int send_size)
{
    on_send(send_size);
    m_multi_sender.on_send();
}

unsigned int Session::get_section_id()
{
    return m_section->get_id();
}
