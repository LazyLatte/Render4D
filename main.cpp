#include <fstream>
#include <iostream>
#include <vector>

//#define GLM_FORCED_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "tiny_obj_loader.h"
#include "shader.h"
#include "scene.h"
#include "hyperplane.h"
#include "tesseract.h"
#include "player.h"
#include "camera.h"
#define deg2rad(x) ((x)*((3.1415926f)/(180.0f)))
#define rad2deg(x) ((x)*(180.0f) / (3.1415926f))
using namespace glm;

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
glm::mat4 proj_matrix;


Scene *scene;
Player *player;
Camera *camera;
Hyperplane *hyperplane;
double prev_xpos, prev_ypos;
float prevRad = 0.0;
float prevTime = 0.0;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
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
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    
    if (glewInit() != GLEW_OK){
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }    

    
    
    scene = new Scene();
    player = new Player("ladybug.obj", glm::vec3(0.0, 5.0, 0.0), glm::vec3(0.5), glm::vec3(0.0, 0.0, 1.0), 0.01);
    camera = new Camera();
    hyperplane = new Hyperplane;
    
    scene->addPlayer(player);
    scene->addCamera(camera);
    scene->addHyperplane(hyperplane);
    for(int i=-5; i<5; i++){
        for(int j=-5; j<5; j++){
            //scene->addTesseract(glm::vec4(i*2.0, 0.0, j*2.0, 0.0));
        }
    }
    for(int i=-5; i<5; i++){
        for(int j=-5; j<5; j++){
            scene->addTesseract(glm::vec4(i*2.0, -10.0, j*2.0, 0.0));
            // scene->addTesseract(glm::vec4(i*2.0, -10.0, j*2.0, 2.0));
            // scene->addTesseract(glm::vec4(i*2.0, -10.0, j*2.0, -2.0));
            // scene->addTesseract(glm::vec4(i*2.0, -10.0, j*2.0, 4.0));
            // scene->addTesseract(glm::vec4(i*2.0, -10.0, j*2.0, -4.0));
        }
    }
    scene->init();
    glEnable(GL_DEPTH_TEST);
    framebuffer_size_callback(window, WIDTH, HEIGHT);
    while (!glfwWindowShouldClose(window)){
        
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float timeValue = (float)glfwGetTime() * 30.0f;
        float rad = deg2rad(timeValue);

        //scene->rotateHyperplane(rad - prevRad, 0.0, 0.0);
        prevRad = rad;
        scene->step(1.0f/60.0f);
        prevTime = timeValue;
        scene->render4D();
        scene->renderPlayer();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    delete player;
    delete camera;
    delete hyperplane;
    delete scene;
    glfwTerminate();
    return 0;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
    if(action == GLFW_PRESS || action == GLFW_REPEAT){
        switch(key){
            case GLFW_KEY_W: {
                player->moving_forward = true;
                player->moving_backward = false;
                break;
            }
            case GLFW_KEY_S: {
                player->moving_backward = true;
                player->moving_forward = false;
                break;
            }
            case GLFW_KEY_A: {
                player->moving_left = true;
                player->moving_right = false;
                break;
            }
            case GLFW_KEY_D: {
                player->moving_right = true;
                player->moving_left = false;
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
        camera->rotate(float(dx), float(dy), r);
        prev_xpos = xpos;
        prev_ypos = ypos;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    hyperplane->rotate(deg2rad(yoffset), 0.0, 0.0);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
    float viewportAspect = (float)width / (float)height;
    scene->setProjectionMatrix(viewportAspect);
}