#include<iostream>
#include"glad/glad.h"
#include <GLFW/glfw3.h>
// 加入glm库
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Shader.h"
#include "imGui/imgui.h"
#include "imGui/imgui_impl_glfw_gl3.h"

using namespace std;

GLFWwindow *initGLFW();
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void curse_poscallback(GLFWwindow *window, double x, double y);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void drawBezier();
void setGUI(GLFWwindow *window);
void setPointMenu(GLFWwindow *window);

const GLuint SCR_WIDTH = 800, SCR_HEIGHT = 600;

GLint now_point = 0;
GLfloat mouseX;
GLfloat mouseY;
GLint   mouseX_int;
GLint   mouseY_int;

float color[] = {1.0f, 0.5f, 0.2f};

GLfloat vertices[] = {
    -0.5f, -0.5f, 0.0f,  
     0.5f, -0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f,
     0.5f,  0.5f, 0.0f
};


int main()
{
    GLFWwindow *window = initGLFW();
    setGUI(window);
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
	char *pointVertexShaderPath     = "simpleShader.vs";
	char *pointFragmentShaderPath   = "simpleShader.fs";
    char *lineVertexShaderPath      = "lineShader.vs";
    char *lineFragmentShaderPath    = "lineShader.fs";

    Shader shader = Shader(pointVertexShaderPath, pointFragmentShaderPath);
    Shader lineShader = Shader(lineVertexShaderPath, lineFragmentShaderPath);
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, curse_poscallback);

    while (!glfwWindowShouldClose(window))
    {
        shader.use();
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindVertexArray(VAO);
        if (now_point == 1)
            glDrawArrays(GL_POINTS, 0, 1);
        else
            glDrawArrays(GL_LINE_STRIP, 0, now_point);

        if (now_point == 4)
        {
            lineShader.use();
            setPointMenu(window);
            lineShader.setVec3("inColor", color[0], color[1], color[2]);
            drawBezier();
        }

        glBindVertexArray(0);
        glfwSwapBuffers(window);
        
    }
    // printf("point = %d:X = %f, Y = %f\n", 0, vertices[0], vertices[1]);
    // printf("point = %d:X = %f, Y = %f\n", 1, vertices[3], vertices[4]);
    // printf("point = %d:X = %f, Y = %f\n", 2, vertices[6], vertices[7]);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}

GLFWwindow* initGLFW()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        // Mac下需要加下面语句
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Mac设置
#endif
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Bezier Curve", nullptr, nullptr);
    if (window == NULL) {
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		return NULL;
	}
    return window;
}

void drawBezier()
{
    GLfloat Q[2];
    GLfloat point[33] = {0.0f};
    int i = 0;
    for (double t = 0.0f; t <= 1.0f; t += 0.1)
    {
        double a1 = pow((1 - t), 3);
        double a2 = pow((1 - t), 2) * 3 * t;
        double a3 = 3 * t * t * (1 - t);
        double a4 = t * t * t;
        Q[0] = a1 * vertices[0] + a2 * vertices[3] + a3 * vertices[6] + a4 * vertices[9];
        Q[1] = a1 * vertices[1] + a2 * vertices[4] + a3 * vertices[7] + a4 * vertices[10];
        point[i * 3]     = Q[0];
        point[i * 3 + 1] = Q[1];
        i++;
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);
    glDrawArrays(GL_LINE_STRIP, 0, 11);
}

void setGUI(GLFWwindow *window)
{
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsClassic();
}

void setPointMenu(GLFWwindow *window)
{
    ImGui_ImplGlfwGL3_NewFrame();
    {
        ImGui::DragFloat2("point1", vertices,       0.01f, -1.0f, 1.0f);
        ImGui::DragFloat2("point2", (vertices + 3), 0.01f, -1.0f, 1.0f);
        ImGui::DragFloat2("point3", (vertices + 6), 0.01f, -1.0f, 1.0f);
        ImGui::DragFloat2("point4", (vertices + 9), 0.01f, -1.0f, 1.0f);
        ImGui::ColorEdit3("line color", color);
    }
    int display_w, display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
    ImGui::Render();
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS && now_point != 0)
    {
        printf("D pressed\n");
        now_point--;
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS and button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (now_point <= 3)
        {
            vertices[now_point * 3]     = mouseX;
            vertices[now_point * 3 + 1] = mouseY;
            vertices[now_point * 3 + 2] = 0.0f;
            // printf("point = %d:X = %f, Y = %f\n", now_point * 3, vertices[now_point * 3], vertices[now_point * 3 + 1]);
            now_point++;
        }
    }
}

void curse_poscallback(GLFWwindow *window, double x, double y)
{
    x =  (x - SCR_WIDTH  / 2) / (SCR_WIDTH  / 2);
    y = -(y - SCR_HEIGHT / 2) / (SCR_HEIGHT / 2);
    mouseX = x;
    mouseY = y;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}