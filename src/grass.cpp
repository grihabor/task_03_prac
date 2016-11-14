#include <GL/Texture.h>
#include "grass.h"



Grass::Grass(GL::Camera& camera_ref)
        : grassVarianceData(GRASS_INSTANCES),
          grassVelocity(GRASS_INSTANCES),
          camera(camera_ref)
{
    prevTimestamp = 0;
}

// Обновление смещения травинок
void Grass::UpdateGrassVariance() {

    int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
    int deltaTime;
    if(prevTimestamp)
        deltaTime = timeSinceStart - prevTimestamp;
    else
        deltaTime = 0;
    prevTimestamp = timeSinceStart;

    //deltaTime = 1;

    std::cout << deltaTime << std::endl;

    //dp = m*(v1-v2) = dF / dt
    float balancePoint = 0.01;
    float k = .1;
    float speed = 1e-3;
    //TODO: добавить силу трения!

    // Генерация случайных смещений
    for (uint i = 0; i < GRASS_INSTANCES; ++i) {
        float newx = grassVarianceData[i].x + deltaTime * grassVelocity[i].x;
        float newz = grassVarianceData[i].z + deltaTime * grassVelocity[i].z;
        //(float(rand()) / RAND_MAX - .5f) / 40000;
        grassVelocity[i].x += speed * ( - k * (grassVarianceData[i].x - balancePoint)) * cos(windDirection);
        grassVelocity[i].z += speed * ( - k * (grassVarianceData[i].z - balancePoint)) * sin(windDirection);

        grassVarianceData[i].x = newx;
        grassVarianceData[i].z = newz;
    }

    // Привязываем буфер, содержащий смещения
    glBindBuffer(GL_ARRAY_BUFFER, grassVariance);                                CHECK_GL_ERRORS
    // Загружаем данные в видеопамять
    glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec4) * GRASS_INSTANCES,
                 grassVarianceData.data(), GL_STATIC_DRAW);                      CHECK_GL_ERRORS
    // Отвязываем буфер
    glBindBuffer(GL_ARRAY_BUFFER, 0);                                            CHECK_GL_ERRORS
}

// Рисование травы
void Grass::Draw() {
    // Тут то же самое, что и в рисовании земли
    glUseProgram(grassShader);                                                  CHECK_GL_ERRORS
    GLint cameraLocation = glGetUniformLocation(grassShader, "camera");         CHECK_GL_ERRORS
    glUniformMatrix4fv(cameraLocation, 1, GL_TRUE,
                       camera.getMatrix().data().data());                       CHECK_GL_ERRORS

    glBindVertexArray(grassVAO);                                                CHECK_GL_ERRORS
    glBindTexture(GL_TEXTURE_2D, texture.id);                                   CHECK_GL_ERRORS
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);                         CHECK_GL_ERRORS

    // Обновляем смещения для травы
    UpdateGrassVariance();
    // Отрисовка травинок в количестве GRASS_INSTANCES
    //glDrawArraysInstanced(GL_TRIANGLES, 0, grassPointsCount, GRASS_INSTANCES);   CHECK_GL_ERRORS

    glDrawElementsInstanced(
        GL_TRIANGLES,
        nTriangles*3,
        GL_UNSIGNED_BYTE,
        (void*)0,
        GRASS_INSTANCES
    );                                                                          CHECK_GL_ERRORS

    glBindVertexArray(0);                                                       CHECK_GL_ERRORS
    glUseProgram(0);                                                            CHECK_GL_ERRORS
}

double Crop(double a)
{
    if(a < 0.)
        return 0.;
    if(a > 1.)
        return 1.;
    return a;
}

#include <algorithm>

vector<VM::vec2> Grass::GenerateGrassPositions() {
    vector<VM::vec2> grassPositions(GRASS_INSTANCES);
    float width = sqrt(GRASS_INSTANCES) + .5;
    float k = 1;

    for (uint i = 0; i < GRASS_INSTANCES; ++i) {
        float x = i % int(width);
        float y = i / int(width);
        x += k * (float(rand()) / RAND_MAX - .5f);
        y += k * (float(rand()) / RAND_MAX);
        x = x/width;
        y = y/width;

        grassPositions[i] = VM::vec2(x, y);
    }

    std::sort(grassPositions.begin(), grassPositions.end(),
              [](const VM::vec2 &v1, const VM::vec2 &v2){return v1.y > v2.y;});

    return grassPositions;
}

void Grass::CreateVAO()
{
    glGenVertexArrays(1, &grassVAO);                                             CHECK_GL_ERRORS
    glBindVertexArray(grassVAO);                                                 CHECK_GL_ERRORS
}

void Grass::InitMeshAndUV()
{
    grassPositions = GenerateGrassPositions();
    for (uint i = 0; i < GRASS_INSTANCES; ++i) {
        grassVarianceData[i] = VM::vec4(0, 0, 0, 0);
    }

    texture = GL::LoadTexture(FILENAME_TEXTURE_GRASS, GL_CLAMP_TO_BORDER, GL_LINEAR);
    nTriangles = GL::LoadCoords("Texture/grass.uv", texture, grassPoints, uvPoints, faces);
    std::cout << "grass: nTriangles = " << nTriangles << std::endl;
    for(int i = 1; i <= nTriangles; ++i){
        indices.push_back(i);
    }
}


template<typename T>
void BindDataAndAttribute(GLuint buffer, vector<T> &data, GLuint location, int nValues, bool eachInstance)
{
    glBindBuffer(GL_ARRAY_BUFFER, buffer);                                 CHECK_GL_ERRORS
    glBufferData(GL_ARRAY_BUFFER, sizeof(data[0]) * data.size(),
                 data.data(), GL_STATIC_DRAW);                            CHECK_GL_ERRORS


    glEnableVertexAttribArray(location);                                   CHECK_GL_ERRORS
    glVertexAttribPointer(location, nValues, GL_FLOAT, GL_FALSE, 0, 0);          CHECK_GL_ERRORS
    if(eachInstance){
        // Здесь мы указываем, что нужно брать новое значение из этого буфера для каждого инстанса (для каждой травинки)
        glVertexAttribDivisor(location, 1);                                       CHECK_GL_ERRORS
    }
}


// Создание травы
void Grass::Create() {
    grassShader = GL::CompileShaderProgram("grass");

    InitMeshAndUV();
    CreateVAO();

    // VBO
    GLuint buffers[5];
    int bufCount = sizeof(buffers)/sizeof(buffers[0]);
    glGenBuffers(bufCount, buffers);                                              CHECK_GL_ERRORS

    GLuint pointsBuffer = buffers[0];
    GLuint positionBuffer = buffers[1];
    grassVariance = buffers[2];
    GLuint uvBuffer = buffers[3];
    indexBuffer = buffers[4];


    GLuint pointsLocation = glGetAttribLocation(grassShader, "point");           CHECK_GL_ERRORS
    GLuint positionLocation = glGetAttribLocation(grassShader, "position");      CHECK_GL_ERRORS
    GLuint varianceLocation = glGetAttribLocation(grassShader, "variance");      CHECK_GL_ERRORS
    GLuint uvpointLocation = glGetAttribLocation(grassShader, "uvpoint");        CHECK_GL_ERRORS


    // indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);                        CHECK_GL_ERRORS
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(faces[0]),
                 faces.data(), GL_STATIC_DRAW);     CHECK_GL_ERRORS

    // mesh points
    BindDataAndAttribute(pointsBuffer, grassPoints, pointsLocation, 4, false);

    // grass position
    BindDataAndAttribute(positionBuffer, grassPositions, positionLocation, 2, true);

    // grass variance
    BindDataAndAttribute(grassVariance, grassVarianceData, varianceLocation, 4, true);

    // uv points
    BindDataAndAttribute(uvBuffer, uvPoints, uvpointLocation, 2, false);


    // Отвязываем VAO
    glBindVertexArray(0);                                                        CHECK_GL_ERRORS
    // Отвязываем буфер
    glBindBuffer(GL_ARRAY_BUFFER, 0);                                            CHECK_GL_ERRORS
}