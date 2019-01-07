#ifndef CALLIMAGELIST_H
#define CALLIMAGELIST_H

#include <list>
#include "runtime_types.h"

class CallImageList: public std::list<std::shared_ptr<CallImage>>
{
public:
    CallImageList();

    CallImageList(const std::list<std::shared_ptr<CallImage>> &call_image_list);

    void sortByTime();

    std::vector<uint32_t> threads() const;

    CallImageList getCallImagestByThread(uint32_t thread_id) const;

    std::list<std::string> functions() const;

    CallImageList getCallImagestByFunction(const std::string &fun_name) const;
};

#endif // CALLIMAGELIST_H
