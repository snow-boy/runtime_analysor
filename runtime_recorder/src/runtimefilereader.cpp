#include "runtimefilereader.h"
#include <fstream>
#include <cassert>
#include <iostream>
#include "runtimefile_types.h"

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

bool RuntimeFileReader::read(const std::string &file_path)
{
    std::ifstream stream(file_path, std::ios::in|std::ios::binary);
    if(stream.bad()){
        return false;
    }

    FileHeader file_header;
    stream.read(reinterpret_cast<char *>(&file_header), sizeof(file_header));
    if(std::string(file_header.tag) != std::string(FILE_TAG)){
        return false;
    }
    imp_->version = file_header.version;
    if(imp_->version != std::string(CURRENT_VERSION)){
        return false;
    }

    while(stream.peek() != std::ifstream::traits_type::eof())
    {
        BlockHeader block_header;
        stream.read(reinterpret_cast<char *>(&block_header), sizeof(block_header));
        switch (block_header.block_type)
        {
        case BT_CallImage:
        {
            std::shared_ptr<CallImage> call_image = std::make_shared<CallImage>();
            stream.read(call_image->fun_name, sizeof(call_image->fun_name));
            stream.read(reinterpret_cast<char *>(&call_image->timestamp),
                        sizeof(call_image->timestamp));
            stream.read(reinterpret_cast<char *>(&call_image->thread_id),
                        sizeof(call_image->thread_id));

            int32_t param_count = 0;
            stream.read(reinterpret_cast<char *>(&param_count), sizeof(param_count));

            std::vector<std::shared_ptr<Param>> arg_list(param_count);
            for(int i = 0; i < param_count; ++i){
                std::shared_ptr<Param> param = std::make_shared<Param>();
                ParamBlock param_block;
                stream.read(reinterpret_cast<char *>(&param_block), sizeof (param_block));
                memcpy(param->type_name, param_block.type_name, sizeof(param_block.type_name));

                int param_data_size = param_block.header.byte_size - sizeof(ParamBlock);
                param->data.resize(param_data_size);
                stream.read(param->data.data(), param->data.size());

                if(param_block.header.block_type == BT_ParamReturn){
                    call_image->ret_param = param;
                }
                else if(param_block.header.block_type == BT_ParamArg){
                    arg_list[param_block.index] = param;
                }
                else{
                    assert(false);
                }
            }

            for(auto param : arg_list){
                call_image->arg_list.push_back(param);
            }

            imp_->call_image_list.push_back(call_image);
            break;
        }
        case BT_ClassCallImage:
        {
            std::shared_ptr<ClassCallImage> class_call_image = std::make_shared<ClassCallImage>();
            class_call_image->call_image = std::make_shared<CallImage>();
            stream.read(class_call_image->class_name, sizeof(class_call_image->class_name));
            stream.read(reinterpret_cast<char *>(&class_call_image->instance_id),
                        sizeof(class_call_image->instance_id));
            stream.read(reinterpret_cast<char *>(&class_call_image->method_type),
                        sizeof(class_call_image->method_type));
            stream.read(class_call_image->call_image->fun_name, sizeof(class_call_image->call_image->fun_name));
            stream.read(reinterpret_cast<char *>(&class_call_image->call_image->timestamp),
                        sizeof(class_call_image->call_image->timestamp));
            stream.read(reinterpret_cast<char *>(&class_call_image->call_image->thread_id),
                        sizeof(class_call_image->call_image->thread_id));

            int32_t param_count = 0;
            stream.read(reinterpret_cast<char *>(&param_count), sizeof(param_count));

            std::vector<std::shared_ptr<Param>> arg_list(param_count);
            for(int i = 0; i < param_count; ++i){
                std::shared_ptr<Param> param = std::make_shared<Param>();
                ParamBlock param_block;
                stream.read(reinterpret_cast<char *>(&param_block), sizeof (param_block));
                memcpy(param->type_name, param_block.type_name, sizeof(param_block.type_name));

                int param_data_size = param_block.header.byte_size - sizeof(ParamBlock);
                param->data.resize(param_data_size);
                stream.read(param->data.data(), param->data.size());

                if(param_block.header.block_type == BT_ParamReturn){
                    class_call_image->call_image->ret_param = param;
                }
                else if(param_block.header.block_type == BT_ParamArg){
                    arg_list[param_block.index] = param;
                }
                else{
                    assert(false);
                }
            }

            for(auto param : arg_list){
                class_call_image->call_image->arg_list.push_back(param);
            }

            imp_->class_call_image_list.push_back(class_call_image);
            break;
        }
        default:
            break;
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
