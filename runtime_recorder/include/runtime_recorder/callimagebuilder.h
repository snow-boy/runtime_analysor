#ifndef CALLIMAGEBUILDER_H
#define CALLIMAGEBUILDER_H

#include <list>
#include <thread>
#include <chrono>
#include <strstream>
#include <memory>

#include "runtime_types.h"
#include "paramendec.h"

class CallImageBuilderBase
{
public:
    CallImageBuilderBase(const std::string &fun_name):
        call_image_(nullptr),
        fun_name_(fun_name)
    {}

protected:
    template<typename _First, typename ... _Rest>
    void buildArgs(_First first, _Rest ...rest)
    {
        ParamEndec param_builder(typeid (first).name());
        call_image_->arg_list.push_back(param_builder.encode(first));
        buildArgs(rest...);
    }

    void buildArgs()
    {
    }

    void allocCallImage()
    {
        call_image_ = new CallImage;

        std::ostrstream oss;
        oss << std::this_thread::get_id();
        std::istrstream iss(oss.str());
        iss >> call_image_->thread_id;

        call_image_->timestamp =
                std::chrono::high_resolution_clock::now().time_since_epoch().count();
    } 

    CallImage *call_image_;
    std::string fun_name_;
};

template <typename _RetT, typename ... _ArgsT>
class CallImageBuilder: public CallImageBuilderBase
{
public:
    CallImageBuilder(const std::string &fun_name):
        CallImageBuilderBase(fun_name)
    {}

    std::shared_ptr<CallImage> buildCallImage(_RetT ret, _ArgsT ...args)
    {
        allocCallImage();
        strcpy_s(call_image_->fun_name, fun_name_.data());

        ParamEndec param_builder(typeid (ret).name());
        call_image_->ret_param = param_builder.encode(ret);

        buildArgs(args...);

        std::shared_ptr<CallImage> call_image_ptr(call_image_);
        call_image_ = nullptr;
        return call_image_ptr;
    }
};

template <typename ... _ArgsT>
class CallImageBuilder<void, _ArgsT...>: public CallImageBuilderBase
{
public:
    CallImageBuilder(const std::string &fun_name):
        CallImageBuilderBase(fun_name)
    {}

    std::shared_ptr<CallImage> buildCallImage(_ArgsT ...args)
    {
        allocCallImage();
        strcpy_s(call_image_->fun_name, fun_name_.data());

        call_image_->ret_param = nullptr;

        buildArgs(args...);

        std::shared_ptr<CallImage> call_image_ptr(call_image_);
        call_image_ = nullptr;
        return call_image_ptr;
    }
};

template <>
class CallImageBuilder<void, void>: public CallImageBuilderBase
{
public:
    CallImageBuilder(const std::string &fun_name):
        CallImageBuilderBase(fun_name)
    {}

    std::shared_ptr<CallImage> buildCallImage()
    {
        allocCallImage();
        strcpy_s(call_image_->fun_name, fun_name_.data());

        call_image_->ret_param = nullptr;

        std::shared_ptr<CallImage> call_image_ptr(call_image_);
        call_image_ = nullptr;
        return call_image_ptr;
    }
};


#endif // CALLIMAGEBUILDER_H
