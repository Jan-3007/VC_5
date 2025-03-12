#pragma once



constexpr
TickType_t ms_to_ticks(uint ms)
{
    return ((ms + (portTICK_PERIOD_MS - 1)) / portTICK_PERIOD_MS);
}


