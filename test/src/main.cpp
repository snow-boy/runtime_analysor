#include <iostream>
#include <cassert>
#include <map>
#include <functional>

#include <rt_serializer/runtimefilewriter.h>
#include <rt_serializer/runtimefilereader.h>
#include <rt_serializer/dataendec.h>
#include <rt_serializer/callimage_ex.h>
#include <rt_serializer/classcallimage_ex.h>

void test01(int argc, char **argv)
{
    class TestA
    {
    public:
        void test(int a)
        {

        }
    };

    std::string file_path("test.data");

    {
        RuntimeFileWriter file_writer;
        file_writer.open(file_path);

        std::shared_ptr<CallImageEx> call_image = std::make_shared<CallImageEx>("main");
        int ret = 0;
        call_image->setReturn(MakeData(ret));
        call_image->addArg(MakeData(argc));
        file_writer.write(call_image);

        TestA test_a;
        std::shared_ptr<ClassCallImageEx> class_call_image = std::make_shared<ClassCallImageEx>("test", &test_a);
        int arg = 2;
        class_call_image->addArg(MakeData(arg));
        file_writer.write(class_call_image);

        file_writer.close();
    }

    {
        RuntimeFileReader file_reader;
        file_reader.read(file_path);

        std::string version = file_reader.version();
        auto call_image_list = file_reader.callImageList();
        auto func_list = call_image_list.functions();
        auto threads = call_image_list.threads();
        auto class_call_image_list = file_reader.classCallImageList();
        auto class_names = class_call_image_list.classList();
    }
}

namespace aa{

class A
{
public:
    void test1(int a)
    {
        std::cout << __FUNCDNAME__ << " " << a << std::endl;
    }

    void test2(float a)
    {
        std::cout << __FUNCDNAME__ << " " << a << std::endl;
    }
};

}

template <>
struct DataDecoder<int>
{
    int decode(std::shared_ptr<DataImage> data_image)
    {
        return 100;
    }
};

template<typename T>
class ClassInvoker
{};

template<>
class ClassInvoker<aa::A>
{
public:
    ClassInvoker<aa::A>(aa::A *instance):
        instance_(instance)
    {}

    void invoke(std::shared_ptr<ClassCallImage> class_call_image)
    {
        do
        {
            if(std::string(class_call_image->call_image->fun_name) == "A::test1"){
                instance_->aa::A::test1(DataDecoder<int>().decode(*class_call_image->call_image->arg_list.begin()));
                break;
            }

            if(std::string(class_call_image->call_image->fun_name) == "A::test2"){
                instance_->aa::A::test1(DataDecoder<float>().decode(*class_call_image->call_image->arg_list.begin()));
                break;
            }

            assert(false);

        }while(0);
    }

private:
    aa::A *instance_;
};

void invokeCall(std::shared_ptr<CallImage> call_image)
{

}

class AutoInvoker
{
public:
    template<typename T>
    void addInstance(T *instance)
    {
        instance_map_[typeid (T).name()] = [=](std::shared_ptr<ClassCallImage> call_image){
            ClassInvoker<T>(instance)->invoke(call_image);
        };
    }

    void invoke(std::shared_ptr<ClassCallImage> call_image)
    {
        assert(instance_map_.count(call_image->class_name) > 0);
        instance_map_[call_image->class_name](call_image);
    }

    void invoke(std::shared_ptr<CallImage> call_image)
    {
        invokeCall(call_image);
    }

private:
    std::map<std::string, std::function<void (std::shared_ptr<ClassCallImage>)>> instance_map_;
};

template<void * _Addr>
struct Ad
{
    void say()
    {
        std::cout << "hi~, I'm default Ad say" << std::endl;
    }
};

const char *str = "A::test1";


template <typename T>
void invoke(T *instance, std::shared_ptr<ClassCallImage> call_image)
{
    ClassInvoker<T>(instance).invoke(call_image);
}

void invoke(std::shared_ptr<CallImage> call_image)
{
//    do
//    {
//        if(std::string(call_image->fun_name) == "A::test1"){
//            A::test1(DataDecoder<int>().decode(*class_call_image->call_image->arg_list.begin()));
//            break;
//        }

//        if(std::string(class_call_image->call_image->fun_name) == "A::test2"){
//            instance_->aa::A::test1(DataDecoder<float>().decode(*class_call_image->call_image->arg_list.begin()));
//            break;
//        }

//        assert(false);

//    }while(0);
}

int main(int argc, char **argv)
{
//    test01(argc, argv);

    std::cout << __FUNCDNAME__ << std::endl;
    std::cout << __FUNCTION__ << std::endl;

    aa::A a;

    {
        std::shared_ptr<ClassCallImageEx> call_image = std::make_shared<ClassCallImageEx>("A::test1", &a);
        int d = 10;
        call_image->addArg(MakeData(d));

        ClassInvoker<aa::A> invoker(&a);
        invoker.invoke(call_image);
    }

    return 0;
}
