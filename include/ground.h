#ifndef TASK_03_GROUND_H
#define TASK_03_GROUND_H

#include "Utility.h"

class Ground
{
private:
    GL::Camera &camera;
    GLuint groundShader; // Шейдер для земли
    GLuint groundVAO; // VAO для земли
    GLuint texture;
public:
    Ground(GL::Camera &camera_ref);
    void Draw();
    void Create();
};

#endif //TASK_03_GROUND_H
