#include "pch.h"
#include "ChatClientService.h"

void ChatClientService::on_iocp_io(NetworkIO* io, int bytes_transferred)
{
    ClientBase::on_iocp_io(io, bytes_transferred);
}
