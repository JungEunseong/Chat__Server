#pragma once

enum packet_number : unsigned short 
{
	PING_PACKET = 0,
	RTT_PACKET,
	ROOM_LIST = 100,
	CHAT_MESSAGE,
	LOGIN,
	CREATE_ROOM,
	MAX,
};
