#include "pch.h"
#include "Session.h"

int Session::generate_session_id()
{
    static int session_id = 0;

    return ++session_id;
}

void Session::do_recieve()
{
}

void Session::do_disconnect()
{
}

unsigned int Session::get_section_id()
{
    return m_section->get_id();
}
