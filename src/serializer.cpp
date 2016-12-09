#include "serializer.h"
#include <string>

JSONSerializerReader::JSONSerializerReader(const std::string &filename)
        : BaseSerializer(filename, std::ios_base::in)
{
    Open();
}
JSONSerializerWriter::JSONSerializerWriter(const std::string &filename)
        : BaseSerializer(filename, std::ios_base::out), first(true)
{
    Open();
}
BinarySerializerReader::BinarySerializerReader(const std::string &filename)
        : BaseSerializer(filename, std::ios_base::in | std::ios_base::binary)
{
    Open();
}
BinarySerializerWriter::BinarySerializerWriter(const std::string &filename)
        : BaseSerializer(filename, std::ios_base::out | std::ios_base::binary)
{
    Open();
}

/* JSON READER */

bool JSONSerializerReader::process(float& value) {
    int buf;
    std::string number;
    while((buf = file_stream.get()) != EOF && buf != ','){
        if(buf >= '0' && buf <= '9' || buf == '.' || buf == '-' || buf == 'e'){
            number.push_back(buf);
        } else if(!std::isspace(buf) && buf != '[' && buf != ']'){
            std::cout << "unexpected character \'" << char(buf) << "\'" << std::endl;
            return false;
        }
    }
    //std::cout << "Got string: " << number << std::endl;
    value = std::strtof(number.c_str(), NULL);
    return true;
}

bool JSONSerializerReader::process(std::vector<VM::vec2> &vec) {
    if(vec.size() > 0){
        std::cout << "Be careful: serializer overwrites vector content!" << std::endl;
    }
    vec.clear();

    int buf;
    int size;

    // skip one float (magic start number)
    {
        float t;
        process(t);
    }

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
    } while (buf != EOF);
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

bool JSONSerializerWriter::process(std::vector<VM::vec2> &vec) {

    // write one float (magic start number)
    if(first){
        file_stream << "[0";
        first = false;
    } else {
        file_stream << ",0";
    }

    for(int i = 0; i < vec.size(); ++i){
        file_stream << ",[" << vec[i].x << ',' << vec[i].y << ']';
    }

    return true;
}

JSONSerializerWriter::~JSONSerializerWriter() {
    file_stream << ']';
}

/* BINARY WRITER */

bool BinarySerializerWriter::process(float &value) {
    file_stream.write(reinterpret_cast<char*>(&value), sizeof(value));
    return true;
}

bool BinarySerializerWriter::process(std::vector<VM::vec2> &vec) {
    float s = vec.size() + 0.1f;
    std::cout << "Writing vector of size " << int(s) << std::endl;
    process(s);
    for(int i = 0; i < vec.size(); ++i){
        process(vec[i].x);
        process(vec[i].y);
    }
    return true;
}

/* BINARY READER */

bool BinarySerializerReader::process(float &value) {
    file_stream.read(reinterpret_cast<char*>(&value), sizeof(value));
    return true;
}

bool BinarySerializerReader::process(std::vector<VM::vec2> &vec) {
    if(vec.size() > 0){
        std::cout << "Be careful: serializer overwrites vector content!" << std::endl;
    }
    vec.clear();

    uint vec_size;
    float t;
    process(t);
    vec_size = uint(t);

    std::cout << "Serializer: Reading vector of size " << vec_size << std::endl;
    for(int i = 0; i < vec_size; ++i){
        VM::vec2 t;
        if(!process(t.x) || !process(t.y)){
            std::cout << "Unexpected EOF!" << std::endl;
            return false;
        }
        vec.push_back(t);
    }
    return true;
}