#include "shader.h"

Shader::Shader(){}

Shader::Shader(const char *vs_filename, const char *fs_filename){
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    std::string shaderSource = readShaderCode(vs_filename);
    const GLchar* s[1];
    s[0] = shaderSource.c_str();
    glShaderSource(vertexShader, 1, s, NULL);
    shaderSource = readShaderCode(fs_filename);
    s[0] = shaderSource.c_str();
    glShaderSource(fragmentShader, 1, s, NULL);
    
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

    this->program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glUseProgram(program);
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    this->proj_matrix = glGetUniformLocation(this->program, "P");
    this->view_matrix = glGetUniformLocation(this->program, "V");
    this->model_matrix = glGetUniformLocation(this->program, "M");
}

void Shader::useShader(const GLfloat *model_matrix, const GLfloat *view_matrix, const GLfloat *proj_matrix){
    glUseProgram(this->program);
    glUniformMatrix4fv(this->model_matrix, 1, GL_FALSE, model_matrix);
    glUniformMatrix4fv(this->view_matrix, 1, GL_FALSE, view_matrix);
    glUniformMatrix4fv(this->proj_matrix, 1, GL_FALSE, proj_matrix);
}
std::string readShaderCode(const char *filename){
    std::ifstream in;
    in.open(filename);
    if(!in.is_open()){
        std::cout << "Failed to open file.\n";
        exit(1);
    }
    return std::string(
        std::istreambuf_iterator<char>(in),
        std::istreambuf_iterator<char>()
    );
}