#pragma once

namespace IO
{
    // Extract by reference
    template <typename Value>
    void Extract(std::istream& stream, Value& value) = delete;

    template <>
    inline void Extract(std::istream& stream, int16_t& value)
    {
        stream.read(reinterpret_cast<char*>(&value), 2);
    }

    template <>
    inline void Extract(std::istream& stream, int32_t& value)
    {
        stream.read(reinterpret_cast<char*>(&value), 4);
    }

    template <>
    inline void Extract(std::istream& stream, int64_t& value)
    {
        stream.read(reinterpret_cast<char*>(&value), 8);
    }

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

    template <>
    inline void Extract(std::istream& stream, uint64_t& value)
    {
        stream.read(reinterpret_cast<char*>(&value), 8);
    }

    template <size_t N>
    inline void Extract(std::istream& stream, std::bitset<N>& value)
    {
        static_assert(N % 8 == 0);
        stream.read(reinterpret_cast<char*>(&value), N / 8);
    }

    // Extract by copy
    template <typename Value>
    Value Extract(std::istream& stream) = delete;

    template <>
    inline int16_t Extract(std::istream& stream)
    {
        int16_t value = 0;
        Extract(stream, value);
        return value;
    }

    template <>
    inline int32_t Extract(std::istream& stream)
    {
        int32_t value = 0;
        Extract(stream, value);
        return value;
    }

    template <>
    inline int64_t Extract(std::istream& stream)
    {
        int64_t value = 0;
        Extract(stream, value);
        return value;
    }

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

    template <>
    inline uint64_t Extract(std::istream& stream)
    {
        uint64_t value = 0;
        Extract(stream, value);
        return value;
    }

    template <size_t N>
    inline std::bitset<N> Extract(std::istream& stream)
    {
        static_assert(N % 8 == 0);
        std::bitset<N> value;
        Extract<N>(stream, value);
        return value;
    }
}