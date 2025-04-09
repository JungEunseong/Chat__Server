#pragma once

 
class Acceptor
{
public:
    void set_address(const char* ip, int port);

private:
	class NetworkCore* m_network_core;

    SOCKET m_listen_socket;

	char m_ip[22];
	int m_port = 0;
};
