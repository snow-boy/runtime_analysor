#ifndef RUNTIME_UTILS_H
#define RUNTIME_UTILS_H

#include <thread>
#include <chrono>
#include <strstream>
#include <memory>

#include "dataendec.h"

std::shared_ptr<CallImage> allocCallImage(const std::string &fun_name)
{
    std::shared_ptr<CallImage> call_image = std::make_shared<CallImage>();
    strcpy_s(call_image->fun_name, fun_name.data());

    std::ostrstream oss;
    oss << std::this_thread::get_id();
    std::istrstream iss(oss.str());
    iss >> call_image->thread_id;

    call_image->timestamp =
            std::chrono::high_resolution_clock::now().time_since_epoch().count();

    return call_image;
}

template<typename T>
std::shared_ptr<ClassCallImage> allocClassCallImage(const std::string &fun_name, T *instance)
{
    std::shared_ptr<ClassCallImage> class_call_image = std::make_shared<ClassCallImage>();
    strcpy_s(class_call_image->class_name, typeid (T).name());
    class_call_image->instance_id = reinterpret_cast<uint64_t>(instance);
    class_call_image->call_image = allocCallImage(fun_name);

    return class_call_image;
}

#endif // RUNTIME_UTILS_H
