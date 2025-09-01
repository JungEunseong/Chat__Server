#pragma once

class DummyChattingClientService : public ClientBase
{
public:
    DummyChattingClientService() = default;
    ~DummyChattingClientService() override = default;

public:
    void init(int iocp_thread_count) override;
    
protected:
    void on_iocp_io(NetworkIO* io, int bytes_transferred) override;
    
public:
    void push_rtt_data(int packet_number, long long rtt) {  std::lock_guard<std::mutex> lock_guard(m_rtt_lock); m_rtt_data.emplace(packet_number, rtt); }

private:
    void performance_monitor_thread_work();
private:

    std::thread m_performance_monitor_thread;
    std::vector<std::thread> m_job_threads;

    std::mutex m_rtt_lock;
    std::multimap<unsigned short, long long> m_rtt_data;
};
