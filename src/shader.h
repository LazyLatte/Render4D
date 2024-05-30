#ifndef SHADER_H
#define SHADER_H
#include <fstream>
#include <iostream>
#include <GL/glew.h>

std::string readShaderCode(const char *filename);

class Shader {
    private:
        GLint model_matrix;
        GLint view_matrix;
        GLint proj_matrix;
        GLuint program;
    public:
        Shader();
        Shader(const char *vs_filename, const char *fs_filename);
        void useShader(const GLfloat *model_matrix, const GLfloat *view_matrix, const GLfloat *proj_matrix);
};

#endif