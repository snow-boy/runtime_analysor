#ifndef CLASSCALLIMAGE_EX_H
#define CLASSCALLIMAGE_EX_H

#include "callimage_ex.h"

class ClassCallImageEx : public ClassCallImage
{
public:
    template<typename T>
    ClassCallImageEx(const std::string &fun_name, T *instance)
    {
        this->class_name = typeid (T).name();
        this->instance_id = reinterpret_cast<uint64_t>(instance);
        this->call_image = std::make_shared<CallImageEx>(fun_name);
    }

    ClassCallImageEx(const ClassCallImage &other)
    {
        *this = other;
    }

    template<typename ... _Args>
    void setReturn(_Args ... args)
    {
        std::static_pointer_cast<CallImageEx>(this->call_image)->setReturn(args...);
    }

    template<typename ... _Args>
    void addArg(_Args ... args)
    {
        std::static_pointer_cast<CallImageEx>(this->call_image)->addArg(args...);
    }

    template<typename ... _Args>
    void addData(_Args ... args)
    {
        std::static_pointer_cast<CallImageEx>(this->call_image)->addData(args...);
    }
};

#define MakeClassCallImageEx(instance) std::make_shared<ClassCallImageEx>(__FUNCTION__, instance)

#endif // CLASSCALLIMAGE_EX_H
