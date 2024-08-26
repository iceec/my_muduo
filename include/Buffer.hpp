#pragma once
#include <vector>
#include <string>
#include <algorithm>
class Buffer
{
public:
    Buffer(size_t init_capatity = DEFAULT_CAPATITY) : _buf(init_capatity + CheapPrepend), _r_index(CheapPrepend), _w_index(CheapPrepend) {}

    size_t readable_bytes() noexcept { return _w_index - _r_index; }
    size_t writeable_bytes() noexcept { return _buf.size() - _w_index; }

    // begin() 返回起始的地址
    char *begin() { return &(*_buf.begin()); }

    void pop(size_t len)
    {
        size_t max_can_be_pop = readable_bytes();

        if (len < max_can_be_pop)
            _r_index += len;
        else
            pop_all();
    }

    void pop_all() { _r_index = _w_index = CheapPrepend; }

    std::string pop_as_string(size_t len)
    {
        std::string result(begin()+_r_index, std::min(readable_bytes(), len));
        pop(len);
        return result;
    }

    std::string pop_all_as_string() { return pop_as_string(readable_bytes()); }

    void ensure_writeable_bytes(size_t size)
    {
        // 本身的容量是够的 不需要扩容
        if (writeable_bytes() >= size)
            return;

        make_space(size - writeable_bytes());
    }

    void append(const char *buf, size_t len)
    {
        ensure_writeable_bytes(len);

        std::copy(buf, buf + len, begin() + _w_index);
        _w_index += len;
    }

    size_t read_fd(int fd, int &error);

    size_t write_fd(int fd, int &error);

private:
    void make_space(size_t size)
    {
        if (size <= _r_index - CheapPrepend)
        {
            // 后拷贝防止重叠
            std::copy_backward(_buf.begin() + _r_index, _buf.begin() + _w_index, _buf.begin() + CheapPrepend);
            return;
        }

        size_t capatity = _buf.size();

        // 需要进行底下的扩容了  这一步不一定buf真的会扩容 因为很可能小于capa
        _buf.resize(capatity + size);
    }
    static constexpr size_t DEFAULT_CAPATITY = 1024;
    static constexpr size_t CheapPrepend = 8;
    std::vector<char> _buf;
    size_t _r_index;
    size_t _w_index;
};