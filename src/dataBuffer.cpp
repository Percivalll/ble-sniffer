#include <dataBuffer.h>
dataBuffer::dataBuffer()
{

}
struct bufCell* dataBuffer::write()
{
    findCell(FREE);
}
struct bufCell* dataBuffer::read()
{
    findCell(FREE);
}
struct bufCell * dataBuffer::findCell(enum bufStatus status)
{
    if (buffer[0].status==status)
        return buffer;
    else if (buffer[1].status==status)
    {
        return buffer+1;
    }
    else if (buffer[2].status==status)
    {
        return buffer+2;
    }
    else
    {
        return NULL;
    }
}