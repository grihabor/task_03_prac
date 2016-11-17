#ifndef TASK_03_MESH_H_H
#define TASK_03_MESH_H_H

#include <vector>
#include <string>
#include "vec3.h"
#include "vec2.h"
#include "Camera.h"
#include "GL.h"

namespace GL
{
    class Mesh
    {
    protected:
        GLuint shader;
        GLuint vao;

    private:
        void CreateVAO();

    public:
        Mesh(std::string filename);
        virtual void Draw(const Camera &camera) = 0;
        virtual void Create() = 0;
        virtual ~Mesh();
    };
}

#endif //TASK_03_MESH_H_H
