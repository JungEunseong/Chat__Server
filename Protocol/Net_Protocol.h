/*class S2C_RES_RTT_PACKET : public iProtocol
{
public:
    S2C_RES_RTT_PACKET() : iProtocol(packet_number::RTT_PACKET) { }
    ~S2C_RES_RTT_PACKET() = default;
	
public:
    unsigned short rtt_protocol_number;
    long long start_tick;

    DEFINE_SERIALIZER_WITH_PARENT(iProtocol, rtt_protocol_number, start_tick)
};*/