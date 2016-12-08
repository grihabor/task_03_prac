#include "serializer.h"
#include <string>

JSONSerializerReader::JSONSerializerReader(const std::string &filename)
        : BaseSerializer(filename, std::ios_base::in)
{}
JSONSerializerWriter::JSONSerializerWriter(const std::string &filename)
        : BaseSerializer(filename, std::ios_base::out), first(true)
{}
BinarySerializerReader::BinarySerializerReader(const std::string &filename)
        : BaseSerializer(filename, std::ios_base::in | std::ios_base::binary)
{}
BinarySerializerWriter::BinarySerializerWriter(const std::string &filename)
        : BaseSerializer(filename, std::ios_base::out | std::ios_base::binary)
{}

/* JSON READER */

bool JSONSerializerReader::process(float& value) {
    int buf;
    std::string number;
    while((buf = file_stream.get()) != EOF && buf != ','){
        if(buf >= '0' && buf <= '9' || buf == '.'){
            number.push_back(buf);
        } else if(!std::isspace(buf) && buf != '[' && buf != ']'){
            // unexpected character - error!
            return false;
        }
    }
    std::cout << "Got string: " << number << std::endl;
    value = std::strtof(number.c_str(), NULL);
    return true;
}

bool JSONSerializerReader::process(std::vector<VM::vec2> &vec) {
    if(vec.size() > 0){
        std::cout << "Be careful: serializer overwrites vector content!" << std::endl;
    }
    vec.clear();

    int buf;

    do {
        // skip spaces
        while((buf = file_stream.get()) != EOF && std::isspace(buf))
        {}
        // if not [float, float] then break
        if (buf != '[') {
            break;
        }
        float x, y;
        process(x);
        process(y);
        vec.push_back(VM::vec2(x, y));
    } while ((buf = file_stream.get()) != EOF);
    std::cout << "Serializer: got " << vec.size() << " vec2 instances" << std::endl;
    return true;
}

/* JSON WRITER */

bool JSONSerializerWriter::process(float& value) {
    if(first){
        file_stream << '[' << value;
        first = false;
    } else {
        file_stream << ',' << value;
    }
    return true;
}

bool JSONSerializerWriter::process(std::vector<VM::vec2> &vector) {
    return false;
}

JSONSerializerWriter::~JSONSerializerWriter() {
    file_stream << ']';
}

bool BinarySerializerReader::process(float &value) {
    return false;
}

bool BinarySerializerReader::process(std::vector<VM::vec2> &vector) {
    return false;
}

bool BinarySerializerWriter::process(float &value) {
    return false;
}

bool BinarySerializerWriter::process(std::vector<VM::vec2> &vector) {
    return false;
}