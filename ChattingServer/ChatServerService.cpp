#include "pch.h"
#include "ChatServerService.h"

void ChatServerService::on_iocp_io(NetworkIO* io, int bytes_transferred)
{
    ServerBase::on_iocp_io(io, bytes_transferred);
}
