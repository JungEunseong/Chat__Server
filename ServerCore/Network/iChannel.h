#pragma once

class iChannel
{
public:
    iChannel(int channel_no);
    ~iChannel();

public:
    void RegisterTask(iTask* task) {m_task_queue.push(task); }
protected:
    void task_thread_work();

private:
    bool m_is_running;
    int m_channel_no;
    
    std::thread m_channel_thread;
    Concurrency::concurrent_queue<iTask*> m_task_queue;
};
