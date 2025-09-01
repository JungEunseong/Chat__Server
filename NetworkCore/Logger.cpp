#include "pch.h"
#include "Logger.h"

#include <string>

void Logger::init()
{
   m_file_counter = 0; 
}

bool Logger::start(std::wstring file_path, std::wstring file_name)
{
    m_is_running = true;
    
    m_current_file = xnew std::ofstream;
    auto now = std::chrono::system_clock::now();
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
    m_current_time = std::localtime(&now_time_t);

    m_current_file_name = file_name + L"_" + std::to_wstring(m_current_time->tm_year) + std::to_wstring(m_current_time->tm_mon) + std::to_wstring(m_current_time->tm_mday) + L"_" + std::to_wstring(m_file_counter);
    m_current_file->open(file_path + L"\\" + m_current_file_name);

    if (false == m_current_file->is_open())
    {
        //TODO: xstop 
        return false;
    }

    m_logger_thread = std::thread(logger_thread_work);
}

void Logger::stop()
{
    m_is_running = false;

    
}

void Logger::logger_thread_work()
{
   while (m_is_running)
   {
       std::wstring log;

       if (false == m_log_datas.try_pop(log))
           continue;

       //TODO: WRITE LOG

       
   } 
}
