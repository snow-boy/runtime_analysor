#include <runtime_recorder/runtimefilewriter.h>
#include <runtime_recorder/runtimefilereader.h>
#include <runtime_recorder/runtime_utils.h>
#include <runtime_recorder/dataendec.h>

int main(int argc, char **argv)
{
    std::string file_path("test.data");

    {
        RuntimeFileWriter file_writer;
        file_writer.open(file_path);

        std::shared_ptr<CallImage> call_image = allocCallImage("main");
        int ret = 0;
        call_image->ret = EncodeData(ret);
        call_image->arg_list.push_back(EncodeData(argc));
        file_writer.writeCallImage(call_image);

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
