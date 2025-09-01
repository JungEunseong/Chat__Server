#pragma once

class Logger
{
public:
    Logger();
    ~Logger();

public:
    Logger& get_logger()
    {
        static Logger logger;
        return logger;
    }

public:
    void init();
    
    bool start(std::wstring file_path, std::wstring file_name);
    void stop();

private:
    void logger_thread_work();
private:
    std::ofstream* m_current_file;
    std::thread m_logger_thread;
    bool m_is_running;

    int m_file_counter;
    std::wstring m_current_file_name;
    std::tm* m_current_time;

    concurrency::concurrent_queue<std::wstring> m_log_datas;
};
