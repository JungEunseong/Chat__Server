﻿#pragma once

class Packet
{
    
    struct PacketHeader
    {
        unsigned short packet_size;
        unsigned short protocol_no;
    };

    enum
    {
        PACKET_MAX_SIZE = 65535,
        PACKET_SIZE_SIZEOF = sizeof(unsigned short),
        PACKET_PROTOCOL_SIZEOF = sizeof(unsigned short),
        PACKET_HEADER_SIZEOF = sizeof(struct PacketHeader),
    };

public:
    Packet();
    ~Packet();
public:

    void initialize(unsigned short protocol_no) { *(static_cast<unsigned short*>(get_protocol_ptr())) = protocol_no; }
    void finalize() { *(static_cast<unsigned short*>(get_size_ptr())) = static_cast<unsigned short>(m_buffer.size()); }
    unsigned short get_size() const { return *m_buffer.data(); }
    unsigned short get_protocol() const { return *(m_buffer.data() + PACKET_SIZE_SIZEOF); }

public:
    
    /* --------------------------------------------- push --------------------------------------------- */
    template<typename t>
    void push(t& data)
    {
        size_t data_size = sizeof(t);
        //TODO: stop(current_idx + data_size >= PACKET_MAX_SIZE)
        
        ::memcpy_s(get_current_idx_ptr(), data_size, &data, data_size);
        m_current_idx += static_cast<int>(data_size);
    }

    void push(void* data, int size)
    {
        //TODO: stop(current_idx + size >= PACKET_MAX_SIZE)
        
        ::memcpy_s(get_current_idx_ptr(), size, data, size);
        m_current_idx += size;
    }
    void push(char* data, int size) { push(reinterpret_cast<void*>(data), size); }

    void push(const std::string_view& data)
    {
        size_t data_size = data.size();
        //TODO: stop(current_idx + size >= PACKET_MAX_SIZE)
        
        ::memcpy_s(get_current_idx_ptr(), data_size, data.data(), data_size);
        m_current_idx += static_cast<int>(data_size);
    }
    
    /* --------------------------------------------- pop --------------------------------------------- */
    template<typename t>
    void pop(t& data)
    {
        size_t data_size = sizeof(t);
        
        ::memcpy_s(data, data_size, get_current_idx_ptr(), data_size);
        m_current_idx += data_size;
    }
    
    void pop(void* data, size_t size)
    {
        //TODO: stop(current_idx + size >= PACKET_MAX_SIZE)
        
        ::memcpy_s(data, size, get_current_idx_ptr(), size);
        m_current_idx += static_cast<int>(size);
    }

    void pop(char* data, int size) { pop(reinterpret_cast<void*>(data), size); }

    void pop(std::string_view& data)
    {
        const size_t data_size = data.size();
        //TODO: stop(current_idx + size >= PACKET_MAX_SIZE)
        
        ::memcpy_s(const_cast<char*>(data.data()), data_size, get_current_idx_ptr(), data_size);
        m_current_idx += static_cast<int>(data_size);
    }
    

private:
    void* get_size_ptr() { return m_buffer.data(); }
    void* get_protocol_ptr() { return m_buffer.data() + PACKET_SIZE_SIZEOF; }
    void* get_current_idx_ptr() {return m_buffer.data() + PACKET_SIZE_SIZEOF + m_current_idx; }

private:
    std::vector<char> m_buffer;

    int m_current_idx;
};
