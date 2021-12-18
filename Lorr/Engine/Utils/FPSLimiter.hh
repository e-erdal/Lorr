#pragma once

#include <chrono>
#include <thread>

// https://stackoverflow.com/a/59165784
namespace lr
{
    template<intmax_t FPS>
    class FrameLimiter
    {
    public:
        FrameLimiter():
            time_between_frames{1},  // std::ratio<1, FPS> seconds
            tp{std::chrono::steady_clock::now()}
        {
        }

        void Wait()
        {
            // add to time point
            tp += time_between_frames;

            // and sleep until that time point
            std::this_thread::sleep_until(tp);
        }

    private:
        // a duration with a length of 1/FPS seconds
        std::chrono::duration<double, std::ratio<1, FPS>> time_between_frames;

        // the time point we'll add to in every loop
        std::chrono::time_point<std::chrono::steady_clock, decltype(time_between_frames)> tp;
    };
}  // namespace lr