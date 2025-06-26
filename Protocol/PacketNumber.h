#pragma once

enum packet_number : unsigned short 
{
	PING_PACKET = 0,
	ROOM_LIST = 1,
	CHAT_MESSAGE = 2,
	LOGIN = 3,
	CREATE_ROOM = 4,
	MAX,
};
