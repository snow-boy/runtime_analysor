#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <io.h>
#include <vector>
#include <string>

#include <rt_serializer/runtimefilewriter.h>
#include <rt_serializer/runtimefilereader.h>
#include <rt_serializer/dataendec.h>
#include <rt_serializer/callimage_ex.h>
#include <rt_serializer/classcallimage_ex.h>

void findFiles(const std::string &path, const std::string &pattern, std::vector<std::string>& files)
{
    intptr_t h_file = 0;
    struct _finddata_t fileinfo;
    std::string p;
    if((h_file = _findfirst(p.assign(path).append("\\").append(pattern).c_str(),&fileinfo)) !=  -1)
    {
        do
        {
            if((fileinfo.attrib &  _A_SUBDIR))
            {
                if(strcmp(fileinfo.name,".") != 0  &&  strcmp(fileinfo.name,"..") != 0)
                    findFiles( p.assign(path).append("\\").append(fileinfo.name), pattern, files );
            }
            else
            {
                files.push_back(p.assign(path).append("\\").append(fileinfo.name) );
            }
        }while(_findnext(h_file, &fileinfo)  == 0);

        _findclose(h_file);
    }
}

std::string makeTab(int count = 1)
{
    std::string tab_str;
    for(int i = 0; i < count; ++i){
        tab_str += "    ";
    }
    return tab_str;
}

std::string buildInvoker(const ClassCallImageList & call_list)
{
    std::ostringstream stream;

    std::list<std::string> class_list = call_list.classList();
    for(std::string &class_name: class_list)
    {
        stream << "template<>" << std::endl;
        stream << "class ClassInvoker<" << class_name << ">" << std::endl;
        stream << "{" << std::endl;
        stream << "public:" << std::endl;
        stream << makeTab(1) << "ClassInvoker<" << class_name << ">(" << class_name << " *instance):" <<std::endl;
        stream << makeTab(2) << "instance_(instance)" << std::endl;
        stream << makeTab(1) << "{}" << std::endl;
        stream << makeTab(1) << "void invoke(std::shared_ptr<ClassCallImage> class_call_image)" << std::endl;
        stream << makeTab(1) << "{" << std::endl;
        stream << makeTab(2) << "do" << std::endl;
        stream << makeTab(2) << "{" << std::endl;
        for(std::shared_ptr<ClassCallImage> class_call_image: call_list.getCallImagesByClass(class_name))
        {
            stream << makeTab(3) << "if(std::string(class_call_image->call_image->fun_name) == \"" << class_call_image->call_image->fun_name
                   << "\"){" << std::endl;

            auto it = class_call_image->call_image->arg_list.begin();
            for(size_t i = 0; i < class_call_image->call_image->arg_list.size(); ++i)
            {
                if(i == 0){
                    stream << makeTab(4) << "auto it = class_call_image->call_image->arg_list.begin();" << std::endl;
                }

                stream << makeTab(4) << "auto arg" << i << " = *it;" << std::endl;
                stream << makeTab(4) << "DataDecoder<" << (*it)->type_name << "> decoder" << i << ";" << std::endl;
                ++it;

                stream << makeTab(4) << "it++;" << std::endl;
            }
            stream << makeTab(4) << "instance_->"
                   << class_call_image->call_image->fun_name << "(";

            for(size_t i = 0; i < class_call_image->call_image->arg_list.size(); ++i)
            {
                stream << "decoder" << i << ".decode(";
                stream << "arg" << i;
                stream << ")";
                if(i != class_call_image->call_image->arg_list.size() - 1){
                    stream << ", ";
                }
            }

            stream << ");" << std::endl;
            stream << makeTab(4) << "break;" << std::endl;
            stream << makeTab(3) << "}" << std::endl;
        }
        stream << makeTab(3) << "assert(false);" << std::endl;
        stream << makeTab(2) << "}while(0);" << std::endl;
        stream << makeTab(1) << "}" << std::endl;

        stream << "private:" << std::endl;
        stream << makeTab(1) << class_name << "*instance_;" << std::endl;
        stream << "};" << std::endl;
    }

    return stream.str();
}

std::string buildInvoker(const CallImageList & call_list)
{
    std::ostringstream stream;

    stream << "void invokeCall(std::shared_ptr<CallImage> call_image)" << std::endl
           << "{" << std::endl;

    stream << makeTab(1) << "do" << std::endl;
    stream << makeTab(1) << "{" << std::endl;
    for(std::shared_ptr<CallImage> call_image: call_list)
    {
        stream << makeTab(2) << "if(std::string(class_call_image->call_image->fun_name) == \"" << call_image->fun_name
               << "\"){" << std::endl;

        auto it = call_image->arg_list.begin();
        for(size_t i = 0; i < call_image->arg_list.size(); ++i)
        {
            if(i == 0){
                stream << makeTab(3) << "auto it = class_call_image->call_image->arg_list.begin();" << std::endl;
            }

            stream << makeTab(3) << "auto arg" << i << " = *it;" << std::endl;
            stream << makeTab(3) << "DataDecoder<" << (*it)->type_name << "> decoder" << i << ";" << std::endl;
            ++it;

            stream << makeTab(3) << "it++;" << std::endl;
        }
        stream << makeTab(3) << "instance_->"
               << call_image->fun_name << "(";

        for(size_t i = 0; i < call_image->arg_list.size(); ++i)
        {
            stream << "decoder" << i << ".decode(";
            stream << "arg" << i;
            stream << ")";
            if(i != call_image->arg_list.size() - 1){
                stream << ", ";
            }
        }

        stream << ");" << std::endl;
        stream << makeTab(3) << "break;" << std::endl;
        stream << makeTab(2) << "}" << std::endl;
    }
    stream << makeTab(2) << "assert(false);" << std::endl;
    stream << makeTab(1) << "}while(0);" << std::endl;

    stream << "}" << std::endl;

    return stream.str();
}

void buildInvoker(const std::string &file_path,
                  const CallImageList &call_image_list,
                  const ClassCallImageList &class_call_image_list)
{
    std::ofstream stream(file_path);
    if(stream.bad()){
        std::cerr << "can not open " << file_path << std::endl;
        exit(-1);
    }

    stream << "#pragma once" << std::endl;
    stream << "template<typename T> class ClassInvoker{};" << std::endl;
    stream << buildInvoker(class_call_image_list) << std::endl;
    stream << buildInvoker(call_image_list) << std::endl;

    stream << "class AutoInvoker" << std::endl
              << "{" << std::endl
              << "public:" << std::endl
              << "    template<typename T>" << std::endl
              << "    void addInstance(T *instance)" << std::endl
              << "    {" << std::endl
              << "        instance_map_[typeid (T).name()] = [=](std::shared_ptr<ClassCallImage> call_image){" << std::endl
              << "            ClassInvoker<T>(instance).invoke(call_image);" << std::endl
              << "        };" << std::endl
              << "    }" << std::endl
              << "    void invoke(std::shared_ptr<ClassCallImage> call_image)" << std::endl
              << "    {" << std::endl
              << "        assert(instance_map_.count(call_image->class_name) > 0);" << std::endl
              << "        instance_map_[call_image->class_name](call_image);" << std::endl
              << "    }" << std::endl
              << "    void invoke(std::shared_ptr<CallImage> call_image)" << std::endl
              << "    {" << std::endl
              << "        invokeCall(call_image);" << std::endl
              << "    }" << std::endl
              << "private:" << std::endl
              << "    std::map<std::string, std::function<void (std::shared_ptr<ClassCallImage>)>> instance_map_;" << std::endl
              << "};" << std::endl;
}

int main(int argc, char **argv)
{
    if(argc == 3){
        std::string rt_path = argv[1];
        std::string out_path = argv[2];

        std::map<std::string, std::shared_ptr<CallImage>> call_image_set;
        std::map<std::string, std::shared_ptr<ClassCallImage>> class_call_image_set;

        RuntimeFileReader rt_reader;
        if(!rt_reader.read(rt_path)){
            std::cerr << "can not open this file: " << rt_path << std::endl;
            return -1;
        }

        for(std::shared_ptr<CallImage> e: rt_reader.callImageList()){
            if(call_image_set.count(e->fun_name) == 0){
                call_image_set[e->fun_name] = e;
            }
        }

        for(std::shared_ptr<ClassCallImage> e: rt_reader.classCallImageList()){
            if(class_call_image_set.count(e->call_image->fun_name) == 0){
                class_call_image_set[e->call_image->fun_name] = e;
            }
        }

        CallImageList call_image_list;
        for(auto e: call_image_set){
            call_image_list.push_back(e.second);
        }

        ClassCallImageList class_call_image_list;
        for(auto e: class_call_image_set){
            class_call_image_list.push_back(e.second);
        }

        buildInvoker(out_path, call_image_list, class_call_image_list);
    }
    else if(argc == 4){
        std::string rt_path = argv[1];
        std::string pattern = argv[2];
        std::string out_path = argv[3];

        std::vector<std::string> files;
        findFiles(rt_path, pattern, files);

        std::map<std::string, std::shared_ptr<CallImage>> call_image_set;
        std::map<std::string, std::shared_ptr<ClassCallImage>> class_call_image_set;
        for(std::string &file_path: files)
        {
            RuntimeFileReader rt_reader;
            if(!rt_reader.read(file_path)){
                std::cerr << "can not open this file: " << argv[1] << std::endl;
                return -1;
            }

            for(std::shared_ptr<CallImage> e: rt_reader.callImageList()){
                if(call_image_set.count(e->fun_name) == 0){
                    call_image_set[e->fun_name] = e;
                }
            }

            for(std::shared_ptr<ClassCallImage> e: rt_reader.classCallImageList()){
                if(class_call_image_set.count(e->call_image->fun_name) == 0){
                    class_call_image_set[e->call_image->fun_name] = e;
                }
            }
        }

        CallImageList call_image_list;
        for(auto e: call_image_set){
            call_image_list.push_back(e.second);
        }

        ClassCallImageList class_call_image_list;
        for(auto e: class_call_image_set){
            class_call_image_list.push_back(e.second);
        }

        buildInvoker(out_path, call_image_list, class_call_image_list);
    }
    else{
        std::cerr << "usage: rt_builder [rt_file_path]/[rt_folder_path pattern] out_path" << std::endl;
    }

    std::cout << "done!" << std::endl;

    return 0;
}
