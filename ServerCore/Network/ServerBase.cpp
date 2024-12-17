#include "pch.h"
#include "ServerBase.h"

void ServerBase::on_recv(int bytes_transferred, NetworkIO* io)
{
    RecvIO* recv_io = static_cast<RecvIO*>(io);
    std::shared_ptr<Session> session = recv_io->m_session;

    int complete_byte_length = 0;
    while(true)
    {
        int remain_len = bytes_transferred - complete_byte_length;
        if(remain_len > PACKET_HEADER_SIZEOF) break;

        PacketHeader header = *(reinterpret_cast<PacketHeader*>(session->get_recv_buffer().GetReadPos() + complete_byte_length));
        
        if(header.packet_size > remain_len) break;

        std::shared_ptr<Packet> packet = std::shared_ptr<Packet>();
        packet->set_packet(session->get_recv_buffer().GetReadPos() + complete_byte_length, header.packet_size);
        packet->set_owner(session);

        m_packet_queue.push(packet);

        complete_byte_length += header.packet_size;
    }
}

void ServerBase::central_thread_work()
{
    while(is_running() == true)
    {
        std::shared_ptr<Packet> packet;

        if(m_packet_queue.try_pop(packet) == false) continue;
        if(packet == nullptr) continue;
        std::shared_ptr<Session> session = packet->get_owner();
        
        std::shared_ptr<iTask> task;
        task->func = [&]() { session->execute_packet(packet); };

        m_sections[session->get_section_id()]->push_task(task);
    }
}
