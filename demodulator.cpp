#include "demodulator.h"
Demodulator::Demodulator(int samplerate)
{
    mSampleRate=samplerate/1e6;
};
uint_fast32_t Demodulator::CRCUpdate(uint_fast32_t crc, const void *data, size_t data_len)
{
    const unsigned char *d = (const unsigned char *)data;
    unsigned int tbl_idx;

    while (data_len--)
    {
        tbl_idx = (crc ^ *d) & 0xff;
        crc = (mCRCTable[tbl_idx] ^ (crc >> 8)) & 0xffffff;

        d++;
    }
    return crc & 0xffffff;
}
uint_fast32_t Demodulator::CRC24Byte(uint8_t *byte_in, int num_byte, uint32_t init_hex)
{
    uint_fast32_t crc = init_hex;

    crc = CRCUpdate(crc, byte_in, num_byte);

    return (crc);
}
bool Demodulator::CRCCheck(uint8_t *tmp_byte, int body_len, uint32_t crc_init)
{
    int crc24_checksum, crc24_received;                        //, i;
    crc24_checksum = CRC24Byte(tmp_byte, body_len, crc_init); // 0x555555 --> 0xaaaaaa. maybe because byte order
    crc24_received = 0;
    crc24_received = ((crc24_received << 8) | tmp_byte[body_len + 2]);
    crc24_received = ((crc24_received << 8) | tmp_byte[body_len + 1]);
    crc24_received = ((crc24_received << 8) | tmp_byte[body_len + 0]);
    return (crc24_checksum == crc24_received);
}
void Demodulator::transBit2Byte(uint8_t* bits,uint8_t* bytes,int length)
{
    int byteIndex=0;
    for (int i = 0; i < length;)
    {
        bytes[byteIndex] = 0;
        for (int j = 0; j < 8; j++)
        {
            bytes[byteIndex] = bytes[byteIndex] | (bits[i] << j);
            i++;
        }
        byteIndex++;
    }
}
int Demodulator::findPacketHead(uint8_t *bits)
{
    uint8_t bytes[5];
    transBit2Byte(bits,bytes,40);
    uint8_t mask[5] = {0xAA, 0xD6, 0xBE, 0x89, 0x8E};
    return memcmp(bytes,mask,5);
}
void Demodulator::createFgPrint(int16_t *data,int length)
{
    int16_t diff[length-80];
    int maxReal,minReal,maxImag,minImag;
    int fgPrint[32][32];
    memset(fgPrint,0,sizeof(fgPrint));
    for(int i=0;i<length-80;i+=2)
    {
        //        (a+bi)(c+di).conj()=(ac+bd)+(bc-ad)i
        diff[i]=data[i+80]*data[i]+data[i+81]*data[i+1];
        diff[i+1]=data[i+81]*data[i]-data[i+80]*data[i+1];

        if(diff[i]>maxReal)
            maxReal=diff[i];
        if(diff[i]<minReal)
            minReal=diff[i];
        if(diff[i+1]>maxImag)
            maxImag=diff[i+1];
        if(diff[i+1]<minImag)
            minImag=diff[i+1];
    }
//    double xaxisGap=(maxReal-minReal)/31.0;
//    double yaxisGap=(maxImag-minImag)/31.0;
//    int printMax;
//    for(int i=0;i<length-40;i++)
//    {
//        int x=int((dataReal[i]-minReal)/xaxisGap);
//        int y=int((dataImag[i]-minImag)/yaxisGap);
//        qDebug()<<x<<y;
//        fgPrint[x][y]++;
//        if(printMax<fgPrint[x][y])
//            printMax=fgPrint[x][y];
//    }
    //    for(int i=0;i<32;i++)
    //        for(int j=0;j<32;j++)
    //            fgPrint[i][j]=(fgPrint[i][j]*255)/printMax;
    FILE *fp1=fopen("file1","wb");
    fwrite(diff,sizeof (int16_t),length-40,fp1);
    fclose(fp1);
    FILE *fp2=fopen("file2","wb");
    fwrite(data,sizeof (int16_t),length,fp2);
    fclose(fp2);
}
int Demodulator::setBuffer(DataBuffer *bufffer)
{
    mSamplesBuffer=bufffer;
    return 0;
}
int Demodulator::startDemod()
{
    mRunFlag=true;
    while(mRunFlag==true)
    {
        int16_t samples[mSamplesBuffer->getLength()];
        mSamplesBuffer->read(samples);
        int16_t I0, Q0, I1, Q1;
        int bitDecision = 0;
        uint8_t bits[mSamplesBuffer->getLength()/20];
        int bitIndex = 0;
        for(int i=0;i<mSamplesBuffer->getLength();)
        {
            I0 = samples[i];
            Q0 = samples[i + 1];
            I1 = samples[i + 2];
            Q1 = samples[i + 3];
            bitDecision = (I0 * Q1 - I1 * Q0) > 0 ? 1 : 0;
            bits[bitIndex] = bitDecision;
            if ((bitIndex > 47 * 8)&&(findPacketHead(bits + bitIndex - 47 * 8) == 0))
            {
                uint8_t bytes[42];
                transBit2Byte(bits+bitIndex-42*8,bytes,42*8);
                for(int index=0;index<42;index++)
                    bytes[index]^=mScrambleTableCh37[index];
                bool checkValue = CRCCheck(bytes, bytes[1] + 2, 0xaaaaaa);
                if(checkValue==true)
                {
                    createFgPrint(samples+i-47*8*20,(bytes[1]+10)*8*20);
                    Q_EMIT receivedPacket(BlePacket(bytes[0]&0x0F,bytes[1],bytes+2,samples+i-47*8*20,checkValue));
                }
                else
                {
                }
            }
            bitIndex++;
            i=i+20;
        }
    }
    return 0;
}
