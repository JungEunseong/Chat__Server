#pragma once

struct iTask : std::enable_shared_from_this<iTask>
{
    std::chrono::steady_clock::time_point execute_time;
    long long delay_time;
    bool is_repeat;
    std::function<void()> func;
};
