#include <demodMoudle.h>
#include <global.h>
#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
int findPacketHead(uint8_t *afterDemodData)
{
    uint8_t byte[LEN_BUF_IN_SAMPLE / (10 * 8)]; //10M Sampling Rate and 8 bits Per Byte.
    int byteIndex = 0;
    uint8_t mask[5] = {0xAA, 0xD6, 0xBE, 0x89, 0x8E};
    for (int i = 0; i < 40 * 10;)
    {
        byte[byteIndex] = 0;
        for (int j = 0; j < 8; j++)
        {
            byte[byteIndex] = byte[byteIndex] | (afterDemodData[i] << j);
            i = i + 10;
        }
        if (byte[byteIndex] != mask[byteIndex])
        {
            return -1;
        }
        byteIndex++;
    }
    return 0;
}
int demod(int16_t *inputData)
{
    static int count = 0;
    int16_t I0, Q0, I1, Q1;
    int bitDecision = 0;
    uint8_t bit[LEN_BUF_IN_SAMPLE];
    int bitIndex = 0;
    timeval clocks, clockf;
                            gettimeofday(&clocks, NULL);
    for (int i = 0; i < LEN_BUF;)
    {
        I0 = inputData[i];
        Q0 = inputData[i + 1];
        I1 = inputData[i + 2];
        Q1 = inputData[i + 3];
        bitDecision = (I0 * Q1 - I1 * Q0) > 0 ? 1 : 0;
        bit[bitIndex] = bitDecision;
        // printf("%d",bitDecision);
        if (bitIndex > 40 * 10)
            if (findPacketHead(bit + bitIndex - 400) == 0)
            {
                count++;
                char *Filename = new char[10];
                sprintf(Filename, "./build/data/%d", count);
                int fp = open(Filename, O_WRONLY | O_CREAT, 00600);
                write(fp, (char *)(inputData + i - 800), sizeof(int16_t) * 47 * 2 * 10 * 8);
                close(fp);
                time_t now = time(0);
                char tmp[64];
                strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&now));
                std::cout << tmp << "   DemodMoudle: Catch a Ble Packet,Have Saved in " << Filename << std::endl;
                delete Filename;
                gettimeofday(&clockf, NULL);
                std::cout << 1000000 * (clockf.tv_sec - clocks.tv_sec) + (clockf.tv_usec - clocks.tv_usec) << std::endl;
            }
        bitIndex++;
        i += 2;
    }
    gettimeofday(&clockf, NULL);
    // std::cout << 1000000 * (clockf.tv_sec - clocks.tv_sec) + (clockf.tv_usec - clocks.tv_usec) << std::endl;
    return 0;
}