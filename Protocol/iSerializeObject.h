#pragma once
class iSerializeObject
{
public:
	iSerializeObject() = default;
	~iSerializeObject() = default;

public:
	DEFINE_SERIALIZER();
};

class iProtocol : public iSerializeObject
{
public:
	iProtocol(unsigned short protocol) : protocol_no(protocol) { }
	~iProtocol() = default;

public:
	unsigned short protocol_no;

	DEFINE_SERIALIZER_WITH_PARENT(iSerializeObject, protocol_no)
};

