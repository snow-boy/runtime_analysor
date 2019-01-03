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
    template<typename _T, typename _Str, typename ..._Rest>
    std::shared_ptr<DataImage> buildDataImage(_T var, _Str var_name, _Rest ...rest)
    {
        std::shared_ptr<DataImage> data_image = DataEndec(typeid (var).name(), var_name).encode(var);
        data_image->next = buildDataImage(rest...);
        return data_image;
    }

    std::shared_ptr<DataImage> buildDataImage()
    {
        return nullptr;
    }
};

#define MakeData(data) data, #data

#endif // CALLIMAGE_EX_H
