#include "blocknode.h"
#include <cassert>

BlockNode::BlockNode(uint32_t tag):
    BlockNode()
{
    block_header_->block_tag = tag;
}

BlockNode::BlockNode(std::ifstream &stream):
    BlockNode()
{
    stream.read(reinterpret_cast<char *>(block_header_.get()), sizeof(BlockHeader));
    if(block_header_->data_size > 0){
        block_data_.resize(block_header_->data_size);
        stream.read(block_data_.data(), block_data_.size());
    }

    std::vector<char> buffer(sizeof(BlockHeader) + block_header_->body_size, 0);
    memcpy(buffer.data(), block_header_.get(), sizeof(BlockHeader));
    stream.read(buffer.data() + sizeof(BlockHeader), block_header_->body_size);

    fromBuffer(buffer, 0);
}

BlockNode::BlockNode(const std::vector<char> &buffer):
    BlockNode()
{
    fromBuffer(buffer, 0);
}

BlockNode::BlockNode()
{
    block_header_ = std::make_shared<BlockHeader>();
    block_header_->block_tag = 0;
    block_header_->body_size = 0;
    block_header_->data_size = 0;
}

void BlockNode::setBlockTag(uint32_t tag)
{
    block_header_->block_tag = tag;
}

uint32_t BlockNode::blockTag()
{
    return block_header_->block_tag;
}

void BlockNode::serializeToFile(std::ofstream &stream)
{
    updateBlockHeader();

    stream.write(reinterpret_cast<char *>(block_header_.get()), sizeof(BlockHeader));
    if(block_data_.size() > 0){
        stream.write(block_data_.data(), block_data_.size());
    }

    for(std::shared_ptr<BlockNode> child : children_){
        child->serializeToFile(stream);
    }
}

void BlockNode::serializeToBuffer(std::vector<char> &buffer)
{
    updateBlockHeader();

    buffer.resize(block_header_->body_size + sizeof(BlockHeader));
    serializeToBuffer(buffer, 0);
}

void BlockNode::setData(const std::vector<char> &data)
{
    block_data_ = data;
}

void BlockNode::setData(const char *data, size_t size)
{
    block_data_.resize(size);
    memcpy(block_data_.data(), data, size);
}

void BlockNode::appendChild(std::shared_ptr<BlockNode> child)
{
    children_.push_back(child);
}

size_t BlockNode::calcBodySize()
{
    size_t size = 0;
    size += block_data_.size();

    for(std::shared_ptr<BlockNode> block : children_){
        size += block->calcBodySize();
        size += sizeof(BlockHeader);
    }

    return size;
}

void BlockNode::updateBlockHeader()
{
    block_header_->data_size = static_cast<uint32_t>(block_data_.size());
    block_header_->body_size = static_cast<uint32_t>(calcBodySize());
}

size_t BlockNode::fromBuffer(const std::vector<char> &buffer, size_t offset)
{
    *block_header_ = *reinterpret_cast<const BlockHeader *>(buffer.data() + offset);
    assert(block_header_->block_tag != -1);
    size_t current_offset = sizeof(BlockHeader) + offset;
    if(block_header_->data_size > 0){
        block_data_.resize(block_header_->data_size);
        memcpy(block_data_.data(), buffer.data() + current_offset, block_data_.size());
        current_offset += block_data_.size();
    }

    while(current_offset - offset < static_cast<size_t>(block_header_->body_size) + sizeof(BlockHeader))
    {
        std::shared_ptr<BlockNode> child = std::make_shared<BlockNode>();
        current_offset = child->fromBuffer(buffer, current_offset);
        appendChild(child);
    }

    return current_offset;
}

size_t BlockNode::serializeToBuffer(std::vector<char> &buffer, size_t offset)
{
    size_t current_offset = offset;
    memcpy(buffer.data() + offset, block_header_.get(), sizeof(BlockHeader));
    current_offset += sizeof(BlockHeader);

    if(block_data_.size() > 0){
        memcpy(buffer.data() + current_offset, block_data_.data(), block_data_.size());
        current_offset += block_data_.size();
    }

    for(std::shared_ptr<BlockNode> child : children_){
        current_offset = child->serializeToBuffer(buffer, current_offset);
    }

    return current_offset;
}
