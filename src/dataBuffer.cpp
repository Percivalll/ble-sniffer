#include <dataBuffer.h>
#include <iostream>
#include <thread>
dataBuffer::dataBuffer()
{
    emptySp = new Semaphore(3);
    fullSp = new Semaphore(0);
    buffer[0].status = EMPTY;
    buffer[1].status = EMPTY;
    buffer[2].status = EMPTY;
}
struct bufCell *dataBuffer::producer()
{
    emptySp->wait();
    // std::thread::id tid = std::this_thread::get_id();
    // std::cout << "w id=" << tid << std::endl;
    return findCell(EMPTY);
}
struct bufCell *dataBuffer::consumer()
{

    fullSp->wait();
    // std::thread::id tid = std::this_thread::get_id();
    // std::cout << "r id=" << tid << std::endl;
    return findCell(FULL);
}
struct bufCell *dataBuffer::findCell(enum bufStatus status)
{
    // std::thread::id tid = std::this_thread::get_id();
    // std::cout << "b id=" << tid << std::endl;
    if (buffer[0].status == status)
    {
        return buffer;
    }
    else if (buffer[1].status == status)
    {
        return buffer + 1;
    }
    else if (buffer[2].status == status)
    {
        return buffer + 2;
    }
    else
    {
        // std::cout << buffer[0].status << buffer[0].status << buffer[0].status << status;
        // throw "FuckC+++++++++++++++++++++++++++++!!!!\n";
    }
}