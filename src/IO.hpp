#pragma once

namespace IO
{
    template <typename T,
        typename = std::enable_if_t<std::is_integral<T>::value>>
    void Extract(std::istream& stream, T& value) 
    {
        stream.read(reinterpret_cast<char*>(&value), sizeof(T));
    }

    template <size_t N>
    void Extract(std::istream& stream, std::bitset<N>& value)
    {
        static_assert(N % 8 == 0);
        stream.read(reinterpret_cast<char*>(&value), N / 8);
    }

    template <typename T, 
        typename = std::enable_if_t<std::is_integral<T>::value>>
    T Extract(std::istream& stream)
    {
        T value;
        Extract(stream, value);
        return value;
    }

    template <size_t N>
    std::bitset<N> Extract(std::istream& stream)
    {
        static_assert(N % 8 == 0);
        std::bitset<N> value;
        Extract<N>(stream, value);
        return value;
    }
}