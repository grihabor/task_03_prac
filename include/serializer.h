#ifndef TASK_03_SERIALIZER_H
#define TASK_03_SERIALIZER_H

#include <string>
#include <fstream>
#include <vector>
#include "Utility.h"

class BaseSerializer
{
protected:
    std::string filename;
    std::fstream file_stream;
    std::ios_base::openmode open_mode;
public:
    BaseSerializer(const std::string& filename, std::ios_base::openmode openmode)
            : filename(filename), open_mode(openmode)
    {}
    virtual ~BaseSerializer()
    {Close();}

    virtual bool Open()
    {
        Close();
        file_stream.open(filename, open_mode);
        if(file_stream.is_open()){
            std::cout << "Opened: " << filename << std::endl;
        } else {
            std::cout << "Failed to open: " << filename << std::endl;
        }
    }
    virtual bool Close()
    {
        if(file_stream.is_open()){
            file_stream.close();
        }
    }

    virtual bool process(std::vector<VM::vec2>& vector) = 0;
    virtual bool process(float& value) = 0;
};

class JSONSerializerReader : public BaseSerializer
{
public:
    JSONSerializerReader(const std::string& filename);
    virtual bool process(std::vector<VM::vec2>& vector) override;
    virtual bool process(float& value) override;
};

class JSONSerializerWriter : public BaseSerializer
{
    bool first;
public:
    JSONSerializerWriter(const std::string& filename);
    virtual bool process(std::vector<VM::vec2>& vector) override;
    virtual bool process(float& value) override;
    virtual ~JSONSerializerWriter();
};

class BinarySerializerReader : public BaseSerializer
{
public:
    BinarySerializerReader(const std::string& filename);
    virtual bool process(std::vector<VM::vec2>& vector) override;
    virtual bool process(float& value) override;
};

class BinarySerializerWriter : public BaseSerializer
{
public:
    BinarySerializerWriter(const std::string& filename);
    virtual bool process(std::vector<VM::vec2>& vector) override;
    virtual bool process(float& value) override;
};



#endif //TASK_03_SERIALIZER_H
