#ifndef RUNTIMEFILEWRITER_H
#define RUNTIMEFILEWRITER_H

#include <memory>
#include "runtime_types.h"

class RuntimeFileWriter
{
public:
    RuntimeFileWriter();
    ~RuntimeFileWriter();

    bool open(const std::string &file_path);
    void close();

    void write(std::shared_ptr<CallImage> call_image);
    void write(std::shared_ptr<ClassCallImage> class_call_image);

private:
    class Imp;
    std::unique_ptr<Imp> imp_;
};

#endif // RUNTIMEFILEWRITER_H
