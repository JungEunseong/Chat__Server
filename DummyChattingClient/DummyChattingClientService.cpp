#include "pch.h"
#include "DummyChattingClientService.h"


void DummyChattingClientService::init(int iocp_thread_count)
{
    ClientBase::init(iocp_thread_count);

    m_performance_monitor_thread = std::thread(&DummyChattingClientService::performance_monitor_thread_work, this);

    for (int i = 0; i < 8; ++i)
        m_job_threads.emplace_back(std::thread(&DummyChattingClientService::job_thread_work, this));
}

void DummyChattingClientService::on_iocp_io(NetworkIO* io, int bytes_transferred)
{
    ClientBase::on_iocp_io(io, bytes_transferred);
}

void DummyChattingClientService::performance_monitor_thread_work()
{
    while (true == is_running())
    {
        std::this_thread::sleep_for(std::chrono::seconds(client_rtt_check_interval));
        
        m_rtt_lock.lock();

        if (true == m_rtt_data.empty())
        {
            m_rtt_lock.unlock();
            continue;
        }
        
        std::wcout << L"=================== [RTT performance] ===================" << std::endl;
        
        for (int i = 0; i < packet_number::MAX; ++i)
        {
            if (0 == m_rtt_data.count(i))
                continue;
            auto range = m_rtt_data.equal_range(i);

            long long sum = std::accumulate(
                   range.first, range.second, 0,
                   [](int acc, const std::pair<int, int>& p) {
                       return acc + p.second;
                   });

            std::wcout << L"pakcet number [" << i <<L"] rtt avg:" << sum / m_rtt_data.count(i) << std::endl;
        }

        m_rtt_data.clear();
        
        m_rtt_lock.unlock();
        
    }
}
