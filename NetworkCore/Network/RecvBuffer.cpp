#include "stdafx.h"
#include "RecvBuffer.h"

RecvBuffer::RecvBuffer()
{
    RecvBuffer(/*TODO: const value로 대체*/65535 * 3);
}

RecvBuffer::RecvBuffer(int buffer_size)
{
    if(buffer_size < 0)
    {
        //TODO: crash
    }
    m_buffer.reserve(buffer_size);
    m_max_buffer_size = buffer_size;
    m_read_pos = 0;
    m_write_pos = 0;
}

RecvBuffer::~RecvBuffer()
{
    m_buffer.clear();
}

bool RecvBuffer::OnRead(int data_size)
{
    if(GetDataSize() < data_size)
        return false;

    m_read_pos += data_size;
    
    Clean();
    return true;
}

bool RecvBuffer::OnWrite(int data_size)
{
    if(GetRemainingSize() < data_size)
        return false;

    m_write_pos += data_size;

    return true;
}

void RecvBuffer::Clean()
{
    int data_size = GetDataSize();
    if(data_size == 0)
        m_read_pos = m_write_pos = 0;
    else if(data_size < /*TODO: const value로 대체*/65535)
    {
        ::memcpy_s(&m_buffer[0], m_read_pos, &m_buffer[m_read_pos], data_size);
        m_read_pos = 0;
        m_write_pos = data_size;
    }
}

