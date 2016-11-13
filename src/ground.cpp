#include "ground.h"
#include "stb_image_aug.h"
#include <vector>
#include <SOIL.h>

using std::vector;
using std::cout;
using std::endl;

Ground::Ground(GL::Camera &camera_ref)
        : camera(camera_ref)
{}

// Функция, рисующая замлю
void Ground::Draw() {
    // Используем шейдер для земли
    glUseProgram(groundShader);                                                  CHECK_GL_ERRORS

    // Устанавливаем юниформ для шейдера. В данном случае передадим перспективную матрицу камеры
    // Находим локацию юниформа 'camera' в шейдере
    GLint cameraLocation = glGetUniformLocation(groundShader, "camera");         CHECK_GL_ERRORS
    // Устанавливаем юниформ (загружаем на GPU матрицу проекции?)                                                     // ###
    glUniformMatrix4fv(cameraLocation, 1, GL_TRUE,
                       camera.getMatrix().data().data());                        CHECK_GL_ERRORS

    // Подключаем VAO, который содержит буферы, необходимые для отрисовки земли
    glBindVertexArray(groundVAO);                                                CHECK_GL_ERRORS


    glBindTexture(GL_TEXTURE_2D, texture);



    // Рисуем землю: 2 треугольника (6 вершин)
    glDrawArrays(GL_TRIANGLES, 0, 6);                                            CHECK_GL_ERRORS

    // Отсоединяем VAO
    glBindVertexArray(0);                                                        CHECK_GL_ERRORS
    // Отключаем шейдер
    glUseProgram(0);                                                             CHECK_GL_ERRORS
}

#define U(x) (x)
#define V(x) (1.0f - (x))

GLuint LoadTexture(const char *filename)
{
    GLuint texture;
    glGenTextures(1, &texture);CHECK_GL_ERRORS

    int width, height, n;

    // load texture
    unsigned char *textureData = stbi_jpeg_load(filename, &width,
                                           &height, &n, 4);
    if(!textureData){
        cout << "stbi_load failed: " << filename << endl;
        return -1;
    } else {
        cout << "stbi_load success! " << filename << endl;
    }


    // pass texture to OpenGL
    glBindTexture(GL_TEXTURE_2D, texture);CHECK_GL_ERRORS
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, textureData);CHECK_GL_ERRORS
    glGenerateMipmap(GL_TEXTURE_2D);CHECK_GL_ERRORS



    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);CHECK_GL_ERRORS
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);CHECK_GL_ERRORS

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);CHECK_GL_ERRORS

    return texture;
    // free image data
    //stbi_image_free(textureData);
}

// Создаём замлю
void Ground::Create() {

    vector<VM::vec2> uvPoints = {
            VM::vec2(U(0.f), V(0.f)),
            VM::vec2(U(1.f), V(0.f)),
            VM::vec2(U(1.f), V(1.f)),

            VM::vec2(U(0.f), V(0.f)),
            VM::vec2(U(1.f), V(1.f)),
            VM::vec2(U(0.f), V(1.f)),
    };
    vector<VM::vec4> meshPoints = {
            VM::vec4(0, 0, 0, 1),
            VM::vec4(1, 0, 0, 1),
            VM::vec4(1, 0, 1, 1),

            VM::vec4(0, 0, 0, 1),
            VM::vec4(1, 0, 1, 1),
            VM::vec4(0, 0, 1, 1)
    };

    groundShader = GL::CompileShaderProgram("ground");

    GLuint vboArray[2];
    int vbosNum = sizeof(vboArray)/sizeof(vboArray[0]);
    glGenBuffers(vbosNum, vboArray);CHECK_GL_ERRORS


    GLuint pointsVBO = vboArray[0];
    GLuint uvVBO = vboArray[1];


    texture = LoadTexture("Texture/ground.jpg");


    // Создание VAO
    // Генерация VAO
    glGenVertexArrays(1, &groundVAO);                                             CHECK_GL_ERRORS
    // Привязка VAO
    glBindVertexArray(groundVAO);                                                 CHECK_GL_ERRORS



    glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);                                 CHECK_GL_ERRORS
    glBufferData(GL_ARRAY_BUFFER, sizeof(meshPoints[0]) * meshPoints.size(),
                 meshPoints.data(), GL_STATIC_DRAW);                             CHECK_GL_ERRORS

    GLuint pointLocation = glGetAttribLocation(groundShader, "point");                   CHECK_GL_ERRORS
    glEnableVertexAttribArray(pointLocation);                                            CHECK_GL_ERRORS
    glVertexAttribPointer(pointLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);                   CHECK_GL_ERRORS



    glBindBuffer(GL_ARRAY_BUFFER, uvVBO);                                 CHECK_GL_ERRORS
    glBufferData(GL_ARRAY_BUFFER, sizeof(uvPoints[0]) * uvPoints.size(),
                 uvPoints.data(), GL_STATIC_DRAW);                             CHECK_GL_ERRORS

    GLuint uvpointLocation = glGetAttribLocation(groundShader, "uvpoint");                   CHECK_GL_ERRORS
    glEnableVertexAttribArray(uvpointLocation);                                            CHECK_GL_ERRORS
    glVertexAttribPointer(uvpointLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);                   CHECK_GL_ERRORS




    GLint samplerId = glGetUniformLocation(groundShader, "textureSampler");
    int boxTextureNum = 0;
    glActiveTexture(GL_TEXTURE0 + boxTextureNum);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(samplerId, boxTextureNum);



    // unbind vao and vbo
    //glBindVertexArray(0);                                                        CHECK_GL_ERRORS
    //glBindBuffer(GL_ARRAY_BUFFER, 0);                                            CHECK_GL_ERRORS

}