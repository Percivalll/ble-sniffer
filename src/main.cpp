#include <bladerfDriver.h>
#include <iostream>
#include <global.h>
#include <thread>
#include <future>
#include <functional>
#include <demodMoudle.h>
using namespace std;
bool signalExit = false;
struct bladerf *sgdev;
struct bladerf_stream *sgstream;
// ___________________________Unit Test Data Buffer -Busy waiting  _________________________________________
void print_int(std::future<int16_t *> &fut)
{
     int16_t *x = fut.get() + 2;
     std::cout << "value: " << *x << '\n';
}
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
     // std::promise<int16_t*> pm;
     // std::future<int16_t*> fut=pm.get_future();
     // std::thread th1(print_int,std::ref(fut));
     // int16_t a[3]={3,2,1};
     // for(int i=0;i<2;i++)
     // pm.set_value(a);
     // th1.join();
     // return 0;
     // ___________________________Unit Test Demod Module_________________________________________

     FILE *fp = fopen("/home/zhanglei/BleFpSniffer/Binary", "rb");
     int16_t *inputData = new int16_t[LEN_BUF];
     int res=0;
     int statu;
     while (statu != 0)
     {
          statu = fread(inputData, sizeof(int16_t), LEN_BUF, fp);
          res=demod(inputData);
     }
     delete inputData;
     uint8_t a[8]={0,1,1,1,1,1,0,1};
     uint8_t b=0;
     for(int i=0;i<8;i++)
     {
          b=b|a[i]<<i;
     }
     printf("%d\n",b);
}