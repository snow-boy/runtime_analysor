#ifndef BLOCK_TYPES_H
#define BLOCK_TYPES_H

#include <stdint.h>

#define FILE_TAG "@runtime@"
#define CURRENT_VERSION "0.0.0.1"

#pragma pack(push, 1)

struct FileHeader
{
    char tag[16];
    char version[64];
};

struct BlockHeader
{
    int32_t block_tag;
    uint32_t body_size;
    uint32_t data_size;
};

#pragma pack(pop)

#endif // BLOCK_TYPES_H
