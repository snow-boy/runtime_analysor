#ifndef DATA_ENDEC_H
#define DATA_ENDEC_H

#include <string>
#include <tuple>
#include "runtime_types.h"

template<typename T>
struct DataEncoder
{
    void encode(std::shared_ptr<DataImage> data_image, T t)
    {
        data_image->data.resize(sizeof(T));
        memcpy(data_image->data.data(), &t, sizeof(T));
    }
};

template<typename T>
struct DataEncoder<T *>
{
    void encode(std::shared_ptr<DataImage> data_image, T* t)
    {
        data_image->data.resize(sizeof(T));
        memcpy(data_image->data.data(), t, sizeof(T));
    }
};

template<>
struct DataEncoder<std::pair<const void *, int>>
{
    void encode(std::shared_ptr<DataImage> data_image, std::pair<const void *, int> t)
    {
        data_image->data.resize(t.second);
        memcpy(data_image->data.data(), t.first, t.second);
    }
};

template <typename T>
struct DataDecoder
{
    T decode(std::shared_ptr<DataImage> data_image)
    {
        return *reinterpret_cast<T *>(data_image->data.data());
    }
};

template <typename T>
struct DataDecoder<T *>
{
    T* decode(std::shared_ptr<DataImage> data_image)
    {
        return reinterpret_cast<T *>(data_image->data.data());
    }
};

template<typename _T, typename _TypeName, typename _VarName, typename ..._Rest>
std::shared_ptr<DataImage> encodeData(_T var, _TypeName type_name, _VarName var_name, _Rest ...rest)
{
    std::shared_ptr<DataImage> data_image = std::make_shared<DataImage>();
    data_image->type_name = type_name;
    data_image->var_name = var_name;

    DataEncoder<_T>().encode(data_image, var);
    data_image->next = encodeData(rest...);
    return data_image;
}

std::shared_ptr<DataImage> encodeData()
{
    return nullptr;
}

#define MakeData(data) data, typeid(data).name(), #data
#define MakePtrData(data, size) std::pair<const void *, int>(data, size), typeid(data).name(), #data

#endif // DATA_ENDEC_H
