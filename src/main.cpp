#include "bladerfDriver.h"
#include <iostream>
#include <global.h>
using namespace std;
int main()
{
     boost::circular_buffer<int16_t> rxbuffer(LEN_BUF);
     struct bladerf *bladerfDev = bladerfDriver::setBoard();
     if (bladerfDev == NULL)
          return -1;
     struct bladerfDriver::bladerf_data *bladerfData = new bladerfDriver::bladerf_data;
     bladerfData->idx = 0;
     bladerfData->num_buffers = 2;
     bladerfData->samples_per_buffer = LEN_BUF_IN_SAMPLE;
     struct bladerf_stream *bladerfStream;
     bladerfStream = bladerfDriver::configureStream(bladerfDev, bladerfStream, bladerfData);
     // bladerf_stream(bladerfStream,BLADERF_RX_X1);
     std::thread streaming(bladerf_stream, bladerfStream, BLADERF_RX_X1);
     while(1)
     cout<<rxbuffer.size()<<endl;
     bladerf_deinit_stream(bladerfStream);
     bladerf_close(bladerfDev);
     return 0;
}