#include <bladerfDriver.h>
#include <iostream>
#include <boost/circular_buffer.hpp>
int32_t rxbuffer[LEN_BUF];
int main()
{
     struct bladerf *bladerfDev = bladerfDriver::setBoard();
     if (bladerfDev == NULL)
          return -1;
     struct bladerf_stream *bladerfStream = bladerfDriver::configureStream(bladerfDev,bladerfStream);
     bladerf_deinit_stream(bladerfStream);
     bladerf_close(bladerfDev);
}