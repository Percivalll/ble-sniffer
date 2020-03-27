#include "demodulator.h"
Demodulator::Demodulator(int samplerate)
{
    mSampleRate=samplerate/1e6;
    mDatabase=QSqlDatabase::addDatabase("QSQLITE","toWrite");
    mDatabase.setDatabaseName("packetDB");
    mDatabase.open();
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
void Demodulator::transRaw2Image(int16_t *data, int16_t *image, int length)
{
    int dlength=length-80;
    int32_t I[dlength/2],Q[dlength/2],maxI=0,minI=0,maxQ=0,minQ=0;
    int16_t maxPoints=0;
    for(int i=0;i<dlength;i+=2)
    {
        int16_t a=data[80+i];
        int16_t b=data[81+i];
        int16_t c=data[i];
        int16_t d=data[i+1];
        I[i/2]=a*c+b*d;
        Q[i/2]=b*c-a*d;
        maxI=(maxI>I[i/2])?maxI:I[i/2];
        minI=(minI<I[i/2])?minI:I[i/2];
        maxQ=(maxQ>Q[i/2])?maxQ:Q[i/2];
        minQ=(minQ<Q[i/2])?minQ:Q[i/2];
    }
    double xGap=(maxI-minI)/31,yGap=(maxQ-minQ)/31;
    memset(image,0,32*32*sizeof(int16_t));
    for(int i=0;i<dlength/2;i++)
    {
        image[int((I[i]-minI)/xGap)*32+int((Q[i]-minQ)/yGap)]++;
        maxPoints=(maxPoints>image[int((I[i]-minI)/xGap)*32+int((Q[i]-minQ)/yGap)])?maxPoints:image[int((I[i]-minI)/xGap)*32+int((Q[i]-minQ)/yGap)];
    }
    for(int i=0;i<32*32;i++)
        image[i]=image[i]*255/maxPoints;
    FILE *fp=fopen("file","wb");
    fwrite(image,2,32*32,fp);
    fclose(fp);
    FILE *fp1=fopen("file1","wb");
    fwrite(I,2,dlength/2,fp1);
    fclose(fp1);
    FILE *fp2=fopen("file2","wb");
    fwrite(Q,2,dlength/2,fp2);
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
                    int16_t image[32*32];
                    transRaw2Image(samples+i-47*8*20,image,(bytes[1]+10)*8*20);
                    BlePacket pkt(bytes[0]&0x0F,bytes[1],bytes+2,image,checkValue);
                    QSqlQuery query(mDatabase);
                    QString sql=QString("insert into packetTable (adva,time,type,length,image,crc) "
                                        "values ('%1','%2','%3','%4',:img,'%5')")
                                        .arg(pkt.mAdva).arg(pkt.mClock).arg(pkt.mPDUType).arg(pkt.mPDULength).arg(pkt.mCRCCheck);
                    query.prepare(sql);
                    query.bindValue(":img",pkt.mImage);
                    query.exec();
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
