#include <bladerfDriver.h>
#include <iostream>
int main()
{
     struct bladerf * bladerfDev=bladerfDriver::setBoard();
     if(bladerfDev==NULL)
     return -1;
     
}