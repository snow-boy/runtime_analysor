#ifndef RUNTIMEFILE_TYPES_H
#define RUNTIMEFILE_TYPES_H

#include <stdint.h>
#include "runtime_types.h"

#define FILE_TAG "@runtime@"
#define CURRENT_VERSION "0.0.0.1"

#pragma pack(push, 1)

struct FileHeader
{
    char tag[16];
    char version[64];
};

enum BlockType
{
    BT_Unknow = 0,
    BT_CallImage = 1,
    BT_ClassCallImage = 2,
    BT_ParamReturn = 3,
    BT_ParamArg = 4
};

struct BlockHeader
{
    int32_t block_type;
    int32_t byte_size;
};

struct ParamBlock
{
    BlockHeader header;
    int32_t index;
    char type_name[64];
    char data[0];
};

struct CallImageBlock
{
    BlockHeader header;
    char fun_name[64];
    int64_t timestamp;
    uint32_t thread_id;
    int32_t param_count;
    //ParamBlock params[0];
};

struct ClassCallImageBlock
{
    BlockHeader header;
    char class_name[64];
    uint64_t instance_id;
    int32_t method_type;
    char fun_name[64];
    int64_t timestamp;
    uint32_t thread_id;
    int32_t param_count;
    //ParamBlock params[0];
};

#pragma pack(pop)

#endif // RUNTIMEFILE_TYPES_H
