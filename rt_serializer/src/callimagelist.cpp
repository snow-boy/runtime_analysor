#include "callimagelist.h"
#include <algorithm>
#include <set>
#include <iterator>
#include <map>

CallImageList::CallImageList()
{

}

CallImageList::CallImageList(const std::list<std::shared_ptr<CallImage> > &call_image_list):
    std::list<std::shared_ptr<CallImage> >(call_image_list)
{

}

void CallImageList::sortByTime()
{
    std::map<int64_t, std::list<std::shared_ptr<CallImage>>> sorted;

    for(std::shared_ptr<CallImage> e : *this){
        sorted[e->timestamp].push_back(e);
    }

    clear();

    for(auto e: sorted){
        std::copy(e.second.begin(), e.second.end(), std::back_inserter(*this));
    }
}

std::vector<uint32_t> CallImageList::threads()
{
    std::set<uint32_t> thread_set;
    for(std::shared_ptr<CallImage> e : *this){
        thread_set.insert(e->thread_id);
    }

    std::vector<uint32_t> all_threads;
    std::copy(thread_set.begin(), thread_set.end(), std::back_inserter(all_threads));
    return all_threads;
}

CallImageList CallImageList::getCallImagestByThread(uint32_t thread_id)
{
    CallImageList call_image_list;

    for(std::shared_ptr<CallImage> e : *this){
        if(e->thread_id == thread_id){
            call_image_list.push_back(e);
        }
    }

    return call_image_list;
}

std::list<std::string> CallImageList::functions()
{
    std::set<std::string> func_set;
    for(std::shared_ptr<CallImage> e : *this){
        func_set.insert(e->fun_name);
    }

    std::list<std::string> func_list;
    std::copy(func_set.begin(), func_set.end(), std::back_inserter(func_list));
    return func_list;
}

CallImageList CallImageList::getCallImagestByFunction(const std::string &fun_name)
{
    CallImageList call_image_list;

    for(std::shared_ptr<CallImage> e : *this){
        if(fun_name == e->fun_name){
            call_image_list.push_back(e);
        }
    }

    return call_image_list;
}
