#include "runtimefilewriter.h"

#include <fstream>
#include <iostream>
#include <cassert>
#include "runtimefile_types.h"
#include "blockwriter.h"

class RuntimeFileWriter::Imp
{
public:
    BlockWriter block_writer;
};

RuntimeFileWriter::RuntimeFileWriter()
{
    imp_ = std::make_unique<Imp>();
}

RuntimeFileWriter::~RuntimeFileWriter()
{

}

bool RuntimeFileWriter::open(const std::string &file_path)
{
    return imp_->block_writer.open(file_path);
}

void RuntimeFileWriter::close()
{
    imp_->block_writer.close();
}

static std::shared_ptr<BlockNode> makeDataBlock(std::shared_ptr<DataImage> data_image)
{
    std::shared_ptr<BlockNode> data_block = std::make_shared<BlockNode>(RTB_Data);

    {
        std::shared_ptr<BlockNode> data_header_block = std::make_shared<BlockNode>(RTB_DataHeader);
        DataHeader header;
        strcpy_s(header.type_name, data_image->type_name.data());
        strcpy_s(header.var_name, data_image->var_name.data());
        data_header_block->setData(reinterpret_cast<char *>(&header), sizeof(header));
        data_block->appendChild(data_header_block);
    }

    {
        std::shared_ptr<BlockNode> data_body_block = std::make_shared<BlockNode>(RTB_DataBody);
        data_body_block->setData(data_image->data);
        data_block->appendChild(data_body_block);
    }

    if(data_image->next != nullptr){
        data_block->appendChild(makeDataBlock(data_image->next));
    }

    return data_block;
}

void RuntimeFileWriter::write(std::shared_ptr<CallImage> call_image)
{
    assert(imp_->block_writer.isOpen());
    std::shared_ptr<BlockNode> call_image_node = std::make_shared<BlockNode>(RTB_CallImage);
    {
        std::shared_ptr<BlockNode> call_image_header_node = std::make_shared<BlockNode>(RTB_CallImageHeader);
        CallImageHeader call_image_header;
        strcpy_s(call_image_header.fun_name, call_image->fun_name.data());
        call_image_header.thread_id = call_image->thread_id;
        call_image_header.enter_time = call_image->enter_time;
        call_image_header.leave_time = call_image->leave_time;
        call_image_header.enter_tick = call_image->enter_tick;
        call_image_header.leave_tick = call_image->leave_tick;
        call_image_header_node->setData(reinterpret_cast<char *>(&call_image_header), sizeof(call_image_header));
        call_image_node->appendChild(call_image_header_node);
    }

    if(call_image->ret != nullptr)
    {
        std::shared_ptr<BlockNode> ret_data_node = std::make_shared<BlockNode>(RTB_ReturnData);
        ret_data_node->appendChild(makeDataBlock(call_image->ret));
        call_image_node->appendChild(ret_data_node);
    }

    for(std::shared_ptr<DataImage> data_image : call_image->arg_list)
    {
        std::shared_ptr<BlockNode> arg_data_node = std::make_shared<BlockNode>(RTB_ArgData);
        arg_data_node->appendChild(makeDataBlock(data_image));
        call_image_node->appendChild(arg_data_node);
    }

    for(std::shared_ptr<DataImage> data_image : call_image->data_list)
    {
        std::shared_ptr<BlockNode> arg_data_node = std::make_shared<BlockNode>(RTB_OtherData);
        arg_data_node->appendChild(makeDataBlock(data_image));
        call_image_node->appendChild(arg_data_node);
    }

    imp_->block_writer.writeBlock(call_image_node);
}

void RuntimeFileWriter::write(std::shared_ptr<ClassCallImage> class_call_image)
{
    std::shared_ptr<CallImage> call_image = class_call_image->call_image;
    std::shared_ptr<BlockNode> call_image_node = std::make_shared<BlockNode>(RTB_ClassCallImage);
    {
        std::shared_ptr<BlockNode> call_image_header_node = std::make_shared<BlockNode>(RTB_ClassCallImageHeader);
        ClassCallImageHeader class_call_image_header;
        strcpy_s(class_call_image_header.class_name, class_call_image->class_name.data());
        class_call_image_header.instance_id = class_call_image->instance_id;

        CallImageHeader &call_image_header = class_call_image_header.call_image_header;
        strcpy_s(call_image_header.fun_name, call_image->fun_name.data());
        call_image_header.thread_id = call_image->thread_id;
        call_image_header.enter_time = call_image->enter_time;
        call_image_header.leave_time = call_image->leave_time;
        call_image_header.enter_tick = call_image->enter_tick;
        call_image_header.leave_tick = call_image->leave_tick;
        call_image_header_node->setData(reinterpret_cast<char *>(&class_call_image_header), sizeof(class_call_image_header));
        call_image_node->appendChild(call_image_header_node);
    }

    if(call_image->ret != nullptr)
    {
        std::shared_ptr<BlockNode> ret_data_node = std::make_shared<BlockNode>(RTB_ReturnData);
        ret_data_node->appendChild(makeDataBlock(call_image->ret));
        call_image_node->appendChild(ret_data_node);
    }

    for(std::shared_ptr<DataImage> data_image : call_image->arg_list)
    {
        std::shared_ptr<BlockNode> arg_data_node = std::make_shared<BlockNode>(RTB_ArgData);
        arg_data_node->appendChild(makeDataBlock(data_image));
        call_image_node->appendChild(arg_data_node);
    }

    for(std::shared_ptr<DataImage> data_image : call_image->data_list)
    {
        std::shared_ptr<BlockNode> arg_data_node = std::make_shared<BlockNode>(RTB_OtherData);
        arg_data_node->appendChild(makeDataBlock(data_image));
        call_image_node->appendChild(arg_data_node);
    }

    imp_->block_writer.writeBlock(call_image_node);
}
