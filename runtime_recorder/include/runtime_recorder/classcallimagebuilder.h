#ifndef CLASSCALLIMAGEBUILDER_H
#define CLASSCALLIMAGEBUILDER_H

#include "callimagebuilder.h"

class ClassCallImageBuilder
{
public:
    ClassCallImageBuilder(const std::string &class_name, void *instance_id = nullptr):
        class_name_(class_name),
        instance_id_(reinterpret_cast<uint64_t>(instance_id))
    {}

    template <typename ... _ArgsT>
    std::shared_ptr<ClassCallImage> buildConstrucure(const std::string &fun_name, _ArgsT ...args)
    {
        std::shared_ptr<ClassCallImage> class_call_image = std::make_shared<ClassCallImage>();
        strcpy_s(class_call_image->class_name, class_name_.data());
        class_call_image->instance_id = instance_id_;
        class_call_image->method_type = MT_Constructure;
        CallImageBuilder<void, _ArgsT...> call_image_builder(fun_name);
        class_call_image->call_image = call_image_builder.buildCallImage(args...);

        return class_call_image;
    }

    std::shared_ptr<ClassCallImage> buildDestructure(const std::string &fun_name)
    {
        std::shared_ptr<ClassCallImage> class_call_image = std::make_shared<ClassCallImage>();
        strcpy_s(class_call_image->class_name, class_name_.data());
        class_call_image->instance_id = instance_id_;
        class_call_image->method_type = MT_Destructure;
        CallImageBuilder<void, void> call_image_builder(fun_name);
        class_call_image->call_image = call_image_builder.buildCallImage();

        return class_call_image;
    }

    template <typename _RetT, typename ... _ArgsT, typename = std::enable_if_t<!std::is_void<_RetT>::value>>
    std::shared_ptr<ClassCallImage> buildMethod(const std::string &fun_name, _RetT ret, _ArgsT ...args)
    {
        std::shared_ptr<ClassCallImage> class_call_image = std::make_shared<ClassCallImage>();
        strcpy_s(class_call_image->class_name, class_name_.data());
        class_call_image->instance_id = instance_id_;
        class_call_image->method_type = MT_Method;
        CallImageBuilder<_RetT, _ArgsT...> call_image_builder(fun_name);
        class_call_image->call_image = call_image_builder.buildCallImage(ret, args...);

        return class_call_image;
    }

    template <typename _RetT, typename ... _ArgsT, typename = std::enable_if_t<std::is_void<_RetT>::value>>
    std::shared_ptr<ClassCallImage> buildMethod(const std::string &fun_name, _ArgsT ...args)
    {
        std::shared_ptr<ClassCallImage> class_call_image = std::make_shared<ClassCallImage>();
        strcpy_s(class_call_image->class_name, class_name_.data());
        class_call_image->instance_id = instance_id_;
        class_call_image->method_type = MT_Method;
        CallImageBuilder<void, _ArgsT...> call_image_builder(fun_name);
        class_call_image->call_image = call_image_builder.buildCallImage(args...);

        return class_call_image;
    }

private:
    std::string class_name_;
    uint64_t instance_id_;
};


#endif // CLASSCALLIMAGEBUILDER_H
