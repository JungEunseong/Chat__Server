#include "pch.h"
#include "ClientSession.h"

void ClientSession::on_disconnected()
{
    Session::on_disconnected();
    m_section->exit_section(get_id());
    m_section = nullptr;
}
