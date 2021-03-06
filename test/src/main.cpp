#include <iostream>
#include <cassert>
#include <map>
#include <functional>
#include <type_traits>

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

void test02()
{
    int a = 1;
    int b = 2;

    std::cout << a << ", " << b << std::endl;

    auto data_image = AutoEncoder().encode(MakeData(a), MakeData(b));
    a = 0;
    b = 0;

    std::cout << a << ", " << b << std::endl;

    AutoDecoder().
    decode(data_image, &a, &b);

    std::cout << a << ", " << b << std::endl;
}

int main(int argc, char **argv)
{
//    test01(argc, argv);
    test02();

    std::cin.ignore();
    return 0;
}
