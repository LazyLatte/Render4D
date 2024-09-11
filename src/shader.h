#ifndef SHADER_H
#define SHADER_H
#include <fstream>
#include <iostream>
#include <GL/glew.h>

std::string readShaderCode(const char *filename);

class Shader3D {
    protected:
        GLint model_matrix_location;
        GLint view_matrix_location;
        GLint proj_matrix_location;
        GLuint program;
    public:
        Shader3D();
        Shader3D(const char *vs_filename, const char *fs_filename);
        void setM(const GLfloat *model_matrix);
        void setVP(const GLfloat *view_matrix, const GLfloat *proj_matrix);
        void setMVP(const GLfloat *model_matrix, const GLfloat *view_matrix, const GLfloat *proj_matrix);
};

class Shader4D : public Shader3D {
    protected:
        GLint hyperplane_normal_location;
        GLint hyperplane_offset_location;
        GLint translation_location;
        GLint left_isoclinic_location;
        GLint right_isoclinic_location;
    public:
        Shader4D(const char *vs_filename, const char *fs_filename, const char *gs_filename);
        void setHyperplane(const GLfloat *hyperplane_normal, const GLfloat hyperplane_offset);
        void setTransform4D(const GLfloat *translation, const GLfloat *left_isoclinic_matrix, const GLfloat *right_isoclinic_matrix);
};

#endif