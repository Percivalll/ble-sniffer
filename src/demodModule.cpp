#include <demodMoudle.h>
#include <global.h>
#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include <scrambleTableCh37.h>
static const uint_fast32_t crc_table[256] = {
    0x000000, 0x01b4c0, 0x036980, 0x02dd40, 0x06d300, 0x0767c0, 0x05ba80, 0x040e40,
    0x0da600, 0x0c12c0, 0x0ecf80, 0x0f7b40, 0x0b7500, 0x0ac1c0, 0x081c80, 0x09a840,
    0x1b4c00, 0x1af8c0, 0x182580, 0x199140, 0x1d9f00, 0x1c2bc0, 0x1ef680, 0x1f4240,
    0x16ea00, 0x175ec0, 0x158380, 0x143740, 0x103900, 0x118dc0, 0x135080, 0x12e440,
    0x369800, 0x372cc0, 0x35f180, 0x344540, 0x304b00, 0x31ffc0, 0x332280, 0x329640,
    0x3b3e00, 0x3a8ac0, 0x385780, 0x39e340, 0x3ded00, 0x3c59c0, 0x3e8480, 0x3f3040,
    0x2dd400, 0x2c60c0, 0x2ebd80, 0x2f0940, 0x2b0700, 0x2ab3c0, 0x286e80, 0x29da40,
    0x207200, 0x21c6c0, 0x231b80, 0x22af40, 0x26a100, 0x2715c0, 0x25c880, 0x247c40,
    0x6d3000, 0x6c84c0, 0x6e5980, 0x6fed40, 0x6be300, 0x6a57c0, 0x688a80, 0x693e40,
    0x609600, 0x6122c0, 0x63ff80, 0x624b40, 0x664500, 0x67f1c0, 0x652c80, 0x649840,
    0x767c00, 0x77c8c0, 0x751580, 0x74a140, 0x70af00, 0x711bc0, 0x73c680, 0x727240,
    0x7bda00, 0x7a6ec0, 0x78b380, 0x790740, 0x7d0900, 0x7cbdc0, 0x7e6080, 0x7fd440,
    0x5ba800, 0x5a1cc0, 0x58c180, 0x597540, 0x5d7b00, 0x5ccfc0, 0x5e1280, 0x5fa640,
    0x560e00, 0x57bac0, 0x556780, 0x54d340, 0x50dd00, 0x5169c0, 0x53b480, 0x520040,
    0x40e400, 0x4150c0, 0x438d80, 0x423940, 0x463700, 0x4783c0, 0x455e80, 0x44ea40,
    0x4d4200, 0x4cf6c0, 0x4e2b80, 0x4f9f40, 0x4b9100, 0x4a25c0, 0x48f880, 0x494c40,
    0xda6000, 0xdbd4c0, 0xd90980, 0xd8bd40, 0xdcb300, 0xdd07c0, 0xdfda80, 0xde6e40,
    0xd7c600, 0xd672c0, 0xd4af80, 0xd51b40, 0xd11500, 0xd0a1c0, 0xd27c80, 0xd3c840,
    0xc12c00, 0xc098c0, 0xc24580, 0xc3f140, 0xc7ff00, 0xc64bc0, 0xc49680, 0xc52240,
    0xcc8a00, 0xcd3ec0, 0xcfe380, 0xce5740, 0xca5900, 0xcbedc0, 0xc93080, 0xc88440,
    0xecf800, 0xed4cc0, 0xef9180, 0xee2540, 0xea2b00, 0xeb9fc0, 0xe94280, 0xe8f640,
    0xe15e00, 0xe0eac0, 0xe23780, 0xe38340, 0xe78d00, 0xe639c0, 0xe4e480, 0xe55040,
    0xf7b400, 0xf600c0, 0xf4dd80, 0xf56940, 0xf16700, 0xf0d3c0, 0xf20e80, 0xf3ba40,
    0xfa1200, 0xfba6c0, 0xf97b80, 0xf8cf40, 0xfcc100, 0xfd75c0, 0xffa880, 0xfe1c40,
    0xb75000, 0xb6e4c0, 0xb43980, 0xb58d40, 0xb18300, 0xb037c0, 0xb2ea80, 0xb35e40,
    0xbaf600, 0xbb42c0, 0xb99f80, 0xb82b40, 0xbc2500, 0xbd91c0, 0xbf4c80, 0xbef840,
    0xac1c00, 0xada8c0, 0xaf7580, 0xaec140, 0xaacf00, 0xab7bc0, 0xa9a680, 0xa81240,
    0xa1ba00, 0xa00ec0, 0xa2d380, 0xa36740, 0xa76900, 0xa6ddc0, 0xa40080, 0xa5b440,
    0x81c800, 0x807cc0, 0x82a180, 0x831540, 0x871b00, 0x86afc0, 0x847280, 0x85c640,
    0x8c6e00, 0x8ddac0, 0x8f0780, 0x8eb340, 0x8abd00, 0x8b09c0, 0x89d480, 0x886040,
    0x9a8400, 0x9b30c0, 0x99ed80, 0x985940, 0x9c5700, 0x9de3c0, 0x9f3e80, 0x9e8a40,
    0x972200, 0x9696c0, 0x944b80, 0x95ff40, 0x91f100, 0x9045c0, 0x929880, 0x932c40};
uint_fast32_t crc_update(uint_fast32_t crc, const void *data, size_t data_len)
{
    const unsigned char *d = (const unsigned char *)data;
    unsigned int tbl_idx;

    while (data_len--)
    {
        tbl_idx = (crc ^ *d) & 0xff;
        crc = (crc_table[tbl_idx] ^ (crc >> 8)) & 0xffffff;

        d++;
    }
    return crc & 0xffffff;
}
uint_fast32_t crc24_byte(uint8_t *byte_in, int num_byte, uint32_t init_hex)
{
    uint_fast32_t crc = init_hex;

    crc = crc_update(crc, byte_in, num_byte);

    return (crc);
}
bool crc_check(uint8_t *tmp_byte, int body_len, uint32_t crc_init)
{
    int crc24_checksum, crc24_received;                        //, i;
    crc24_checksum = crc24_byte(tmp_byte, body_len, crc_init); // 0x555555 --> 0xaaaaaa. maybe because byte order
    crc24_received = 0;
    crc24_received = ((crc24_received << 8) | tmp_byte[body_len + 2]);
    crc24_received = ((crc24_received << 8) | tmp_byte[body_len + 1]);
    crc24_received = ((crc24_received << 8) | tmp_byte[body_len + 0]);
    return (crc24_checksum != crc24_received);
}
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
int openPacket(uint8_t *afterDemodData)
{
    uint8_t byte[42];
    int index = 0;
    for (int i = 0; i < 42 * 10 * 8; index++)
    {
        byte[index] = 0;
        for (int j = 0; j < 8; j++)
        {
            byte[index] = byte[index] | afterDemodData[i] << j;
            i += 10;
        }
        byte[index] = byte[index] ^ scramble_table_ch37[index];
    }
    int length = byte[1] + 2;
    int checkValue = crc_check(byte, length, 0xaaaaaa);
    if (checkValue == 0)
    {
        time_t now = time(0);
        char tmp[64];
        strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&now));
        std::cout << tmp << "    ";
        std::cout << "CRC CHECK PASS.";
        switch (byte[0] & 0x0F)
        {
        case 0:
            std::cout << "|ADV_IND|";
            break;
        case 1:
            std::cout << "|ADV_DIRECT_IND|";
            break;
        case 2:
            std::cout << "|ADV_NONCONN_IND|";
            break;
        case 3:
            std::cout << "|SCAN_REQ|";
            break;
        case 4:
            std::cout << "|SCAN_RSP|";
            break;
        case 5:
            std::cout << "|CONNECT_REQ|";
            break;
        case 6:
            std::cout << "|ADV_SCAN_IND|";
            break;
        default:
            std::cout << "|RESERVED|";
        }
        std::cout << "RxAdType:" << ((byte[0] & 0x80) >> 7) << "|";
        std::cout << "TxAdType:" << ((byte[0] & 0x40) >> 6) << "|";
        std::cout << "PduLength:" << length - 2 << "|";
        std::cout << "AdvAdr:";
        for (int i = 2; i < 8; i++)
            printf("%x", byte[i]);
        std::cout << "|";
        std::cout << "AdvData:";
        for (int i = 8; i < byte[1] - 6; i++)
            printf("%x", byte[i]);
        std::cout << "|";
        std::cout << std::endl;
    }

    else
    {
        // time_t now = time(0);
        // char tmp[64];
        // strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&now));
        // std::cout << tmp << "    ";
        // std::cout << "CRC CHECK FAIL,Packet Have been Droped.";
    }
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
        if (bitIndex > 47 * 8 * 10)
            if (findPacketHead(bit + bitIndex - 47 * 8 * 10) == 0)
            {
                openPacket(bit + bitIndex - 42 * 8 * 10);
                // count++;
                // char *Filename = new char[10];
                // sprintf(Filename, "./build/data/%ds", count);
                // int fp = open(Filename, O_WRONLY | O_CREAT, 00600);
                // write(fp, (char *)(inputData + i - 47 * 8 * 10 * 2), sizeof(int16_t) * 47 * 2 * 10 * 8);
                // close(fp);
                // delete Filename;
                // gettimeofday(&clockf, NULL);
                // std::cout << 1000000 * (clockf.tv_sec - clocks.tv_sec) + (clockf.tv_usec - clocks.tv_usec) << std::endl;
            }
        bitIndex++;
        i += 2;
    }
    gettimeofday(&clockf, NULL);
    // std::cout << 1000000 * (clockf.tv_sec - clocks.tv_sec) + (clockf.tv_usec - clocks.tv_usec) << std::endl;
    return 0;
}