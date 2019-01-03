#include "runtimefilereader.h"
#include <fstream>
#include <cassert>
#include <iostream>
#include "runtimefile_types.h"
#include "blockreader.h"

class RuntimeFileReader::Imp
{
public:
    std::list<std::shared_ptr<CallImage> > call_image_list;
    std::list<std::shared_ptr<ClassCallImage> > class_call_image_list;
    std::string version;
};

RuntimeFileReader::RuntimeFileReader()
{
    imp_ = std::make_unique<Imp>();
}

RuntimeFileReader::~RuntimeFileReader()
{

}

static std::shared_ptr<DataImage> toDataImage(std::shared_ptr<BlockNode> block_node)
{
    std::shared_ptr<DataImage> data_image = std::make_shared<DataImage>();
    for(std::shared_ptr<BlockNode> child : block_node->children())
    {
        switch (child->blockTag())
        {
        case RTB_Data:
        {
            data_image->next = toDataImage(child);
            break;
        }
        case RTB_DataHeader:
        {
            DataHeader data_header = *reinterpret_cast<const DataHeader *>(child->blockData().data());
            strcpy_s(data_image->type_name, data_header.type_name);
            strcpy_s(data_image->var_name, data_header.var_name);
            break;
        }
        case RTB_DataBody:
        {
            data_image->data = child->blockData();
            break;
        }
        default:
            assert(false);
            break;
        }
    }

    return data_image;
}

static std::shared_ptr<CallImage> toCallImage(std::shared_ptr<BlockNode> block_node)
{
    std::shared_ptr<CallImage> call_image = std::make_shared<CallImage>();
    for(std::shared_ptr<BlockNode> child : block_node->children())
    {
        switch(child->blockTag())
        {
        case RTB_CallImageHeader:
        {
            CallImageHeader call_image_header = *reinterpret_cast<const CallImageHeader *>(child->blockData().data());
            strcpy_s(call_image->fun_name, call_image_header.fun_name);
            call_image->timestamp = call_image_header.timestamp;
            call_image->thread_id = call_image_header.thread_id;
            break;
        }
        case RTB_ReturnData:
        {
            call_image->ret = toDataImage(*child->children().begin());
            break;
        }
        case RTB_ArgData:
        {
            for(std::shared_ptr<BlockNode> arg_block : child->children())
            {
                call_image->arg_list.push_back(toDataImage(arg_block));
            }

            break;
        }
        case RTB_OtherData:
        {
            for(std::shared_ptr<BlockNode> data_block : child->children())
            {
                call_image->data_list.push_back(toDataImage(data_block));
            }

            break;
        }
        default:
            assert(false);
            break;
        }
    }

    return call_image;
}

static std::shared_ptr<ClassCallImage> toClassCallImage(std::shared_ptr<BlockNode> block_node)
{
    std::shared_ptr<ClassCallImage> class_call_image = std::make_shared<ClassCallImage>();
    std::shared_ptr<CallImage> call_image = std::make_shared<CallImage>();
    class_call_image->call_image = call_image;

    for(std::shared_ptr<BlockNode> child : block_node->children())
    {
        switch(child->blockTag())
        {
        case RTB_ClassCallImageHeader:
        {
            ClassCallImageHeader class_call_image_header =
                    *reinterpret_cast<const ClassCallImageHeader *>(child->blockData().data());
            strcpy_s(class_call_image->class_name, class_call_image_header.class_name);
            class_call_image->instance_id = class_call_image_header.instance_id;

            CallImageHeader &call_image_header = class_call_image_header.call_image_header;
            strcpy_s(call_image->fun_name, call_image_header.fun_name);
            call_image->timestamp = call_image_header.timestamp;
            call_image->thread_id = call_image_header.thread_id;
            break;
        }
        case RTB_ReturnData:
        {
            call_image->ret = toDataImage(*child->children().begin());
            break;
        }
        case RTB_ArgData:
        {
            for(std::shared_ptr<BlockNode> arg_block : child->children())
            {
                call_image->arg_list.push_back(toDataImage(arg_block));
            }

            break;
        }
        case RTB_OtherData:
        {
            for(std::shared_ptr<BlockNode> data_block : child->children())
            {
                call_image->data_list.push_back(toDataImage(data_block));
            }

            break;
        }
        default:
            assert(false);
            break;
        }
    }

    return class_call_image;
}

bool RuntimeFileReader::read(const std::string &file_path)
{
    BlockReader block_reader;
    if(!block_reader.open(file_path)){
        return false;
    }

    imp_->version = block_reader.version();
    std::list<std::shared_ptr<BlockNode>> block_node_list = block_reader.readAll();

    for(std::shared_ptr<BlockNode> block_node : block_node_list)
    {
        if(block_node->blockTag() == RTB_CallImage){
            imp_->call_image_list.push_back(toCallImage(block_node));
        }
        else if(block_node->blockTag() == RTB_ClassCallImage){
            imp_->class_call_image_list.push_back(toClassCallImage(block_node));
        }
        else{
            assert(false);
            continue;
        }
    }

    return true;
}

std::string RuntimeFileReader::version()
{
    return imp_->version;
}

CallImageList RuntimeFileReader::callImageList()
{
    return imp_->call_image_list;
}

ClassCallImageList RuntimeFileReader::classCallImageList()
{
    return imp_->class_call_image_list;
}
