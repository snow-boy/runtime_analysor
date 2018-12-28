#include <runtime_recorder/callimagebuilder.h>
#include <runtime_recorder/classcallimagebuilder.h>
#include <runtime_recorder/runtimefilewriter.h>
#include <runtime_recorder/runtimefilereader.h>

int main(int argc, char **argv)
{
    std::string file_path("e:/test.data");

    {
        RuntimeFileWriter file_writer;
        file_writer.open(file_path);
        file_writer.writeClassCallImage(ClassCallImageBuilder("TestClass", argv).buildMethod<void, int>("test", 12));
        file_writer.writeCallImage(CallImageBuilder<void, int>("test").buildCallImage(100));
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
