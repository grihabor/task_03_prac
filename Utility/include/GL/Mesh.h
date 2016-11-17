#ifndef TASK_03_MESH_H_H
#define TASK_03_MESH_H_H

#include <vector>
#include <string>
#include "vec3.h"
#include "vec2.h"

namespace GL
{
    class Mesh
    {
        std::string name;

        std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
        std::vector< VM::vec3 > vertices;
        std::vector< VM::vec2 > uvs;
        std::vector< VM::vec3 > normals;

    public:
        Mesh(std::string filename);
    };
}

#endif //TASK_03_MESH_H_H
