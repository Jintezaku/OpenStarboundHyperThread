#include <iostream>
#include <thread>
#include <vector>
#include "StarBuffer.hpp"

namespace Star {

class HyperBuffer : public Buffer {
public:
    HyperBuffer() : Buffer() {}
    HyperBuffer(size_t initialSize) : Buffer(initialSize) {}
    HyperBuffer(ByteArray b) : Buffer(std::move(b)) {}
    HyperBuffer(Buffer const& buffer) : Buffer(buffer) {}
    HyperBuffer(Buffer&& buffer) : Buffer(std::move(buffer)) {}

    // Parallel read function
    void parallelRead(char* data, size_t len, size_t numThreads) {
        size_t chunkSize = len / numThreads;
        std::vector<std::thread> threads;

        for (size_t i = 0; i < numThreads; ++i) {
            size_t startPos = i * chunkSize;
            size_t readLen = (i == numThreads - 1) ? (len - startPos) : chunkSize;
            threads.emplace_back(&HyperBuffer::readChunk, this, data + startPos, startPos, readLen);
        }

        for (auto& thread : threads) {
            thread.join();
        }
    }

    // Parallel write function
    void parallelWrite(char const* data, size_t len, size_t numThreads) {
        size_t chunkSize = len / numThreads;
        std::vector<std::thread> threads;

        for (size_t i = 0; i < numThreads; ++i) {
            size_t startPos = i * chunkSize;
            size_t writeLen = (i == numThreads - 1) ? (len - startPos) : chunkSize;
            threads.emplace_back(&HyperBuffer::writeChunk, this, data + startPos, startPos, writeLen);
        }

        for (auto& thread : threads) {
            thread.join();
        }
    }

private:
    void readChunk(char* data, size_t pos, size_t len) {
        std::unique_lock<std::mutex> lock(m_mutex);
        readAbsolute(pos, data, len);
    }

    void writeChunk(char const* data, size_t pos, size_t len) {
        std::unique_lock<std::mutex> lock(m_mutex);
        writeAbsolute(pos, data, len);
    }

    std::mutex m_mutex;
};

};
