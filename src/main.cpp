#include <bladerfDriver.h>
#include <iostream>
#include <global.h>
#include <thread>
#include <demodMoudle.h>
#include <boost/crc.hpp>
using namespace std;
bool signalExit = false;
struct bladerf *sgdev;
struct bladerf_stream *sgstream;
// ___________________________Unit Test Data Buffer -Busy waiting  _________________________________________
dataBuffer *rxbuf = new dataBuffer{3};
// ____________________CRC________________________________
int main()
{
     // ___________________________Unit Test BladeRF Driver_________________________________________
     // struct bladerf *bladerfDev = bladerfDriver::setBoard();
     // if (bladerfDev == NULL)
     //      return -1;
     // struct bladerfDriver::bladerf_data *bladerfData = new bladerfDriver::bladerf_data;
     // bladerfData->idx = 0;
     // bladerfData->num_buffers = 10;
     // bladerfData->samples_per_buffer = LEN_BUF_IN_SAMPLE;
     // struct bladerf_stream *bladerfStream;
     // bladerfStream = bladerfDriver::configureStream(bladerfDev, bladerfStream, bladerfData);
     // // bladerf_stream(bladerfStream,BLADERF_RX_X1);
     // std::thread streaming(bladerf_stream, bladerfStream, BLADERF_RX_X1);
     // sgdev=bladerfDev;
     // sgstream=bladerfStream;
     // signal(SIGINT,bladerfDriver::closeBoard);
     // streaming.join();
     // cout<<"Program has been exited."<<endl;
     // return 0;
     // ___________________________Unit Test Data Buffer_________________________________________
     // dataBuffer buffer{3};
     // std::thread p1(test,&buffer);
     // std::thread p2(test,&buffer);
     // for(int i=0;i<20;i++)
     // buffer.write();
     // if(p1.joinable())
     // p1.join();
     // if(p2.joinable())
     // p2.join();
     // ___________________________Unit Test Demod Module_________________________________________

     // FILE *fp = fopen("/home/zhanglei/BleFpSniffer/Binary", "rb");
     // int16_t *inputData = new int16_t[LEN_BUF];
     // int res=0;
     // int statu;
     // while (statu != 0)
     // {
     //      statu = fread(inputData, sizeof(int16_t), LEN_BUF, fp);
     //      res=demod(inputData);
     // }
     // delete inputData;
     // uint8_t a[8]={0,1,1,1,1,1,0,1};
     // uint8_t b=0;
     // for(int i=0;i<8;i++)
     // {
     //      b=b|a[i]<<i;
     // }
     // printf("%d\n",b);
     // // ___________________________Union Test BladerfDriver-Buffer-Demod_________________________________________
     struct bladerf *bladerfDev = bladerfDriver::setBoard();
     if (bladerfDev == NULL)
          return -1;
     struct bladerfDriver::bladerf_data *bladerfData = new bladerfDriver::bladerf_data;
     bladerfData->idx = 0;
     bladerfData->num_buffers = 2;
     bladerfData->samples_per_buffer = LEN_BUF_IN_SAMPLE;
     struct bladerf_stream *bladerfStream;
     bladerfStream = bladerfDriver::configureStream(bladerfDev, bladerfStream, bladerfData);
     sgdev = bladerfDev;
     sgstream = bladerfStream;
     signal(SIGINT, bladerfDriver::closeBoard);
     std::thread streaming(bladerf_stream, bladerfStream, BLADERF_RX_X1);
     while (!signalExit)
          rxbuf->read(demod);
     streaming.join();
     cout << "Program has been exited." << endl;
     return 0;
}