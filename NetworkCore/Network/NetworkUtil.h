#pragma once

class NetworkUtil
{
public:
   static bool socket_opt_setting(SOCKET socket); 
   static bool bind(SOCKET socket, const char* ip, int port);
   static bool listen(SOCKET socket, int backlog = 1);
   static bool accept(class std::shared_ptr<class NetworkCore> network_core, SOCKET listen_socket, std::shared_ptr<class AcceptIO> io);
   static bool connect();
   static bool send();
   static bool receive();
   static bool disconnect();
    
};
