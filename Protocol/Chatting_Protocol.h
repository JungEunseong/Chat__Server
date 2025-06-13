#include "pch.h"


class S2C_ROOM_LIST : public iProtocol 
{
public:
	S2C_ROOM_LIST() : iProtocol(packet_number::ROOM_LIST) { }
	~S2C_ROOM_LIST() = default;

public:
	
	DEFINE_SERIALIZER_WITH_PARENT(iProtocol, protocol_no);
};

class C2S_REQ_LOGIN : public iProtocol
{
public:
	C2S_REQ_LOGIN() : iProtocol(packet_number::LOGIN) { }
	~C2S_REQ_LOGIN() = default;
	
public:
	std::wstring nickname;

	DEFINE_SERIALIZER_WITH_PARENT(iProtocol, nickname)
};

class S2C_RES_LOGIN : public iProtocol
{
public:
	S2C_RES_LOGIN() : iProtocol(packet_number::LOGIN) { }
	~S2C_RES_LOGIN() = default;
	
public:
	bool is_mine;
	std::wstring nickname;
	
	DEFINE_SERIALIZER_WITH_PARENT(iProtocol, is_mine, nickname)
};

class C2S_REQ_CHAT_MESSAGE : public iProtocol
{
public:
	C2S_REQ_CHAT_MESSAGE() : iProtocol(packet_number::CHAT_MESSAGE) { }
	~C2S_REQ_CHAT_MESSAGE() = default;

public:
	std::wstring message;
	
	 DEFINE_SERIALIZER_WITH_PARENT(iProtocol, message)
};

class S2C_NTF_CHAT_MESSAGE : public iProtocol
{
public:
	S2C_NTF_CHAT_MESSAGE() : iProtocol(packet_number::CHAT_MESSAGE) { }
	~S2C_NTF_CHAT_MESSAGE() = default;

public:
    std::wstring nickname;
	std::wstring message;
	
	 DEFINE_SERIALIZER_WITH_PARENT(iProtocol, nickname, message)
};