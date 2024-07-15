#ifndef HYPERBUFFER_HPP
#define HYPERBUFFER_HPP

#include <thread>
#include <vector>
#include <mutex>
#include "StarBuffer.hpp"

namespace Star {

class HyperBuffer : public Buffer {
public:
    HyperBuffer();
    HyperBuffer(size_t initialSize);
    HyperBuffer(ByteArray b);
    HyperBuffer(Buffer const& buffer);
    HyperBuffer(Buffer&& buffer);

    void parallelRead(char* data, size_t len, size_t numThreads);
    void parallelWrite(char const* data, size_t len, size_t numThreads);

private:
    void readChunk(char* data, size_t pos, size_t len);
    void writeChunk(char const* data, size_t pos, size_t len);

    std::mutex m_mutex;
};

}

#endif // HYPERBUFFER_HPP
