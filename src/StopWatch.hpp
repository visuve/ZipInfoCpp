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

    template<>
    StopWatch<MilliSeconds>::StopWatch() :
        m_unit("ms")
    {
    }
}