#pragma once

struct iTask
{
    bool is_repeat = false;
    std::chrono::steady_clock::time_point execute_time;
    long long delay_time = 0;
    std::function<void()> func;
};
