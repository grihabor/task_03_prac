#ifndef TASK_03_GRASS_H
#define TASK_03_GRASS_H

#include "Utility.h"
#include <vector>
#include "Texture.h"

using std::vector;


class Grass
{
private:
    const uint GRASS_INSTANCES = 100 * 100; // Количество травинок
    const char *FILENAME_TEXTURE_GRASS = "Texture/grass_1.png";

    GL::Camera& camera;
    GLuint indexBuffer;
    int nTriangles;
    GL::Texture texture;

    bool windFlag = true;
    float windPhase;
    float windDirection;
    int prevTimestamp;

    vector<VM::vec4> grassPoints;
    vector<VM::vec2> uvPoints;
    vector<unsigned char> faces;
    vector<unsigned char> indices;
    vector<VM::vec2> grassPositions;

    GLuint grassShader;      // Шейдер, рисующий траву
    GLuint grassVAO;         // VAO для травы (что такое VAO почитайте в доках)
    GLuint grassVariance;    // Буфер для смещения координат травинок
    vector<VM::vec2> grassVarianceData; // Вектор со смещениями для координат травинок
    vector<VM::vec2> grassVelocity; // Вектор со смещениями для координат травинок

private:
    void CreateVAO();
    void InitMeshAndUV();

public:
    Grass(GL::Camera& camera_ref);
    void UpdateGrassVariance();
    void Draw();
    vector<VM::vec2> GenerateGrassPositions();
    void Create();
    void WindSwitch(){windFlag = !windFlag;}
};

#endif //TASK_03_GRASS_H
