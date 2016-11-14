#include "Texture.h"
#include <SOIL.h>
#include <VM/vec2.h>
#include <vector>

using namespace std;

namespace GL {

void bindTexture(GLuint program, const string& name, GLuint texture, GLint unit) {
    glActiveTexture(GL_TEXTURE0 + unit);                                            CHECK_GL_ERRORS;
    glBindTexture(GL_TEXTURE_2D, texture);                                          CHECK_GL_ERRORS;

    GLint location = glGetUniformLocation(program, name.c_str());                   CHECK_GL_ERRORS;
    if(location >=0 ) {
        glUniform1i(location, unit);                                                CHECK_GL_ERRORS;
    }
}


    GLuint LoadTexture(const char *filename, GLuint mode)
    {
        GLuint texture;
        glGenTextures(1, &texture);                                                 CHECK_GL_ERRORS

        int width, height, n;

        // load texture
        unsigned char *textureData = SOIL_load_image(filename, &width,
                                                    &height, &n, 4);
        if(!textureData){
            cout << "stbi_load failed: " << filename << endl;
            return -1;
        } else {
            cout << "stbi_load success! " << filename << endl;
        }


        // pass texture to OpenGL
        glBindTexture(GL_TEXTURE_2D, texture);                                      CHECK_GL_ERRORS
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, textureData);                                CHECK_GL_ERRORS
        glGenerateMipmap(GL_TEXTURE_2D);                                            CHECK_GL_ERRORS



        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);                    CHECK_GL_ERRORS
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);                    CHECK_GL_ERRORS

        /*
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);                                   CHECK_GL_ERRORS
        */
        // free image data
        SOIL_free_image_data(textureData);
        return texture;
    }

    void LoadCoords(const char *filename, vector<VM::vec2>& uv, vector<int>& indices)
    {

    }
}
