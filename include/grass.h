#ifndef TASK_03_GRASS_H
#define TASK_03_GRASS_H

#include "Utility.h"
#include <vector>

using std::vector;

const uint GRASS_INSTANCES = 64; // Количество травинок

class Grass
{
private:
    GL::Camera& camera;
    GLuint grassPointsCount; // Количество вершин у модели травинки
    GLuint grassShader;      // Шейдер, рисующий траву
    GLuint grassVAO;         // VAO для травы (что такое VAO почитайте в доках)
    GLuint grassVariance;    // Буфер для смещения координат травинок
    vector<VM::vec4> grassVarianceData; // Вектор со смещениями для координат травинок

public:
    Grass(GL::Camera& camera_ref);
    void UpdateGrassVariance();
    void Draw();
    vector<VM::vec2> GenerateGrassPositions();
    void Create();
};

#endif //TASK_03_GRASS_H
