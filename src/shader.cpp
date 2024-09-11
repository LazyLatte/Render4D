#include "shader.h"
Shader3D::Shader3D(){}
Shader3D::Shader3D(const char *vs_filename, const char *fs_filename){
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
    glAttachShader(this->program, vertexShader);
    glAttachShader(this->program, fragmentShader);
    glLinkProgram(this->program);

    glUseProgram(this->program);
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    this->proj_matrix_location = glGetUniformLocation(this->program, "P");
    this->view_matrix_location = glGetUniformLocation(this->program, "V");
    this->model_matrix_location = glGetUniformLocation(this->program, "M");
}

void Shader3D::setM(const GLfloat *model_matrix){
    glUseProgram(this->program);
    glUniformMatrix4fv(this->model_matrix_location, 1, GL_FALSE, model_matrix);
}

void Shader3D::setVP(const GLfloat *view_matrix, const GLfloat *proj_matrix){
    glUseProgram(this->program);
    glUniformMatrix4fv(this->view_matrix_location, 1, GL_FALSE, view_matrix);
    glUniformMatrix4fv(this->proj_matrix_location, 1, GL_FALSE, proj_matrix);
}

void Shader3D::setMVP(const GLfloat *model_matrix, const GLfloat *view_matrix, const GLfloat *proj_matrix){
    this->setM(model_matrix);
    this->setVP(view_matrix, proj_matrix);
}

Shader4D::Shader4D(const char *vs_filename, const char *fs_filename, const char *gs_filename){
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
    std::string shaderSource = readShaderCode(vs_filename);
    const GLchar* s[1];
    s[0] = shaderSource.c_str();
    glShaderSource(vertexShader, 1, s, NULL);

    shaderSource = readShaderCode(fs_filename);
    s[0] = shaderSource.c_str();
    glShaderSource(fragmentShader, 1, s, NULL);
    
    shaderSource = readShaderCode(gs_filename);
    s[0] = shaderSource.c_str();
    glShaderSource(geometryShader, 1, s, NULL);

    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);
    glCompileShader(geometryShader);

    this->program = glCreateProgram();
    glAttachShader(this->program, vertexShader);
    glAttachShader(this->program, fragmentShader);
    glAttachShader(this->program, geometryShader);
    glLinkProgram(this->program);

    glUseProgram(this->program);
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(geometryShader);

    this->proj_matrix_location = glGetUniformLocation(this->program, "P");
    this->view_matrix_location = glGetUniformLocation(this->program, "V");
    this->model_matrix_location = glGetUniformLocation(this->program, "M");

    this->hyperplane_normal_location = glGetUniformLocation(this->program, "hyperPlaneNormal");
    this->hyperplane_offset_location = glGetUniformLocation(this->program, "hyperPlaneOffset");
    this->left_isoclinic_location = glGetUniformLocation(this->program, "left_isoclinic");
    this->right_isoclinic_location = glGetUniformLocation(this->program, "right_isoclinic");
    this->translation_location = glGetUniformLocation(this->program, "translation");
}

void Shader4D::setHyperplane(const GLfloat *hyperplane_normal, const GLfloat hyperplane_offset){
    glUseProgram(this->program);
    glUniform4fv(this->hyperplane_normal_location, 1, hyperplane_normal);
    glUniform1f(this->hyperplane_offset_location, hyperplane_offset);
}

void Shader4D::setTransform4D(const GLfloat *translation, const GLfloat *left_isoclinic_matrix, const GLfloat *right_isoclinic_matrix){
    glUseProgram(this->program);
    glUniform4fv(this->translation_location, 1, translation);
    glUniformMatrix4fv(this->left_isoclinic_location, 1, GL_FALSE, left_isoclinic_matrix);
    glUniformMatrix4fv(this->right_isoclinic_location, 1, GL_FALSE, right_isoclinic_matrix);
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