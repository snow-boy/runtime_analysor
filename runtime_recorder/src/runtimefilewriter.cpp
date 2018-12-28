#include "runtimefilewriter.h"

#include <fstream>
#include <iostream>
#include "runtimefile_types.h"

class RuntimeFileWriter::Imp
{
public:
    std::ofstream stream;
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
    imp_->stream.open(file_path);
    if(imp_->stream.bad()){
        return false;
    }

    FileHeader header;
    memset(&header, 0, sizeof(header));

    std::string tag = FILE_TAG;
    strcpy_s(header.tag, tag.data());

    std::string version = CURRENT_VERSION;
    strcpy_s(header.version, version.data());

    imp_->stream.write(reinterpret_cast<char *>(&header), sizeof(header));

    return true;
}

void RuntimeFileWriter::close()
{
    imp_->stream.close();
}

static std::shared_ptr<ParamBlock> buildParamBlock(std::shared_ptr<Param> param, BlockType block_type, int index)
{
    int data_size = static_cast<int>(sizeof(ParamBlock) + param->data.size());
    std::shared_ptr<ParamBlock> param_block(
                reinterpret_cast<ParamBlock *>(malloc(data_size)),
                [](ParamBlock *block){ free(block); });

    param_block->header.block_type = block_type;
    param_block->header.byte_size = data_size;
    param_block->index  = index;
    memcpy(param_block->type_name, param->type_name, sizeof(param->type_name));
    memcpy(param_block->data, param->data.data(), param->data.size());

    return param_block;
}

void RuntimeFileWriter::writeCallImage(std::shared_ptr<CallImage> call_image)
{
    std::unique_ptr<CallImageBlock> image_block_ptr = std::make_unique<CallImageBlock>();
    memcpy(image_block_ptr->fun_name, call_image->fun_name, sizeof(call_image->fun_name));
    image_block_ptr->timestamp = call_image->timestamp;
    image_block_ptr->thread_id = call_image->thread_id;
    image_block_ptr->header.block_type = BT_CallImage;
    image_block_ptr->header.byte_size = sizeof(CallImageBlock);
    image_block_ptr->param_count = 0;

    std::vector<std::shared_ptr<ParamBlock>> param_blocks;

    if(call_image->ret_param != nullptr)
    {
        std::shared_ptr<ParamBlock> param_block =
                buildParamBlock(call_image->ret_param, BT_ParamReturn, static_cast<int>(param_blocks.size()));
        param_blocks.push_back(param_block);
        image_block_ptr->header.byte_size += param_block->header.byte_size;
        image_block_ptr->param_count++;
    }

    for(auto param : call_image->arg_list){
        std::shared_ptr<ParamBlock> param_block = buildParamBlock(
                    param, BT_ParamArg, static_cast<int>(param_blocks.size()));
        param_blocks.push_back(param_block);
        image_block_ptr->header.byte_size += param_block->header.byte_size;
        image_block_ptr->param_count++;
    }

    imp_->stream.write(reinterpret_cast<char *>(image_block_ptr.get()), sizeof (CallImageBlock));
    for(std::shared_ptr<ParamBlock> param_block : param_blocks){
        imp_->stream.write(reinterpret_cast<char *>(param_block.get()), param_block->header.byte_size);
    }
}

void RuntimeFileWriter::writeClassCallImage(std::shared_ptr<ClassCallImage> class_call_image)
{
    std::unique_ptr<ClassCallImageBlock> image_block_ptr = std::make_unique<ClassCallImageBlock>();
    memcpy(image_block_ptr->class_name,
           class_call_image->class_name, sizeof(class_call_image->class_name));
    image_block_ptr->instance_id = class_call_image->instance_id;
    memcpy(image_block_ptr->fun_name,
           class_call_image->call_image->fun_name, sizeof(class_call_image->call_image->fun_name));
    image_block_ptr->method_type = class_call_image->method_type;
    image_block_ptr->timestamp = class_call_image->call_image->timestamp;
    image_block_ptr->thread_id = class_call_image->call_image->thread_id;
    image_block_ptr->header.block_type = BT_ClassCallImage;
    image_block_ptr->header.byte_size = sizeof(ClassCallImageBlock);
    image_block_ptr->param_count = 0;

    std::vector<std::shared_ptr<ParamBlock>> param_blocks;

    if(class_call_image->call_image->ret_param != nullptr)
    {
        std::shared_ptr<ParamBlock> param_block =
                buildParamBlock(class_call_image->call_image->ret_param, BT_ParamReturn,
                                static_cast<int>(param_blocks.size()));
        param_blocks.push_back(param_block);
        image_block_ptr->header.byte_size += param_block->header.byte_size;
        image_block_ptr->param_count++;
    }

    for(auto param : class_call_image->call_image->arg_list){
        std::shared_ptr<ParamBlock> param_block = buildParamBlock(param, BT_ParamArg,
                                      static_cast<int>(param_blocks.size()));
        param_blocks.push_back(param_block);
        image_block_ptr->header.byte_size += param_block->header.byte_size;
        image_block_ptr->param_count++;
    }

    imp_->stream.write(reinterpret_cast<char *>(image_block_ptr.get()), sizeof (ClassCallImageBlock));
    for(std::shared_ptr<ParamBlock> param_block : param_blocks){
        imp_->stream.write(reinterpret_cast<char *>(param_block.get()), param_block->header.byte_size);
    }
}
