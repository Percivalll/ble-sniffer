#include <dataBuffer.h>
dataBuffer::dataBuffer()
{
    buffer[0].status=EMPTY;
    buffer[1].status=EMPTY;
    buffer[2].status=EMPTY;
}
struct bufCell *dataBuffer::write()
{
    struct bufCell *thisCell=findCell(EMPTY);
    return thisCell;
}
struct bufCell *dataBuffer::read()
{
    struct bufCell *thisCell=findCell(FULL);
    return thisCell;
}
struct bufCell *dataBuffer::findCell(enum bufStatus status)
{
    if (buffer[0].status == status)
        return buffer;
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
        return NULL;
    }
}