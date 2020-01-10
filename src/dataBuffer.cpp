#include <dataBuffer.h>
#include <iostream>
#include <thread>
dataBuffer::dataBuffer()
{
    buffer = new int16_t *[3];
    for (int i = 0; i < 3; i++)
    {
        buffer[i] = new int16_t[LEN_BUF];
        status[i] = 0;
    }
}
dataBuffer::~dataBuffer()
{
    for (int i = 0; i < 3; i++)
    {
        delete buffer;
    }
    delete buffer;
}
// Must Write A Full Cell.
int dataBuffer::write()
{
    std::unique_lock<std::mutex> thisLock(twLock);
    while (status[0] != 0 && status[1] != 0 && status[2] != 0)
        towrite.wait(thisLock);
    // Write
    int i = 0;
    for (; status[i] != 0; i++);
    std::cout<<"111"<<std::endl;
    status[i] = -1;
    // Do Something...
    status[i] = 1;
    // Singal to Read
    toread.notify_one();
}

int dataBuffer::read()
{
    std::unique_lock<std::mutex> thisLock(trLock);
    while (status[0] != 1 && status[1] != 1 && status[2] != 1)
        toread.wait(thisLock);
    // Read
    int i = 0;
    for (; status[i] != 1; i++);
    status[i] = -1;
    // Do Something...
    status[i] = 0;
    // Singal to Write
    towrite.notify_one();
}