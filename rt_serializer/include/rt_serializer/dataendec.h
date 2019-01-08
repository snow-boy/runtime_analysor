#ifndef DATA_ENDEC_H
#define DATA_ENDEC_H

#include <string>
#include <tuple>
#include <functional>
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
        if(t == nullptr){
            return;
        }

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

struct AutoEncoder
{
    template<typename _T, typename _TypeName, typename _VarName, typename ..._Rest>
    std::shared_ptr<DataImage> encode(_T var, _TypeName type_name, _VarName var_name, _Rest ...rest)
    {
        std::shared_ptr<DataImage> data_image = std::make_shared<DataImage>();
        data_image->type_name = type_name;
        data_image->var_name = var_name;

        auto encoder = std::make_shared<DataEncoder<_T>>();
        encoder->encode(data_image, var);
        reserver_.push_back([=](){ return encoder.get(); });

        data_image->next = encode(rest...);
        return data_image;
    }

private:
    std::shared_ptr<DataImage> encode()
    {
        return nullptr;
    }

    std::list<std::function<void *()>> reserver_;
};

struct AutoDecoder
{
    template<typename _T, typename ..._Rest>
    void decode(std::shared_ptr<DataImage> data_image, _T var, _Rest ...rest)
    {
        static_assert (std::is_pointer<_T>::value, "must be a pointer");
        if(data_image == nullptr){
            return;
        }

        auto decoder = std::make_shared<DataDecoder<typename std::remove_pointer<_T>::type>>();
        reserver_.push_back([=](){ return decoder.get(); });

        *var = decoder->decode(data_image);
        decode(data_image->next, rest...);
    }

private:
    void decode(std::shared_ptr<DataImage>)
    {
    }

    std::list<std::function<void *()>> reserver_;
};

#define MakeData(data) data, typeid(data).name(), #data
#define MakePtrData(data, size) std::pair<const void *, int>(data, size), typeid(data).name(), #data

#endif // DATA_ENDEC_H
