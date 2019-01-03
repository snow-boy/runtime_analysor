#ifndef CLASSCALLIMAGELIST_H
#define CLASSCALLIMAGELIST_H

#include <list>
#include "runtime_types.h"

class ClassCallImageList: public std::list<std::shared_ptr<ClassCallImage>>
{
public:
    ClassCallImageList();

    ClassCallImageList(const std::list<std::shared_ptr<ClassCallImage>> &call_image_list);

    void sortByTime();

    std::vector<uint32_t> threads();

    ClassCallImageList getCallImagesByThread(uint32_t thread_id);

    std::list<std::string> classList();

    ClassCallImageList getCallImagesByClass(const std::string &class_name);

    std::vector<uint64_t> instances();

    ClassCallImageList getCallImagesByInstance(uint64_t instance_id);

    std::list<std::string> functions();

    ClassCallImageList getCallImagestByFunction(const std::string &fun_name);
};

#endif // CLASSCALLIMAGELIST_H
