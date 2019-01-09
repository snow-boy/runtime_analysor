#ifndef AUTO_RT_WRITER_H
#define AUTO_RT_WRITER_H

template <typename _DataType, typename _WriteType>
class AutoWriter
{
public:
    AutoWriter(_DataType data, _WriteType writer):
        data_(data),
        writer_(writer)
    {}

    ~AutoWriter()
    {
        data_->setLeave();
        writer_->write(data_);
    }

private:
    _DataType data_;
    _WriteType writer_;
};

#define WriteLater(data, writer) AutoWriter<decltype(data), decltype(writer)> auto_writer(data, writer)

#endif // AUTO_RT_WRITER_H
