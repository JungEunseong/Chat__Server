#include "pch.h"
#include "ChatServerService.h"

void ChatServerService::init(int iocp_thread_count, int section_count)
{
    ServerBase::init(iocp_thread_count, section_count);

    m_lobby = xnew Lobby;
    m_lobby->init(this, NetworkSection::generate_section_id());

    m_sections.emplace(m_lobby->get_id(), m_lobby);
}

void ChatServerService::on_iocp_io(NetworkIO* io, int bytes_transferred)
{
    ServerBase::on_iocp_io(io, bytes_transferred);
}

NetworkSection* ChatServerService::select_first_section()
{
    return m_lobby;
}
