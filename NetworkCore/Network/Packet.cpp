#include "pch.h"
#include "Packet.h"

Packet::Packet()
{
    m_buffer = xnew char[PACKET_MAX_SIZE] {0, };
}

Packet::Packet(Packet* packet)
{
    m_owner = packet->m_owner;
    m_buffer = packet->m_buffer;
    m_current_idx = packet->m_current_idx;
}

Packet::~Packet()
{
    xdelete[] m_buffer;
}

void Packet::set_packet(char* data, int size)
{
    memcpy(m_buffer, data, size);  
}

void Packet::set_owner(Session* session)
{
    m_owner = session;
}

Session* Packet::get_owner()
{
    return m_owner;
}
