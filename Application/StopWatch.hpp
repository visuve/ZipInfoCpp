#pragma once

#include <chrono>
#include <string>
#include <iostream>

namespace
{
    using NanoSeconds = std::chrono::nanoseconds;
    using MicroSeconds = std::chrono::microseconds;
    using MilliSeconds = std::chrono::milliseconds;

    template <typename T>
    class StopWatch
    {
    public:
        StopWatch();

        virtual ~StopWatch()
        {
            PrintDuration();
        }

        T Duration() const
        {
            const auto diff = std::chrono::high_resolution_clock::now() - m_start;
            return std::chrono::duration_cast<T>(diff);
        }

        void PrintDuration() const
        {
            std::cout << "Completed in: " << Duration().count() << m_unit << std::endl;
        }

        StopWatch(const StopWatch&) = delete;
        StopWatch& operator = (const StopWatch&) = delete;

    private:
        const std::chrono::high_resolution_clock::time_point m_start = std::chrono::high_resolution_clock::now();
        const std::string m_unit;
    };

    StopWatch<NanoSeconds>::StopWatch() :
        m_unit("ns")
    {
    }

    StopWatch<MicroSeconds>::StopWatch() :
        m_unit("us")
    {
    }

    StopWatch<MilliSeconds>::StopWatch() :
        m_unit("ms")
    {
    }
}