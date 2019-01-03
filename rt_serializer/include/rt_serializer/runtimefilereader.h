#ifndef RUNTIMEFILEREADER_H
#define RUNTIMEFILEREADER_H

#include <memory>
#include <string>
#include <list>
#include "runtime_types.h"
#include "classcallimagelist.h"
#include "callimagelist.h"

class RuntimeFileReader
{
public:
    RuntimeFileReader();
    ~RuntimeFileReader();

    bool read(const std::string &file_path);

    std::string version();

    CallImageList callImageList();

    ClassCallImageList classCallImageList();

private:
    class Imp;
    std::unique_ptr<Imp> imp_;
};

#endif // RUNTIMEFILEREADER_H
