#include <runtime_recorder/runtimefilewriter.h>
#include <runtime_recorder/runtimefilereader.h>
#include <runtime_recorder/dataendec.h>
#include <runtime_recorder/callimage_ex.h>
#include <runtime_recorder/classcallimage_ex.h>

class TestA
{
public:
    void test(int a)
    {

    }
};

int main(int argc, char **argv)
{
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

    return 0;
}
