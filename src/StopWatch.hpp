#pragma once

namespace
{
    using MilliSeconds = std::chrono::duration<uint64_t, std::ratio<1, 1000>>;

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
            const auto diff = std::chrono::high_resolution_clock::now() - _start;
            return std::chrono::duration_cast<T>(diff);
        }

        void PrintDuration() const
        {
            std::cout << "Completed in: " << Duration().count() << _unit << std::endl;
        }

        StopWatch(const StopWatch&) = delete;
        StopWatch& operator = (const StopWatch&) = delete;

    private:
        const std::chrono::high_resolution_clock::time_point _start = std::chrono::high_resolution_clock::now();
        const std::string _unit;
    };

    template<>
    StopWatch<MilliSeconds>::StopWatch() :
        _unit("ms")
    {
    }
}