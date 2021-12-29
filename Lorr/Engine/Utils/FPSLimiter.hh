#pragma once

#include <EASTL/chrono.h>
#include <thread>

// https://stackoverflow.com/a/59165784
namespace lr
{
    template<intmax_t FPS>
    class FrameLimiter
    {
    public:
        FrameLimiter():
            time_between_frames{1},  // eastl::ratio<1, FPS> seconds
            tp{eastl::chrono::steady_clock::now()}
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
        eastl::chrono::duration<double, eastl::ratio<1, FPS>> time_between_frames;

        // the time point we'll add to in every loop
        eastl::chrono::time_point<eastl::chrono::steady_clock, decltype(time_between_frames)> tp;
    };
}  // namespace lr