#include "bladerfDriver.h"
#include <iostream>
#include <boost/circular_buffer.hpp>
using namespace std;
int32_t rxbuffer[LEN_BUF];
int main()
{
     struct bladerf *bladerfDev = bladerfDriver::setBoard();
     if (bladerfDev == NULL)
          return -1;
    struct bladerfDriver::bladerf_data *bladerfData=new  bladerfDriver::bladerf_data ;
     bladerfData->idx = 0;
     bladerfData->num_buffers = 2;
     bladerfData->samples_per_buffer = LEN_BUF_IN_SAMPLE;
     struct bladerf_stream *bladerfStream ;
     bladerfStream=bladerfDriver::configureStream(bladerfDev, bladerfStream,bladerfData);
     // bladerf_stream(bladerfStream,BLADERF_RX_X1);
     std::thread streaming(bladerf_stream, bladerfStream, BLADERF_RX_X1);
     bladerf_deinit_stream(bladerfStream);
     bladerf_close(bladerfDev);
}