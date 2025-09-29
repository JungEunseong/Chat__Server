﻿#pragma once

class ServerBase : public NetworkCore
{
public:
    ServerBase() = default;
    virtual ~ServerBase() = default;
    
public:
    virtual void init(int iocp_thread_count = 1, int section_count = 1);
    void open(std::string open_ip, int open_port, std::function<class ClientSession*()> session_factory, int accpet_back_log = 1);
    void add_section(class NetworkSection* section);
    
    double get_fps_avg();
    double get_recv_tps_avg();
    double get_send_tps_avg();
    void print_fps_info();
    
    double get_accept_tps() const { return m_current_accept_tps; }
    void update_accept_tps_info();
    void increment_accept_count_for_tps();

public:
    void on_accept(int bytes_transferred, NetworkIO* io);

private:
    void central_thread_work();
    void fps_monitor_thread_work();

protected:
    void on_iocp_io(NetworkIO* io, int bytes_transferred) override;
    virtual NetworkSection* select_first_section() abstract;

protected:
    SOCKET m_listen_socket;
    
    std::thread m_central_thread;
    std::thread m_performance_monitor_thread;

    std::map<unsigned int, class NetworkSection*> m_sections;
    std::function<class ClientSession*()> m_session_factory;
    
    // Accept TPS 측정 관련
    std::chrono::high_resolution_clock::time_point m_last_accept_tps_time;
    std::atomic<int> m_accept_count;
    double m_current_accept_tps;
};
