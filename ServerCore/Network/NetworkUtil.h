﻿#pragma once

class NetworkUtil
{
public:
   static bool socket_opt_setting(SOCKET socket);
   static bool register_socket();
   static bool bind(SOCKET socket, const char* ip, int port);
   static bool listen(SOCKET socket, int backlog = 1);
   static bool accept(SOCKET listen_socket, class AcceptIO* io);
   static bool connect(class NetworkCore* network_core, SOCKET socket, class ConnectIO* io);
   static bool send(class SendIO* io, bool& is_not_pending, DWORD& send_byte_size);
   static bool receive(SOCKET socket, class RecvIO* io);
   static bool disconnect();
    
};
