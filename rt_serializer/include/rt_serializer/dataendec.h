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


template <typename T>
struct DataDecoder
{
    T decode(std::shared_ptr<DataImage> data_image)
    {
        return *reinterpret_cast<T *>(data_image->data.data());
    }
};

#endif // DATA_ENDEC_H
