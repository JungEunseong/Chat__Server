#pragma once

class iChannel
{
public:
    iChannel(int channel_no);
    ~iChannel();

public:
    void RegisterTask(std::shared_ptr<iTask> task) {m_task_queue.push(task->shared_from_this()); }
protected:
    void task_thread_work();

private:
    bool m_is_running;
    int m_channel_no;
    
    std::thread m_channel_thread;
    Concurrency::concurrent_queue<std::shared_ptr<iTask>> m_task_queue;
};
