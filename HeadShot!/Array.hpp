#pragma once

#include <cstdlib>

template<typename T>
class Array {
public:
    Array() {
        _data = nullptr;
        _length = 0;
    }

    explicit Array(std::size_t len)
    {
        _data = new T[len];
        _length = len;
    }

    Array(const Array& other)
        : _data(new T[other._length]), _length(other._length)
    {
        std::copy(other._data, other._data + other._length, _data);
    }

    Array(Array&& other) noexcept
        : _data(other._data), _length(other._length)
    {
        other._data = nullptr;
        other._length = 0;
    }

    Array& operator=(const Array& other)
    {
        if(this != &other) 
        {
            delete[] _data;
            _length = other._length;
            _data = new T[other._length];
            std::copy(other._data, other._data + other._length, _data);
        }
        return *this;
    }

    Array& operator=(Array&& other) noexcept
    {
        if(this != &other) 
        {
            delete[] _data;
            _data = other._data;
            _length = other._length;
            other._data = nullptr;
            other._length = 0;
        }
        return *this;
    }

    ~Array()
    {
        delete[] _data;
    }

    void New(const std::size_t& len)
    {
        _data = new T[len];
        _length = len;
    }

    const T* Data() const noexcept
    {
        return _data;
    }

    T* Data() noexcept
    {
        return _data;
    }

    /**
     * \brief Hardly set a new data chunk with given length. May be unsafe
     * \param data pointer to new data
     * \param length length of new array
     */
    void SetData(T* data, std::size_t length)
    {
        _data = data;
        _length = length;
    }

    std::size_t Length() const noexcept
    {
        return _length;
    }

    std::size_t& Length() noexcept
    {
        return _length;
    }

private:
    T* _data;
    std::size_t _length;
};