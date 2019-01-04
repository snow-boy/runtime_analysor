#ifndef DATA_ENDEC_H
#define DATA_ENDEC_H

#include <string>
#include <tuple>
#include "runtime_types.h"

class DataEndec
{
public:
    DataEndec(const std::string &type_name, const std::string &var_name):
        type_name_(type_name),
        var_name_(var_name)
    {}

    DataEndec()
    {}

    template<typename T>
    std::shared_ptr<DataImage> encode(T t)
    {
        return encode(std::pair<const void *, int>(&t, static_cast<int>(sizeof(T))));
    }

    std::shared_ptr<DataImage> encode(std::pair<const void *, int> data)
    {
        std::shared_ptr<DataImage> data_image = std::make_shared<DataImage>();
        strcpy_s(data_image->type_name, type_name_.data());
        strcpy_s(data_image->var_name, var_name_.data());
        data_image->data.resize(data.second);
        memcpy(data_image->data.data(), data.first, data.second);
        data_image->next = nullptr;

        return data_image;
    }

    template<typename T>
    T decode(std::shared_ptr<DataImage> data_image)
    {
        return *reinterpret_cast<T *>(data_image->data.data());
    }

private:
    std::string type_name_;
    std::string var_name_;
};

#endif // DATA_ENDEC_H
