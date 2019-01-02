#ifndef RUNTIMEFILE_TYPES_H
#define RUNTIMEFILE_TYPES_H

#include <stdint.h>
#include "runtime_types.h"

#pragma pack(push, 1)

enum RTBlockTag
{
    RTB_Unknow = 0,

    RTB_CallImage = 10,
    RTB_CallImageHeader = 11,

    RTB_ClassCallImage = 20,
    RTB_ClassCallImageHeader = 21,

    RTB_ReturnData = 30,
    RTB_ArgData = 40,
    RTB_OtherData = 50,

    RTB_Data = 60,
    RTB_DataHeader = 61,
    RTB_DataBody = 62
};

struct DataHeader
{
    char type_name[64];
    char var_name[64];
};

struct CallImageHeader
{
    char fun_name[64];
    int64_t timestamp;
    uint32_t thread_id;
};

struct ClassCallImageHeader
{
    char class_name[64];
    uint64_t instance_id;
    CallImageHeader call_image_header;
};

#pragma pack(pop)

// //////////// //
// BlockHeader  //
// ///////////  //
// BlockBody // //
//////////////////

#endif // RUNTIMEFILE_TYPES_H
