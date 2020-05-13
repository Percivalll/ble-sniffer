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
void Demodulator::transRaw2Image(int16_t *data,std::vector<unsigned char> &img, int bitlength)
{
    FILE *fp=fopen("file","wb");
    fwrite(data,sizeof (int16_t),bitlength,fp);
    fclose(fp);
    std::vector<unsigned char> pngbuffer;
    int16_t image_20[32*32],image_25[32*32],image_30[32*32];
    int max20,max25,max30;
    diffPicture(data,image_20,max20,bitlength,20);
    diffPicture(data,image_25,max25,bitlength,25);
    diffPicture(data,image_30,max30,bitlength,30);
    for(int i=0;i<32*32;i++)
    {
        pngbuffer.push_back(255-image_20[i]*255/max20);
        pngbuffer.push_back(255-image_25[i]*255/max25);
        pngbuffer.push_back(255-image_30[i]*255/max30);
    }
    lodepng::State state;
    state.info_raw.colortype=LCT_RGB;
    state.info_raw.bitdepth=8;
    state.info_png.color.colortype=LCT_RGB;
    state.info_png.color.bitdepth=8;
    state.encoder.auto_convert=0;
    lodepng::encode(img,pngbuffer,32,32,state);
}
void Demodulator::diffPicture(int16_t*data,int16_t *image,int &maxPoints,int length,int interval)
{
    int dlength=length-interval*2;
    int32_t I[dlength/2],Q[dlength/2],maxI=0,minI=0,maxQ=0,minQ=0;
    for(int i=0;i<dlength;i+=2)
    {
        int16_t a=data[interval*2+i];
        int16_t b=data[interval*2+1+i];
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
}
int Demodulator::setBuffer(DataBuffer *bufffer)
{
    mSamplesBuffer=bufffer;
    return 0;
}
int Demodulator::startDemod()
{
    static int sqls=0;
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
                    std::vector<unsigned char>image;
                    transRaw2Image(samples+i-47*8*20,image,(bytes[1]+10)*8*20);
                    BlePacket pkt(bytes[0]&0x0F,bytes[1],bytes+2,image,checkValue);
                    if(sqls%10==0)
                    {
                        if(sqls!=0)
                        mDatabase.commit();
                        mDatabase.transaction();
                    }
                    QSqlQuery query(mDatabase);
                    QString sql=QString("insert into packetTable (adva,time,type,length,image,crc) "
                                        "values ('%1','%2','%3','%4',:img,'%5')")
                            .arg(pkt.mAdva).arg(pkt.mClock).arg(pkt.mPDUType).arg(pkt.mPDULength).arg(pkt.mCRCCheck);
                    query.prepare(sql);
                    query.bindValue(":img",pkt.mImage);
                    query.exec();
                    sqls++;
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
