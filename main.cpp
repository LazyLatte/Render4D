#include <fstream>
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "slice.h"

#define deg2rad(x) ((x)*((3.1415926f)/(180.0f)))
#define CUBE_TETRAHEDRON_INDICIES(v0, v1, v2, v3, v4, v5, v6, v7) \
    v0,v1,v3,v4,\
    v2,v1,v3,v6,\
    v7,v3,v4,v6,\
    v5,v1,v4,v6,\
    v6,v1,v3,v4
using namespace glm;

struct {
    GLint model_matrix;
    GLint view_matrix;
    GLint proj_matrix;
}uniforms;

vec4 tesseract[] = {
    vec4(1.0, 1.0, 1.0, 1.0),
    vec4(1.0, -1.0, 1.0, 1.0),
    vec4(-1.0, -1.0, 1.0, 1.0),
    vec4(-1.0, 1.0, 1.0, 1.0),

    vec4(1.0, 1.0, -1.0, 1.0),
    vec4(1.0, -1.0, -1.0, 1.0),
    vec4(-1.0, -1.0, -1.0, 1.0),
    vec4(-1.0, 1.0, -1.0, 1.0),

    vec4(1.0, 1.0, 1.0, -1.0),
    vec4(1.0, -1.0, 1.0, -1.0),
    vec4(-1.0, -1.0, 1.0, -1.0),
    vec4(-1.0, 1.0, 1.0, -1.0),

    vec4(1.0, 1.0, -1.0, -1.0),
    vec4(1.0, -1.0, -1.0, -1.0),
    vec4(-1.0, -1.0, -1.0, -1.0),
    vec4(-1.0, 1.0, -1.0, -1.0)
};

unsigned tesseract_indices[] = {  
    //cube order -> in out top down right front left back
    CUBE_TETRAHEDRON_INDICIES(0, 1, 2, 3, 4, 5, 6, 7),
    CUBE_TETRAHEDRON_INDICIES(8, 9, 10, 11, 12, 13, 14, 15),     
    CUBE_TETRAHEDRON_INDICIES(8, 9, 10, 11, 0, 1, 2, 3),    
    CUBE_TETRAHEDRON_INDICIES(4, 5, 6, 7, 12, 13, 14, 15), 
    CUBE_TETRAHEDRON_INDICIES(8, 0, 3, 11, 12, 4, 7 ,15),
    CUBE_TETRAHEDRON_INDICIES(8, 9, 1, 0, 12, 13, 5, 4),
    CUBE_TETRAHEDRON_INDICIES(1, 9, 10, 2, 5, 13, 14, 6),
    CUBE_TETRAHEDRON_INDICIES(3, 2, 10, 11, 7, 6, 14, 15) 
};

vec4 offsets[] = {
    vec4(0.0, 0.0, 0.0, 0.0),
    vec4(4.0, 0.0, 0.0, 0.0),
    vec4(-4.0, 0.0, 0.0, 0.0),
    vec4(0.0, 4.0, 0.0, 0.0),
    vec4(0.0, -4.0, 0.0, 0.0),
    vec4(0.0, 0.0, 4.0, 0.0),
    vec4(0.0, 0.0, -4.0, 0.0)
};

const GLuint WIDTH = 1600, HEIGHT = 1400;
mat4 model_matrix;
mat4 view_matrix;
mat4 proj_matrix;
vec3 camera = vec3(10.0, 5.0, 8.0);
vec3 front = -normalize(camera);
vec3 hyperPlaneRad = vec3(0.0, 0.0, 0.0);
const vec3 z_axis = vec3(0.0, 0.0, 1.0);
const float displacement = 0.5f;
double prev_xpos, prev_ypos;

void My_Init(void);
std::string readShaderCode(const char *filename);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow* window, int height, int width);

int main(int argc, char** argv){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Tesseract", NULL, NULL);    
    if (!window){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    framebuffer_size_callback(window, WIDTH, HEIGHT);
    if (glewInit() != GLEW_OK){
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }    
    
    My_Init();
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    std::vector<vec4> vertices(tesseract, tesseract + (sizeof(tesseract)/sizeof(tesseract[0])));
    std::vector<unsigned> indices(tesseract_indices, tesseract_indices + (sizeof(tesseract_indices)/sizeof(tesseract_indices[0])));

    while (!glfwWindowShouldClose(window)){
        
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //glUseProgram(shaderProgram);

        float timeValue = (float)glfwGetTime() * 30.0f;
        float rad = deg2rad(timeValue);
        // mat3x4 hyperPlane = mat3x4(
        //     cos(hyperPlaneRad.x), 0.0, 0.0, sin(hyperPlaneRad.x),
        //     0.0, cos(hyperPlaneRad.y), 0.0, sin(hyperPlaneRad.y),
        //     0.0, 0.0, cos(hyperPlaneRad.z), sin(hyperPlaneRad.z)
        // );
        mat3x4 hyperPlane = mat3x4(
            cos(rad), 0.0, 0.0, sin(rad),
            0.0, -sin(rad), 0.0, cos(rad),
            0.0, 0.0, cos(rad) + sin(rad), 1.0
        );
        vec4 hyperPlaneNormal = hyperPlane[0][0] != 0 && hyperPlane[1][1] !=0 && hyperPlane[2][2] != 0 ? vec4(-hyperPlane[0][3]/hyperPlane[0][0], -hyperPlane[1][3]/hyperPlane[1][1], -hyperPlane[2][3]/hyperPlane[2][2], 1.0) : vec4(hyperPlane[0][0] == 0, hyperPlane[1][1] == 0, hyperPlane[2][2] == 0, 0);
        float hyperPlaneOffset = 0.0;
        std::vector<float> triangle_vertices;
        //render 7 hypercube
        for(int i=0; i<sizeof(offsets)/sizeof(offsets[0]); i++){
            cpu_slice(triangle_vertices, hyperPlaneNormal, hyperPlaneOffset, vertices, indices, offsets[i]);
        }
        if(triangle_vertices.size() > 0){
            glBufferData(GL_ARRAY_BUFFER, triangle_vertices.size()*sizeof(float), &triangle_vertices[0], GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
            glEnableVertexAttribArray(0);
        }

        model_matrix = mat4(1.0f);
        view_matrix = lookAt(camera, camera+front, z_axis);

        glUniformMatrix4fv(uniforms.model_matrix, 1, GL_FALSE, &model_matrix[0][0]);
        glUniformMatrix4fv(uniforms.view_matrix, 1, GL_FALSE, &view_matrix[0][0]);
        glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &proj_matrix[0][0]);

        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)triangle_vertices.size());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
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

void My_Init(){

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    std::string shaderSource = readShaderCode("./shader/vs.glsl");
    const GLchar* s[1];
    s[0] = shaderSource.c_str();
    glShaderSource(vertexShader, 1, s, NULL);
    shaderSource = readShaderCode("./shader/fs.glsl");
    s[0] = shaderSource.c_str();
    glShaderSource(fragmentShader, 1, s, NULL);
    
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glUseProgram(program);
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    uniforms.proj_matrix = glGetUniformLocation(program, "P");
    uniforms.view_matrix = glGetUniformLocation(program, "V");
    uniforms.model_matrix = glGetUniformLocation(program, "M");

    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
    if(action == GLFW_PRESS){
        switch(key){
            case GLFW_KEY_W: {
                mat4 translate_matrix = translate(mat4(1.0), front*displacement);
                camera = translate_matrix * vec4(camera, 1.0);
                break;
            }
            case GLFW_KEY_S: {
                mat4 translate_matrix = translate(mat4(1.0), -front*displacement);
                camera = translate_matrix * vec4(camera, 1.0);
                break;
            }
            case GLFW_KEY_A: {
                vec3 right = normalize(cross(front, z_axis));
                camera = translate(mat4(1.0), -right*displacement) * vec4(camera, 1.0);
                break;
            }
            case GLFW_KEY_D: {
                vec3 right = normalize(cross(front, z_axis));
                camera = translate(mat4(1.0), right*displacement) * vec4(camera, 1.0);
                break;
            }
            case GLFW_KEY_E: {
                hyperPlaneRad.x += deg2rad(10);
                break;
            }
            case GLFW_KEY_Q: {
                hyperPlaneRad.x -= deg2rad(10);
                break;
            }
            case GLFW_KEY_1: {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                break;
            }
            case GLFW_KEY_2: {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                break;
            }
            case GLFW_KEY_ESCAPE: {
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            }
            default:
                break;
        }
    }

}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        int cursor_mode = glfwGetInputMode(window, GLFW_CURSOR);
        glfwSetInputMode(window, GLFW_CURSOR, cursor_mode == GLFW_CURSOR_NORMAL ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
        glfwGetCursorPos(window, &prev_xpos, &prev_ypos);
    }
        
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos){
    if(glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED){
        double dx = xpos - prev_xpos;
        double dy = ypos - prev_ypos;
        float r = (float)sqrt(dx*dx+dy*dy)*0.0008f;
        float max_theta = (float)acos(dot(front, z_axis) / length(front)) - 0.52359877559f;
        float theta = std::min(r, max_theta);
        vec3 right = normalize(cross(front, vec3(0.0, 0.0, 1.0)));
        vec3 up = normalize(cross(right, front));
        vec3 axis = float(-dy)*right + float(-dx)*up;
        mat4 rotate_matrix = rotate(mat4(1.0), theta, axis); 
        front = rotate_matrix * vec4(front, 1.0);
        prev_xpos = xpos;
        prev_ypos = ypos;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
    float viewportAspect = (float)width / (float)height;
    proj_matrix = perspective(deg2rad(60.0f), viewportAspect, 0.1f, 1000.0f);
}