#include "stdafx.h"
#include "iChannel.h"

iChannel::iChannel(int channel_no)
{
    m_channel_no = channel_no;
    m_channel_thread = std::thread([this](){ task_thread_work(); });
}

iChannel::~iChannel()
{
}

void iChannel::task_thread_work()
{
    while(m_is_running == true)
   {
       while(m_task_queue.empty() == false)
       {
           std::shared_ptr<iTask> task = nullptr;
           if(m_task_queue.try_pop(task) == false)
               break;

           task->func();
       }
   }
}
