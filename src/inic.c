#include "inic.h"

/*
hybrid ini/csv parser
use ini syntax for section, data can be in either csv or ini format

# ini section
[section]
a=1
b=2
c=3

# csv section
[section]
a,b,c,d
1,2,3,4
*/

int inic_test()
{
    struct inics ics;

    uint8_t a, b, c, d;

    struct inic_datas datas[4] = {
        {"a", &a, TYPE_UINT8, 0, 0},
        {"b", &b, TYPE_UINT8, 0, 0},
        {"c", &c, TYPE_UINT8, 0, 0},
        {"d", &d, TYPE_UINT8, 0, 0}
    };

    if(inic_init(&ics, "test.inic") != 0){
        printf("inic_init");
        return -1;
    }

    if(inic_readSection(&ics, "[ini]") == 0){
        if(inic_readIni(&ics, datas, 4) == 0){
            printf("a=%d\n", *((uint8_t*)datas[0].data));
            printf("b=%d\n", *((uint8_t*)datas[1].data));
            printf("c=%d\n", *((uint8_t*)datas[2].data));
            printf("d=%d\n", *((uint8_t*)datas[3].data));
        }
    } else {
        printf("inic_section ini");
        return -2;
    }

    if(inic_readSection(&ics, "[csv]") == 0){
        while(inic_readCsv(&ics, datas, 4) == 0){
            printf("a=%d\n", *((uint8_t*)datas[0].data));
            printf("b=%d\n", *((uint8_t*)datas[1].data));
            printf("c=%d\n", *((uint8_t*)datas[2].data));
            printf("d=%d\n", *((uint8_t*)datas[3].data));
        }
    } else {
        printf("inic_section csv");
        return -3;
    }

    inic_writeSection(&ics, "[initt]");
    inic_writeIni(&ics, datas, 4);

    inic_close(&ics);

    fflush(stdout);

    return 0;
}

int inic_init(struct inics* ics, char* path)
{
    if(ics == NULL || path == NULL){
        return -1; // invalid parameters
    }

    ics->path = path;
    ics->fd = fopen(ics->path, "rw");
    if(ics->fd == NULL){
        return -2;  // file not found
    }

    return 0;
}

int inic_readSection(struct inics* ics, char* section)
{
    char line[256];

    if(ics == NULL || section == NULL){
        return -1;  // invalid parameters
    }

    if(ics->fd == NULL){
        ics->fd = fopen(ics->path, "r");
        if(ics->fd == NULL){
            return -2;  // file not found
        }
    }

    fseek(ics->fd, 0, SEEK_SET);

    while(fgets(line, 256, ics->fd)){
        if(line[0] == '['){
            if(strncmp(line, section, strlen(section)) == 0){
                return 0;
            }
        }
    }
    return -3; // section not found
}

// read a single csv line
int inic_readCsv(struct inics* ics, struct inic_datas* datas, int nbDatas)
{
    char line[256];
    char* token;
    int i = 0;

    if(ics == NULL || datas == NULL || nbDatas == 0){
        return -1; // invalid parameters
    }
    
    while(fgets(line, 256, ics->fd)){
        if(line[0] == '['){
            return 0; // new section
        } else if(line[0] == '#' || line[0] == '\n' || line[0] == '\r'){
            continue; // skip empty line or comment
        } else {
            token = strtok(line, ",");
            while(token != NULL && i < nbDatas){
                datas[i].found = 1;
                switch(datas[i].type){
                    case TYPE_UINT8:
                        *((uint8_t*)datas[i].data) = atoi(token);
                        break;
                    case TYPE_UINT16:
                        *((uint16_t*)datas[i].data) = atoi(token);
                        break;
                    case TYPE_UINT32:
                        *((uint32_t*)datas[i].data) = atoi(token);
                        break;
                    case TYPE_UINT64:
                        *((uint64_t*)datas[i].data) = atoi(token);
                        break;
                    case TYPE_INT8:
                        *((int8_t*)datas[i].data) = atoi(token);
                        break;
                    case TYPE_INT16:
                        *((int16_t*)datas[i].data) = atoi(token);
                        break;
                    case TYPE_INT32:
                        *((int32_t*)datas[i].data) = atoi(token);
                        break;
                    case TYPE_INT64:
                        *((int64_t*)datas[i].data) = atoi(token);
                        break;
                    case TYPE_FLOAT:
                        *((float*)datas[i].data) = atof(token);
                        break;
                    case TYPE_STRING:
                        strncpy((char*)datas[i].data, token, datas[i].size);
                        break;
                }
                i++;
                token = strtok(NULL, ",");
            }
            return 0; // all datas read
        }
    }
    return 1; // error or end of file
}

// read a single ini section
int inic_readIni(struct inics* ics, struct inic_datas* datas, int nbDatas)
{
    char line[256];
    char* token;
    int i = 0;

    if(ics == NULL || datas == NULL || nbDatas == 0){
        return -1; // invalid parameters
    }

    while(fgets(line, 256, ics->fd)){
        if(line[0] == '['){
            return 0; // new section
        } else if(line[0] == '#' || line[0] == '\n' || line[0] == '\r'){
            continue; // skip empty line or comment
        }
        token = strtok(line, "=");
        if(token != NULL){
            for(i = 0; i < nbDatas; i++){
                if(strncmp(token, datas[i].name, strlen(datas[i].name)) == 0){
                    datas[i].found = 1;
                    token = strtok(NULL, "=");
                    switch(datas[i].type){
                        case TYPE_UINT8:
                            *((uint8_t*)datas[i].data) = atoi(token);
                            break;
                        case TYPE_UINT16:
                            *((uint16_t*)datas[i].data) = atoi(token);
                            break;
                        case TYPE_UINT32:
                            *((uint32_t*)datas[i].data) = atoi(token);
                            break;
                        case TYPE_UINT64:
                            *((uint64_t*)datas[i].data) = atoi(token);
                            break;
                        case TYPE_INT8:
                            *((int8_t*)datas[i].data) = atoi(token);
                            break;
                        case TYPE_INT16:
                            *((int16_t*)datas[i].data) = atoi(token);
                            break;
                        case TYPE_INT32:
                            *((int32_t*)datas[i].data) = atoi(token);
                            break;
                        case TYPE_INT64:
                            *((int64_t*)datas[i].data) = atoi(token);
                            break;
                        case TYPE_FLOAT:
                            *((float*)datas[i].data) = atof(token);
                            break;
                        case TYPE_STRING:
                            strncpy((char*)datas[i].data, token, datas[i].size);
                            break;
                    }
                }
            }
        }
    }
    return 1; // end of file
}

int inic_close(struct inics* ics)
{
    if(ics == NULL){
        return -1; // invalid parameters
    }

    if(ics->fd != NULL){
        fclose(ics->fd);
        ics->fd = NULL;
    }
    return 0;
}

int inic_writeSection(struct inics* ics, char* section)
{
    if(ics == NULL || section == NULL){
        return -1; // invalid parameters
    }

    if(ics->fd == NULL){
        ics->fd = fopen(ics->path, "w");
        if(ics->fd == NULL){
            return -2;  // file not found
        }
    }

    fprintf(ics->fd, "\n[%s]\n", section);
    fflush(ics->fd);

    return 0;
}

int inic_writeIni(struct inics* ics, struct inic_datas* datas, int nbDatas)
{
    int i;

    if(ics == NULL || datas == NULL || nbDatas == 0){
        return -1; // invalid parameters
    }

    if(ics->fd == NULL){
        ics->fd = fopen(ics->path, "w");
        if(ics->fd == NULL){
            return -2;  // file not found
        }
    }

    for(i = 0; i < nbDatas; i++){
        switch(datas[i].type){
            case TYPE_UINT8:
                fprintf(ics->fd, "%s=%u\n", datas[i].name, *((uint8_t*)datas[i].data));
                break;
            case TYPE_UINT16:
                fprintf(ics->fd, "%s=%u\n", datas[i].name, *((uint16_t*)datas[i].data));
                break;
            case TYPE_UINT32:
                fprintf(ics->fd, "%s=%u\n", datas[i].name, *((uint32_t*)datas[i].data));
                break;
            case TYPE_UINT64:
                #ifdef __EMSCRIPTEN__
                    fprintf(ics->fd, "%s=%llu\n", datas[i].name, *((uint64_t*)datas[i].data));
                #else
                    fprintf(ics->fd, "%s=%lu\n", datas[i].name, *((uint64_t*)datas[i].data));
                #endif
                break;
            case TYPE_INT8:
                fprintf(ics->fd, "%s=%d\n", datas[i].name, *((int8_t*)datas[i].data));
                break;
            case TYPE_INT16:
                fprintf(ics->fd, "%s=%d\n", datas[i].name, *((int16_t*)datas[i].data));
                break;
            case TYPE_INT32:
                fprintf(ics->fd, "%s=%d\n", datas[i].name, *((int32_t*)datas[i].data));
                break;
            case TYPE_INT64:
                #ifdef __EMSCRIPTEN__
                    fprintf(ics->fd, "%s=%lld\n", datas[i].name, *((int64_t*)datas[i].data));
                #else
                    fprintf(ics->fd, "%s=%lu\n", datas[i].name, *((uint64_t*)datas[i].data));
                #endif
                break;
            case TYPE_FLOAT:
                fprintf(ics->fd, "%s=%f\n", datas[i].name, *((float*)datas[i].data));
                break;
            case TYPE_STRING:
                fprintf(ics->fd, "%s=%s\n", datas[i].name, (char*)datas[i].data);
                break;
        }
        fflush(ics->fd);
    }

    return 0;
}

int inic_writeCsv(struct inics* ics, struct inic_datas* datas, int nbDatas)
{
    int i;

    if(ics == NULL || datas == NULL || nbDatas == 0){
        return -1; // invalid parameters
    }

    if(ics->fd == NULL){
        ics->fd = fopen(ics->path, "w");
        if(ics->fd == NULL){
            return -2;  // file not found
        }
    }

    for(i = 0; i < nbDatas; i++){
        switch(datas[i].type){
            case TYPE_UINT8:
                fprintf(ics->fd, "%u", *((uint8_t*)datas[i].data));
                break;
            case TYPE_UINT16:
                fprintf(ics->fd, "%u", *((uint16_t*)datas[i].data));
                break;
            case TYPE_UINT32:
                fprintf(ics->fd, "%u", *((uint32_t*)datas[i].data));
                break;
            case TYPE_UINT64:
                #ifdef __EMSCRIPTEN__
                    fprintf(ics->fd, "%llu", *((uint64_t*)datas[i].data));
                #else
                    fprintf(ics->fd, "%lu", *((uint64_t*)datas[i].data));
                #endif
                break;
            case TYPE_INT8:
                fprintf(ics->fd, "%d", *((int8_t*)datas[i].data));
                break;
            case TYPE_INT16:
                fprintf(ics->fd, "%d", *((int16_t*)datas[i].data));
                break;
            case TYPE_INT32:
                fprintf(ics->fd, "%d", *((int32_t*)datas[i].data));
                break;
            case TYPE_INT64:
                #ifdef __EMSCRIPTEN__
                    fprintf(ics->fd, "%lld", *((int64_t*)datas[i].data));
                #else
                    fprintf(ics->fd, "%ld", *((int64_t*)datas[i].data));
                #endif
                break;
            case TYPE_FLOAT:
                fprintf(ics->fd, "%f", *((float*)datas[i].data));
                break;
            case TYPE_STRING:
                fprintf(ics->fd, "%s", (char*)datas[i].data);
                break;
        }
        if(i < nbDatas - 1){
            fprintf(ics->fd, ",");
            fflush(ics->fd);
        }
    }
    fprintf(ics->fd, "\n");
    fflush(ics->fd);

    return 0;
}