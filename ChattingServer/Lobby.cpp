#include "pch.h"
#include "Lobby.h"

void Lobby::enter_section(ClientSession* session)
{
    NetworkSection::enter_section(session);
}

void Lobby::exit_section(int session_id)
{
    NetworkSection::exit_section(session_id);
}
