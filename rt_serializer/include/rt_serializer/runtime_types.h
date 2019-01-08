#ifndef RUNTIME_TYPES_H
#define RUNTIME_TYPES_H

#include <stdint.h>
#include <list>
#include <vector>
#include <memory>

struct DataImage
{
    std::string type_name;
    std::string var_name;
    std::vector<char> data;
    std::shared_ptr<DataImage> next;
};

struct CallImage
{
    std::string fun_name;
    int64_t timestamp;
    uint32_t thread_id;
    std::shared_ptr<DataImage> ret;
    std::list<std::shared_ptr<DataImage>> arg_list;
    std::list<std::shared_ptr<DataImage>> data_list;
};

struct ClassCallImage
{
    std::string class_name;
    uint64_t instance_id;
    std::shared_ptr<CallImage> call_image;
};

#endif // RUNTIME_TYPES_H
