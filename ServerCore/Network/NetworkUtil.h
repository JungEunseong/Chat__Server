#pragma once

class NetworkUtil
{
public:
   static bool socket_opt_setting(SOCKET socket);
   static bool register_socket();
   static bool bind(SOCKET socket, const char* ip, int port);
   static bool listen(SOCKET socket, int backlog = 1);
   static bool accept(SOCKET listen_socket, class AcceptIO* io);
   static bool connect(std::shared_ptr<class NetworkCore> network_core, SOCKET socket, class ConnectIO* io);
   static bool send();
   static bool receive();
   static bool disconnect();
    
};
