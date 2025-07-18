﻿#pragma once

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

class Packet
{

public:
    Packet();
    Packet(Packet* packet);
    ~Packet();

    void set_packet(char* data, int size);
    void set_owner(class Session* session);
    Session* get_owner(); 
public:

    void initialize() { m_current_idx = 0; }
    void finalize() { *(static_cast<unsigned short*>(get_size_ptr())) = static_cast<unsigned short>(m_current_idx) + PACKET_SIZE_SIZEOF; }
    unsigned short get_size() const { return *m_buffer; }
    unsigned short get_protocol() const { return *(m_buffer + PACKET_SIZE_SIZEOF); }
    char* get_buffer() {return m_buffer; }

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

    void push(std::wstring& data)
    {
        //TODO: stop(current_idx + size >= PACKET_MAX_SIZE)
        size_t data_size = data.size() * sizeof(wchar_t);
        push(data_size);

        ::memcpy_s(get_current_idx_ptr(), data_size, data.c_str(), data_size);
        m_current_idx += static_cast<int>(data_size);
    }

    template <typename... Types>
    void push(Types&... args)
    {
         (push(args), ...); // C++ 17 fold expression
    }
    /* --------------------------------------------- pop --------------------------------------------- */
    template<typename t>
    void pop(t& data)
    {
        size_t data_size = sizeof(t);
        
        ::memcpy_s(&data, data_size, get_current_idx_ptr(), data_size);
        m_current_idx += static_cast<int>(data_size);
    }
    
    void pop(void* data, size_t size)
    {
        //TODO: stop(current_idx + size >= PACKET_MAX_SIZE)
        
        ::memcpy_s(data, size, get_current_idx_ptr(), size);
        m_current_idx += static_cast<int>(size);
    }

    void pop(char* data, int size) { pop(reinterpret_cast<void*>(data), size); }

    void pop(std::wstring& data)
    {
        //TODO: stop(current_idx + size >= PACKET_MAX_SIZE)
        size_t data_size = 0;
        pop(data_size);
        data.resize(data_size / sizeof(wchar_t));
        
        ::memcpy_s(const_cast<wchar_t*>(data.data()), data_size, get_current_idx_ptr(), data_size);
        m_current_idx += static_cast<int>(data_size);
    }
    
    template <typename... Types>
    void pop(Types&... args)
    {
        (pop(args), ...); // C++ 17 fold expression
    }
    
private:
    void* get_protocol_ptr() { return m_buffer + PACKET_SIZE_SIZEOF; }
    void* get_size_ptr() { return m_buffer; }
    void* get_current_idx_ptr() {return (m_buffer + PACKET_SIZE_SIZEOF + m_current_idx); }

private:
    char* m_buffer; // TODO: use buffer pool
    int m_current_idx;

    // For Read 
    Session* m_owner;
};

#define DEFINE_SERIALIZER(...) \
    virtual void Write(Packet& p) \
    { \
        p.push(__VA_ARGS__); \
    } \
    virtual void Read(Packet& p) \
    { \
        p.pop(__VA_ARGS__); \
    } \


#define DEFINE_SERIALIZER_WITH_PARENT(parent, ...) \
    virtual void Write(Packet& p) \
    { \
        parent::Write(p); \
        p.push(__VA_ARGS__); \
    } \
    virtual void Read(Packet& p) \
    { \
        parent::Write(p); \
        p.pop(__VA_ARGS__); \
    } \
