#pragma once

class ClientSession : public Session
{
    ClientSession() = default;
    ~ClientSession() = default;
public:
    unsigned int get_section_id();
    class NetworkSection* get_section()  { return m_section; }
    void set_section(class NetworkSection* section) { m_section = section; }
    
    void on_connected() override;
    int on_recieve() override;
    void on_send(int data_size) override;
    void on_disconnected() override;
    void execute_packet(Packet* packet) override;

protected:
    class NetworkSection* m_section;
};
