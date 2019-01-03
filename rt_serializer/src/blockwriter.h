#ifndef BLOCKWRITER_H
#define BLOCKWRITER_H

#include <string>
#include <fstream>
#include "blocknode.h"

class BlockWriter
{
public:
    BlockWriter(const std::string &file_path);
    BlockWriter();

    bool open(const std::string &file_path);
    bool isOpen();
    void close();

    void writeBlock(std::shared_ptr<BlockNode> block);

private:
    std::ofstream stream_;
};

#endif // BLOCKWRITER_H
