#ifndef RENDERABLE_H
#define RENDERABLE_H
#include <iostream>
#include <GL/glew.h>
class Renderable {
    private:
        GLuint vao;
        GLuint texture;
        GLuint position_buffer;
        GLuint normal_buffer;
        GLuint tangent_buffer;
        GLuint texcoord_buffer;
        GLuint index_buffer;
        unsigned index_count;
    public:
        Renderable();
        Renderable(std::string filename);
        void draw();
};

#endif