#include "Buffer.hpp"
#include <sys/uio.h>
#include <unistd.h>
static constexpr int DEFAULT_STACK_BUF = 65536;
// 确保fd 是打开的 并且是非阻塞的
// 你要读取东西 那么就一定 是往buffer里面写
size_t
Buffer::read_fd(int fd, int &error)
{

    char buf[DEFAULT_STACK_BUF]{0};

    struct iovec vec[2];

    size_t writeable = writeable_bytes();

    vec[0].iov_base = &(*_buf.begin()) + _w_index;

    vec[0].iov_len = writeable;

    vec[1].iov_base = buf;

    vec[1].iov_len = sizeof(buf) - 1;

    // why ????
    const int iovcnt = (writeable_bytes() < sizeof(buf)) ? 2 : 1;
    size_t n = ::readv(fd, vec, iovcnt);

    if (n < 0)
    {
        error = errno;
    }
    else
    {
        if (n <= writeable)
        {
            _w_index += n;
        }
        else
        {
            _w_index = _buf.size();
            append(buf, n - writeable);
        }
    }

    return n;
}

// 要向外面写 就是从buffer当中读出数据
// 这个时候应该调整整个的r_index
size_t
Buffer::write_fd(int fd, int &error)
{
    size_t n = ::write(fd, begin() + _r_index, readable_bytes());

    if (n < 0)
    {
        error = errno;
    }
    else
    {
        pop(n);
    }

    return n;
}