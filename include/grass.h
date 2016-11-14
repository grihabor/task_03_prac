#ifndef TASK_03_GRASS_H
#define TASK_03_GRASS_H

#include "Utility.h"
#include <vector>
#include "Texture.h"

using std::vector;


class Grass
{
private:
    const uint GRASS_INSTANCES = 8*8; // Количество травинок
    const char *FILENAME_TEXTURE_GRASS = "Texture/grass_1.png";

    GL::Camera& camera;
    GLuint indexBuffer;
    int nIndices;
    GL::Texture texture;

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
