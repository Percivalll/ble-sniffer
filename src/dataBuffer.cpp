#include <dataBuffer.h>
dataBuffer::dataBuffer(int size)
{
    cellSize = size;
    buffer = new int16_t *[cellSize];
    for (int i = 0; i < cellSize; i++)
    {
        buffer[i] = new int16_t[LEN_BUF];
    }
    empty = new Semaphore{cellSize};
    full = new Semaphore{0};
    writeIndex = 0;
    readIndex = 0;
}
dataBuffer::~dataBuffer()
{
    for (int i = 0; i < cellSize; i++)
    {
        delete buffer[i];
    }
    delete buffer;
    delete empty;
    delete full;
}
// Must Write A Full Cell.
int dataBuffer::write(int16_t *input)
{
    empty->wait();
    int index = writeIndex;
    // std::cout << "Write:" << index << std::endl;
    writeIndex = (writeIndex + 1) % cellSize;
    for (int i = 0; i < LEN_BUF; i++)
        buffer[index][i] = input[i];
    full->notify();
}

int dataBuffer::read(int (*function)(int16_t *input))
{
    full->wait();
    int index=readIndex;
    // std::cout << "Read:" << readIndex << std::endl;
    lock.lock();
    readIndex = (readIndex + 1) % cellSize;
    lock.unlock();
    function(buffer[index]);
    empty->notify();
}