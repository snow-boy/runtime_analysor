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

    std::vector<uint32_t> threads() const;

    ClassCallImageList getCallImagesByThread(uint32_t thread_id) const;

    std::list<std::string> classList() const;

    ClassCallImageList getCallImagesByClass(const std::string &class_name) const;

    std::vector<uint64_t> instances() const;

    ClassCallImageList getCallImagesByInstance(uint64_t instance_id) const;

    std::list<std::string> functions() const;

    ClassCallImageList getCallImagestByFunction(const std::string &fun_name) const;
};

#endif // CLASSCALLIMAGELIST_H
