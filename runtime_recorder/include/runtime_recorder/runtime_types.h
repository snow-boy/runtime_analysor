#ifndef RUNTIME_TYPES_H
#define RUNTIME_TYPES_H

#include <stdint.h>
#include <list>
#include <vector>
#include <memory>

struct Param
{
    char type_name[64];
    std::vector<char> data;
};

struct CallImage
{
    char fun_name[64];
    int64_t timestamp;
    uint32_t thread_id;
    std::shared_ptr<Param> ret_param;
    std::list<std::shared_ptr<Param>> arg_list;
};

enum MethodType
{
    MT_Unknow = 0,
    MT_Constructure = 1,
    MT_Destructure = 2,
    MT_Method = 3
};

struct ClassCallImage
{
    char class_name[64];
    uint64_t instance_id;
    int32_t method_type;
    std::shared_ptr<CallImage> call_image;
};

#endif // RUNTIME_TYPES_H
