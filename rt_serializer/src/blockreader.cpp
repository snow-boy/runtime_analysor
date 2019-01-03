#include "blockreader.h"

BlockReader::BlockReader(const std::string &file_path)
{
    open(file_path);
}

BlockReader::BlockReader()
{

}

bool BlockReader::open(const std::string &file_path)
{
    stream_.open(file_path, std::ios::in|std::ios::binary);
    if(!stream_.is_open()){
        return false;
    }

    FileHeader file_header;
    stream_.read(reinterpret_cast<char *>(&file_header), sizeof(file_header));
    if(std::string(FILE_TAG) != file_header.tag){
        stream_.close();
        return false;
    }

    version_ = file_header.version;

    return true;
}

bool BlockReader::isOpen()
{
    return stream_.is_open();
}

void BlockReader::close()
{
    stream_.close();
}

std::list<std::shared_ptr<BlockNode> > BlockReader::readAll()
{
    if(block_list_.size() == 0){
        while(stream_.peek() != std::ifstream::traits_type::eof())
        {
            std::shared_ptr<BlockNode> block = std::make_shared<BlockNode>(stream_);
            block_list_.push_back(block);
        }
    }

    return block_list_;
}

std::list<std::shared_ptr<BlockNode> > BlockReader::blockList()
{
    return block_list_;
}
