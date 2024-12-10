#pragma once

struct iTask : std::enable_shared_from_this<iTask>
{
    std::function<void()> func;
};
