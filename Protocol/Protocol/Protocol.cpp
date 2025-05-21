#include "pch.h"


class S2C_ROOM_LIST : public iProtocol 
{
public:
	S2C_ROOM_LIST() : iProtocol(packet_number::ROOM_LIST) { }
	~S2C_ROOM_LIST() = default;

public:
	
	DEFINE_SERIALIZER_WITH_PARENT(iProtocol, protocol_no);
};