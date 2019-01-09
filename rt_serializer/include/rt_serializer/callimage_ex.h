#ifndef CALLIMAGE_EX_H
#define CALLIMAGE_EX_H

#include <thread>
#include <chrono>
#include <strstream>
#include <memory>
#include <atomic>

#include "dataendec.h"

class CallImageEx : public CallImage
{
public:
    CallImageEx(const CallImage &other)
    {
        *this = other;
    }

    CallImageEx(const std::string &fun_name)
    {
        this->fun_name = fun_name;

        std::ostrstream oss;
        oss << std::this_thread::get_id();
        std::istrstream iss(oss.str());
        iss >> this->thread_id;

        this->enter_time =
                std::chrono::duration_cast<std::chrono::microseconds>(
                    std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        this->leave_time = this->enter_time;

        uint32_t tick = getTick();
        this->enter_tick = tick;
        this->leave_tick = tick;
    }

    void setLeave()
    {
        this->leave_time =
                std::chrono::duration_cast<std::chrono::microseconds>(
                    std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        this->leave_tick = getTick();
    }

    template<typename ... _Args>
    void setReturn(_Args ... args)
    {
        this->ret = AutoEncoder().encode(args...);
    }

    template<typename ... _Args>
    void addArg(_Args ... args)
    {
        this->arg_list.push_back(AutoEncoder().encode(args...));
    }

    template<typename ... _Args>
    void addData(_Args ... args)
    {
        this->data_list.push_back(AutoEncoder().encode(args...));
    }

private:
    uint32_t getTick()
    {
        static std::atomic<uint32_t> tick(0);
        return tick ++;
    }
};

#define MakeCallImageEx() std::make_shared<CallImageEx>(__FUNCTION__)

#endif // CALLIMAGE_EX_H
