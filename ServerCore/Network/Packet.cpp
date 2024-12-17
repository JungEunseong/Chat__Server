#include "pch.h"
#include "Packet.h"

Packet::Packet()
{
}

Packet::~Packet()
{
}

void Packet::set_packet(char* data, int size)
{
    memcpy(m_buffer.data(), data, size);  
}

void Packet::set_owner(std::shared_ptr<Session> session)
{
    m_owner = session;
}

std::shared_ptr<Session> Packet::get_owner()
{
    return m_owner;
}
