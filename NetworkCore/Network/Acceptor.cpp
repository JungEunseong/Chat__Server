#include "stdafx.h"
#include "Acceptor.h"

void Acceptor::set_address(const char* ip, int port)
{
   memcpy_s(m_ip, sizeof(m_ip), ip, sizeof(m_ip));
   m_port = port;
}
