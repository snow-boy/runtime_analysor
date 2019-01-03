#include "blockwriter.h"

BlockWriter::BlockWriter(const std::string &file_path)
{
    open(file_path);
}

BlockWriter::BlockWriter()
{

}

bool BlockWriter::open(const std::string &file_path)
{
    stream_.open(file_path, std::ios::out|std::ios::binary);
    if(!stream_.is_open()){
        return false;
    }

    FileHeader file_header;
    strcpy_s(file_header.tag, std::string(FILE_TAG).data());
    strcpy_s(file_header.version, std::string(CURRENT_VERSION).data());

    stream_.write(reinterpret_cast<char *>(&file_header), sizeof(file_header));

    return true;
}

bool BlockWriter::isOpen()
{
    return stream_.is_open();
}

void BlockWriter::close()
{
    stream_.close();
}

void BlockWriter::writeBlock(std::shared_ptr<BlockNode> block)
{
    block->serializeToFile(stream_);
}
