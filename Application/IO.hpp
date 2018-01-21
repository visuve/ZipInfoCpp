#pragma once

#include <iostream>
#include <istream>

namespace IO
{
    // Extract by reference
    template <typename Value>
    void Extract(std::istream& stream, Value& value) = delete;

    template <>
    inline void Extract(std::istream& stream, uint16_t& value)
    {
        stream.read(reinterpret_cast<char*>(&value), 2);
    }

    template <>
    inline void Extract(std::istream& stream, uint32_t& value)
    {
        stream.read(reinterpret_cast<char*>(&value), 4);
    }

    // Extract by copy
    template <typename Value>
    Value Extract(std::istream& stream) = delete;

    template <>
    inline uint16_t Extract(std::istream& stream)
    {
        uint16_t value = 0;
        Extract(stream, value);
        return value;
    }

    template <>
    inline uint32_t Extract(std::istream& stream)
    {
        uint32_t value = 0;
        Extract(stream, value);
        return value;
    }

    class FmtGuard
    {
    public:
        FmtGuard() :
            m_state(nullptr)
        {
            m_state.copyfmt(std::cout);
        }

        ~FmtGuard()
        {
            std::cout.copyfmt(m_state);
        }

    private:
        std::ios m_state;
    };
}