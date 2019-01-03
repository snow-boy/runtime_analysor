#ifndef BLOCKREADER_H
#define BLOCKREADER_H

#include <string>
#include <fstream>
#include "blocknode.h"

class BlockReader
{
public:
    BlockReader(const std::string &file_path);
    BlockReader();

    std::string version(){ return version_; }

    bool open(const std::string &file_path);
    bool isOpen();
    void close();

    std::list<std::shared_ptr<BlockNode>> readAll();
    std::list<std::shared_ptr<BlockNode>> blockList();

private:
    std::ifstream stream_;
    std::string version_;
    std::list<std::shared_ptr<BlockNode>> block_list_;
};

#endif // BLOCKREADER_H
