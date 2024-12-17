#pragma once

struct iTask : std::enable_shared_from_this<iTask>
{
    bool is_repeat = false;
    std::chrono::steady_clock::time_point execute_time;
    long long delay_time = 0;
    std::function<void()> func;
};
