﻿#include "pch.h"
#include "DummyChattingClientService.h"

void DummyChattingClientService::on_iocp_io(NetworkIO* io, int bytes_transferred)
{
    ClientBase::on_iocp_io(io, bytes_transferred);
}
