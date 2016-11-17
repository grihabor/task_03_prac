#include <GL/glew.h>
#include "Mesh.h"
#include <exception>
#include <sstream>

struct FileException : std::exception
{
    std::string exception_string;
    const char* what() const noexcept override
    {
        return exception_string.c_str();
    }

    FileException(std::string filename)
            :exception_string(std::string("Failed to open file: ") + filename + "\n")
    {}
};

struct ObjException : std::exception
{
    std::string exception_string;
    const char* what() const noexcept override
    {
        return exception_string.c_str();
    }

    ObjException(std::string exception_string)
            :exception_string(exception_string)
    {}
};

GL::Mesh::Mesh(std::string filename) {
    filename = std::string("mesh/") + filename + ".obj";
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw FileException(filename);
    }

    std::string line_string;
    while (std::getline(file, line_string)) {

        // skip comments
        if(line_string[0] == '#'){
            continue;
        }

        std::stringstream line(line_string);
        std::string lineHeader;
        line >> lineHeader;

        if (lineHeader == "v") {
            VM::vec3 vertex;
            line >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);

        } else if (lineHeader == "vt") {
            VM::vec2 uv;
            line >> uv.x >> uv.y;
            uvs.push_back(uv);

        } else if (lineHeader == "vn") {
            VM::vec3 normal;
            line >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        } else if (lineHeader == "f") {
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            char slash;
            line >> vertexIndex[0] >> slash
                 >> uvIndex[0] >> slash
                 >> normalIndex[0]
                 >> vertexIndex[1] >> slash
                 >> uvIndex[1] >> slash
                 >> normalIndex[1]
                 >> vertexIndex[2] >> slash
                 >> uvIndex[2] >> slash
                 >> normalIndex[2];

            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices.push_back(uvIndex[0]);
            uvIndices.push_back(uvIndex[1]);
            uvIndices.push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        } else if(lineHeader == "o"){
            // object header
            // parse only one object in the file
            line >> name;
        } else if(lineHeader == "s"){
            // smooth shading
            // skip
            continue;
        } else if(lineHeader == "mtllib"){
            // mtl library to use
            // skip
            std::string mtl_file;
            line >> mtl_file;
        } else if(lineHeader == "usemtl"){
            // material ot use
            // skip
            std::string material;
            line >> material;
        } else {
            throw ObjException("Unknown tag: " + lineHeader + "\n" +
                               "    in line: " + line_string + "\n");
        }
    }

}

/*
 * TODO: refactor main.cpp -> scene.cpp
 *
 * TODO: create constructor and draw functions for Mesh
 *
void GL::Mesh::Draw()
{

}

*/