#ifndef BLOCKNODE_H
#define BLOCKNODE_H

#include <memory>
#include <list>
#include <fstream>
#include <vector>
#include "block_types.h"

class BlockNode
{
public:
    BlockNode(uint32_t tag);

    BlockNode(std::ifstream &stream);

    BlockNode(const std::vector<char> &buffer);

    BlockNode();

    void setBlockTag(uint32_t tag);

    uint32_t blockTag();

    void serializeToFile(std::ofstream &stream);

    void serializeToBuffer(std::vector<char> &buffer);

    const std::vector<char> &blockData() { return block_data_; }

    void setData(const std::vector<char> &data);

    void setData(const char *data, size_t size);

    void appendChild(std::shared_ptr<BlockNode> child);

    std::list<std::shared_ptr<BlockNode>> children() { return children_; }

private:
    size_t calcBodySize();
    void updateBlockHeader();
    size_t fromBuffer(const std::vector<char> &buffer, size_t offset);
    size_t serializeToBuffer(std::vector<char> &buffer, size_t offset);

    std::shared_ptr<BlockHeader> block_header_;
    std::vector<char> block_data_;
    std::list<std::shared_ptr<BlockNode>> children_;
};

#endif // BLOCKNODE_H
