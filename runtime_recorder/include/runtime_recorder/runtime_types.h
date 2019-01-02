#ifndef RUNTIME_TYPES_H
#define RUNTIME_TYPES_H

#include <stdint.h>
#include <list>
#include <vector>
#include <memory>

struct DataImage
{
    char type_name[64];
    char var_name[64];
    std::vector<char> data;
    std::shared_ptr<DataImage> next;
};

struct CallImage
{
    char fun_name[64];
    int64_t timestamp;
    uint32_t thread_id;
    std::shared_ptr<DataImage> ret;
    std::list<std::shared_ptr<DataImage>> arg_list;
    std::list<std::shared_ptr<DataImage>> data_list;
};

struct ClassCallImage
{
    char class_name[64];
    uint64_t instance_id;
    std::shared_ptr<CallImage> call_image;
};

#endif // RUNTIME_TYPES_H
