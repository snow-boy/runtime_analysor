#include <iostream>
#include <map>
#include <rt_serializer/runtimefilewriter.h>
#include <rt_serializer/runtimefilereader.h>
#include <rt_serializer/dataendec.h>
#include <rt_serializer/callimage_ex.h>
#include <rt_serializer/classcallimage_ex.h>

void print(std::shared_ptr<DataImage> data_image)
{
    std::cout << data_image->type_name << " "
              << data_image->var_name << ":" << data_image->data.size();

    if(data_image->next != nullptr){
        std::cout << " - ";
        print(data_image->next);
    }
}

void print(std::shared_ptr<CallImage> call_image)
{
    std::cout << call_image->timestamp << " ";
    if(call_image->ret != nullptr){
        print(call_image->ret);
    }
    std::cout << call_image->fun_name << "( ";

    int arg_index = 0;
    for(auto arg : call_image->arg_list){
        if(arg_index > 0){
            std::cout << ", ";
        }
        print(arg);
        arg_index ++;
    }

    std::cout << " )" << std::endl;
    for(auto data : call_image->data_list){
        std::cout << "- ";
        print(data);
        std::cout << std::endl;
    }

    std::cout << std::endl;
}

void print(std::shared_ptr<ClassCallImage> call_image)
{
    std::cout << call_image->call_image->timestamp << " ";
    std::cout << call_image->class_name << " ";
    std::cout << call_image->instance_id << " ";
    if(call_image->call_image->ret != nullptr){
        print(call_image->call_image->ret);
    }
    std::cout << call_image->call_image->fun_name << "( ";
    int arg_index = 0;
    for(auto arg : call_image->call_image->arg_list){
        if(arg_index > 0){
            std::cout << ", ";
        }
        print(arg);
        arg_index ++;
    }
    std::cout << " )" << std::endl;
    for(auto data : call_image->call_image->data_list){
        std::cout << "- ";
        print(data);
        std::cout << std::endl;
    }

    std::cout << std::endl;
}

int main(int argc, char **argv)
{
    if(argc != 2){
        std::cerr << "invalid arg count, one and only one file path arg is required" << std::endl;
        return -1;
    }

    RuntimeFileReader rt_reader;
    if(!rt_reader.read(argv[1])){
        std::cerr << "can not open this file: " << argv[1] << std::endl;
        return -1;
    }

    std::map<int64_t, std::pair<std::shared_ptr<CallImage>, std::shared_ptr<ClassCallImage>>> call_queue;

    CallImageList call_image_list = rt_reader.callImageList();

    for(std::shared_ptr<CallImage> call_image : call_image_list){
        call_queue[call_image->timestamp] = std::make_pair(call_image, nullptr);
    }

    ClassCallImageList class_call_image_list = rt_reader.classCallImageList();

    for(std::shared_ptr<ClassCallImage> call_image : class_call_image_list){
        call_queue[call_image->call_image->timestamp] = std::make_pair(nullptr, call_image);
    }

    for(auto e: call_queue){
        auto call_image = e.second;
        if(call_image.first != nullptr){
            print(call_image.first);
        }

        if(call_image.second != nullptr){
            print(call_image.second);
        }
    }

    return 0;
}
