#ifndef TASK_03_GROUND_H
#define TASK_03_GROUND_H

#include "Utility.h"
#include "Texture.h"

class Ground
{
private:
    GLuint groundShader; // Шейдер для земли
    GLuint groundVAO; // VAO для земли
    GL::Texture texture;

public:
    Ground();
    void Draw(const GL::Camera &camera);
    void Create();
};

#endif //TASK_03_GROUND_H
