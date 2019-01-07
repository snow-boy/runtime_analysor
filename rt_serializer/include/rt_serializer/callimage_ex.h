#ifndef CALLIMAGE_EX_H
#define CALLIMAGE_EX_H

#include <thread>
#include <chrono>
#include <strstream>
#include <memory>

#include "dataendec.h"
#include "runtimefile_types.h"

class CallImageEx : public CallImage
{
public:
    CallImageEx(const CallImage &other)
    {
        *this = other;
    }

    CallImageEx(const std::string &fun_name)
    {
        strcpy_s(this->fun_name, fun_name.data());

        std::ostrstream oss;
        oss << std::this_thread::get_id();
        std::istrstream iss(oss.str());
        iss >> this->thread_id;

        this->timestamp =
                std::chrono::high_resolution_clock::now().time_since_epoch().count();
    }

    template<typename ... _Args>
    void setReturn(_Args ... args)
    {
        this->ret = buildDataImage(args...);
    }

    template<typename ... _Args>
    void addArg(_Args ... args)
    {
        this->arg_list.push_back(buildDataImage(args...));
    }

    template<typename ... _Args>
    void addData(_Args ... args)
    {
        this->data_list.push_back(buildDataImage(args...));
    }

private:
    template<typename _T, typename _TypeName, typename _VarName, typename ..._Rest>
    std::shared_ptr<DataImage> buildDataImage(_T var, _TypeName type_name, _VarName var_name, _Rest ...rest)
    {
        std::shared_ptr<DataImage> data_image = std::make_shared<DataImage>();
        strcpy_s(data_image->type_name, std::string(type_name).data());
        strcpy_s(data_image->var_name, std::string(var_name).data());

        DataEncoder<_T>().encode(data_image, var);
        data_image->next = buildDataImage(rest...);
        return data_image;
    }

    std::shared_ptr<DataImage> buildDataImage()
    {
        return nullptr;
    }
};

#define MakeCallImageEx() std::make_shared<CallImageEx>(__FUNCTION__)
#define MakeData(data) data, typeid(data).name(), #data
#define MakePtrData(data, size) std::pair<const void *, int>(data, size), typeid(data).name(), #data

#endif // CALLIMAGE_EX_H
