#ifndef PARAMBUILDER_H
#define PARAMBUILDER_H

#include <string>
#include <tuple>
#include "runtime_types.h"

class ParamEndec
{
public:
    ParamEndec(const std::string &type_name):
        type_name_(type_name)
    {}

    ParamEndec()
    {}

    template<typename T>
    std::shared_ptr<Param> encode(T t)
    {
        return encode(std::pair<void *, int>(&t, static_cast<int>(sizeof(T))));
    }

    template<typename T>
    T decode(std::shared_ptr<Param> param)
    {
        return *reinterpret_cast<T *>(param->data.data());
    }

private:
    std::string type_name_;
};

template<>
std::shared_ptr<Param> ParamEndec::encode<std::pair<void *, int>>(std::pair<void *, int> data)
{
    std::shared_ptr<Param> param = std::make_shared<Param>();
    strcpy_s(param->type_name, type_name_.data());
    param->data.resize(data.second);
    memcpy(param->data.data(), data.first, data.second);

    return param;
}

#endif // PARAMBUILDER_H
