#include "classcallimagelist.h"
#include <algorithm>
#include <set>
#include <iterator>
#include <map>

ClassCallImageList::ClassCallImageList()
{

}

ClassCallImageList::ClassCallImageList(const std::list<std::shared_ptr<ClassCallImage> > &call_image_list):
    std::list<std::shared_ptr<ClassCallImage> >(call_image_list)
{

}

void ClassCallImageList::sortByTime()
{
    std::map<int64_t, std::list<std::shared_ptr<ClassCallImage>>> sorted;

    for(std::shared_ptr<ClassCallImage> e : *this){
        sorted[e->call_image->timestamp].push_back(e);
    }

    clear();

    for(auto e: sorted){
        std::copy(e.second.begin(), e.second.end(), std::back_inserter(*this));
    }
}

std::vector<uint32_t> ClassCallImageList::threads()
{
    std::set<uint32_t> thread_set;
    for(std::shared_ptr<ClassCallImage> e : *this){
        thread_set.insert(e->call_image->thread_id);
    }

    std::vector<uint32_t> all_threads;
    std::copy(thread_set.begin(), thread_set.end(), std::back_inserter(all_threads));
    return all_threads;
}

ClassCallImageList ClassCallImageList::getCallImagesByThread(uint32_t thread_id)
{
    ClassCallImageList call_image_list;

    for(std::shared_ptr<ClassCallImage> e : *this){
        if(e->call_image->thread_id == thread_id){
            call_image_list.push_back(e);
        }
    }

    return call_image_list;
}

std::list<std::string> ClassCallImageList::classList()
{
    std::set<std::string> class_set;
    for(std::shared_ptr<ClassCallImage> e : *this){
        class_set.insert(e->class_name);
    }

    std::list<std::string> class_list;
    std::copy(class_set.begin(), class_set.end(), std::back_inserter(class_list));
    return class_list;
}

ClassCallImageList ClassCallImageList::getCallImagesByClass(const std::string &class_name)
{
    ClassCallImageList call_image_list;

    for(std::shared_ptr<ClassCallImage> e : *this){
        if(class_name == e->class_name){
            call_image_list.push_back(e);
        }
    }

    return call_image_list;
}

std::vector<uint64_t> ClassCallImageList::instances()
{
    std::set<uint64_t> instance_set;

    for(std::shared_ptr<ClassCallImage> e : *this){
        instance_set.insert(e->instance_id);
    }

    std::vector<uint64_t> all_instance;
    std::copy(instance_set.begin(), instance_set.end(), std::back_inserter(all_instance));
    return all_instance;
}

ClassCallImageList ClassCallImageList::getCallImagesByInstance(uint64_t instance_id)
{
    ClassCallImageList call_image_list;

    for(std::shared_ptr<ClassCallImage> e : *this){
        if(e->instance_id == instance_id){
            call_image_list.push_back(e);
        }
    }

    return call_image_list;
}

std::list<std::string> ClassCallImageList::functions()
{
    std::set<std::string> func_set;
    for(std::shared_ptr<ClassCallImage> e : *this){
        func_set.insert(e->call_image->fun_name);
    }

    std::list<std::string> func_list;
    std::copy(func_set.begin(), func_set.end(), std::back_inserter(func_list));
    return func_list;
}

ClassCallImageList ClassCallImageList::getCallImagestByFunction(const std::string &fun_name)
{
    ClassCallImageList call_image_list;

    for(std::shared_ptr<ClassCallImage> e : *this){
        if(fun_name == e->call_image->fun_name){
            call_image_list.push_back(e);
        }
    }

    return call_image_list;
}
