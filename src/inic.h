#pragma once

#include "game.h"

#define TYPE_UINT8      0
#define TYPE_UINT16     1
#define TYPE_UINT32     2
#define TYPE_UINT64     3
#define TYPE_INT8       4
#define TYPE_INT16      5
#define TYPE_INT32      6
#define TYPE_INT64      7
#define TYPE_FLOAT      8
#define TYPE_STRING     9

struct inic_datas
{
    char* name;     // name of the data
    void* data;     // pointer to the data
    int type;       // type of the data
    int size;       // size of the data, for strings
    uint8_t found;  // 0 = not found, 1 = found
};

struct inics
{
    char* path;
    FILE* fd;
};


int inic_init(struct inics* ics, char* path);
int inic_readSection(struct inics* ics, char* section);
int inic_readCsv(struct inics* ics, struct inic_datas* datas, int nbDatas);
int inic_readIni(struct inics* ics, struct inic_datas* datas, int nbDatas);
int inic_close(struct inics* ics);

