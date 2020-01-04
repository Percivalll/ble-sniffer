#include "bladerfDriver.h"
#include <iostream>
int32_t rxbuffer[LEN_BUF];
int main()
{
     struct bladerf * bladerfDev=bladerfDriver::setBoard();
     if(bladerfDev==NULL)
     return -1;
     bladerfDriver::configureStream(bladerfDev);
}