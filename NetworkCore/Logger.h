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
    bool start(std::wstring file_path);
    void stop();

private:
    void logger_thread_work();
private:

    std::ofstream* m_current_file;
    std::thread m_logger_thread;
};
