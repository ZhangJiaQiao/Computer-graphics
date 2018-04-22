#include<glad\glad.h>
#include<GLFW\glfw3.h>
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<iostream>
#include<math.h>
#include<algorithm>
#include"Camera.h"
#include"Shader.h"
#include"imGui\imgui.h"
#include"imGui\imgui_impl_glfw_gl3.h"

using namespace std;
//设置回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow *window, double xPos, double yPos);
void scroll_callback(GLFWwindow *window, double xOffset, double yOffset);
//窗口初始化函数
GLFWwindow* initialGLFW_GLAD();
void drawCube(GLFWwindow* window);

// homework 6
void drawCubeAndLight(GLFWwindow* window);
Shader setLightMenu(GLFWwindow *window);
//设置imGui以及生成窗口
void setGui(GLFWwindow* window);
void setMenu(GLFWwindow* window, Shader shader);

// homework 5
void projection(GLFWwindow *window, Shader shader);
void viewChange(GLFWwindow *window, Shader shader);
void useCamera(GLFWwindow *window, Shader shader);

// homework 4
void translate(glm::mat4 &transform);
void rotate(glm::mat4 &transform);
void scale(glm::mat4 &transform);
void revolute(glm::mat4 &transform);

void lightMove();
//设置窗口大小
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

const float length = 0.5f;

static float degreeRotate = 0.1f;
static float lenTranslate = 0.01f;
static float ratioScale = 1.0f;
static float degreeRevolute = 0.1f;
static float revoluteRadius = 2.0f;

static float translateStep = 0.05f;
static float scaleStep = 0.01f;

float eyePos[3] = {0.0f, 0.0f, 8.0f};
float cubePos[3] = { -1.5f, 0.5f, -1.5f };
float lightPos[3] = { 1.2f, 1.0f, 2.0f };

float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool firstMouse = true;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
float fov = 45.0;

float k_a = 1;
float k_d = 1;
float k_s = 1;
float specularStrength = 0.5;

Camera camera(glm::vec3(eyePos[0], eyePos[1], eyePos[2]));

int main() {
	GLFWwindow *window = initialGLFW_GLAD();
	if (window == NULL) {
		return -1;
	}
	//drawCube(window);
	drawCubeAndLight(window);
	return 0;
}

GLFWwindow* initialGLFW_GLAD() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Bresenham", NULL, NULL);
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
	setGui(window);
	return window;
}

//初始化imGui
void setGui(GLFWwindow* window) {
	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui::StyleColorsClassic();
}
//设置Gui窗口
void setMenu(GLFWwindow* window, Shader shader) {
	//shader.use();
	//// 投影矩阵
	//glm::mat4 projection = glm::perspective(glm::radians(45.0f),
	//	(GLfloat)(SCR_WIDTH) / SCR_HEIGHT, 0.1f, 100.0f);
	//// 视变换矩阵
	//glm::mat4 view;
	//glm::vec3 tempPos = glm::vec3(eyePos[0], eyePos[1], eyePos[2]);
	////view = glm::translate(view, tempPos);
	//view = glm::lookAt(tempPos,
	//	glm::vec3(cubePos[0], cubePos[1], cubePos[2]), glm::vec3(0.0f, 1.0f, 0.0f));
	//// 模型变换矩阵
	////glm::mat4 projection = glm::mat4();
	////glm::mat4 view = glm::mat4();
	//glm::mat4 transform = glm::mat4();
	//transform = glm::translate(transform, glm::vec3(cubePos[0], cubePos[1], cubePos[2]));
	//unsigned int transformLoc = glGetUniformLocation(shader.ID, "transform");
	//unsigned int viewLoc = glGetUniformLocation(shader.ID, "view");
	//unsigned int projectionLoc = glGetUniformLocation(shader.ID, "projection");

	//static bool ifTranslate = false;
	//static bool ifScale = false;
	//static bool ifRotate = false;
	//static bool ifRovolute = false;
	//ImGui_ImplGlfwGL3_NewFrame();
	//{
	//	ImGui::Checkbox("Translate left and right", &ifTranslate);
	//	ImGui::Checkbox("Roate by y=z", &ifRotate);
	//	ImGui::Checkbox("Scale and shrink", &ifScale);
	//	ImGui::Checkbox("Revolute", &ifRovolute);
	//	ImGui::DragFloat3("LookAt pos", eyePos, 0.005f, -20.0f, 20.0f);
	//	ImGui::DragFloat3("Cube Pos", cubePos, 0.005f, -3.0f, 3.0f);
	//}
	//if (ifTranslate) {
	//	translate(transform);
	//}
	//if (ifScale) {
	//	scale(transform);
	//}
	//if (ifRovolute) {
	//	revolute(transform);
	//}
	//if (ifRotate) {
	//	rotate(transform);
	//}
	//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
	//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	//glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	static int choice = 0;
	ImGui_ImplGlfwGL3_NewFrame();
	{
		ImGui::Combo("Action", &choice, "projection\0view change\0camera\0");
	}
	if (choice == 0) {
		projection(window, shader);
	}
	else if (choice == 1) {
		viewChange(window, shader);
	}
	else {
		useCamera(window, shader);
	}
	int display_w, display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	ImGui::Render();
}

Shader setLightMenu(GLFWwindow *window)
{
	char *gouraudFragmentShaderPath = "gouraudLightObject.fs";
	char *gouraudVertexShaderPath   = "gouraudLightObject.vs";
	char *objectVertexShaderPath = "object.vs";
	char *objectFragmentShaderPath = "object.fs";

	static bool ifFPS = false;
	static int lightChoice = 0;

	ImGui_ImplGlfwGL3_NewFrame();
	{
		ImGui::Combo("Action", &lightChoice, "Phong Shading\0Gouraud Shading\0");
		ImGui::Checkbox("FPS", &ifFPS);
		ImGui::DragFloat("ambient", &k_a, 0.01f, 0.0f, 10.0f);
		ImGui::DragFloat("diffuse", &k_d, 0.01f, 0.0f, 10.0f);
		ImGui::DragFloat("specular", &k_s, 0.01f, 0.0f, 10.0f);
		ImGui::DragFloat("Specular Strength", &specularStrength, 0.01f, 0.0f, 10.0f);
	}
	int display_w, display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	ImGui::Render();
	if (ifFPS) {
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else {
		glfwSetCursorPosCallback(window, NULL);
		glfwSetScrollCallback(window, NULL);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	if (lightChoice == 1) {
		Shader objectShader = Shader(gouraudVertexShaderPath, gouraudFragmentShaderPath);
		return objectShader;
	}
	else {
		Shader objectShader = Shader(objectVertexShaderPath, objectFragmentShaderPath);
		return objectShader;
	}

}

void drawCube(GLFWwindow* window) {
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	//设置着色器路径
	char *vertexShaderPath = "object.vs";
	char *fragmentShaderPath = "object.fs";
	float vertices[] = {
		// positions			    // color	      //texture coords		
		 length,  length,  length,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		 length, -length,  length,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-length, -length,  length,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		-length,  length,  length,  0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		 length,  length, -length,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		 length, -length, -length,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-length, -length, -length,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-length,  length, -length,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3,   //前面 
		4, 5, 6,
		4, 6, 7,
		0, 1, 5,
		0, 4, 5,
		2, 3, 6,
		3, 6, 7,
		1, 2, 5,
		1, 5, 6,
		0, 3, 4,
		3, 4, 7,
	};
	Shader shader(vertexShaderPath, fragmentShaderPath);
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	shader.use();
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		//glDisable(GL_DEPTH_TEST);
		//projection(window, shader);
		//viewChange(window, shader);
		setMenu(window, shader);
		//useCamera(window, shader);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6 * 6, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
}

void drawCubeAndLight(GLFWwindow* window) {
	char *lightVertexShaderPath     = "light.vs";
	char *lightFragmentShaderPath   = "light.fs";

	//Shader objectShader = Shader(objectVertexShaderPath, objectFragmentShaderPath);
	Shader lightShader  = Shader(lightVertexShaderPath, lightFragmentShaderPath);
	//Shader gouraudLightShader = Shader(gouraudVertexShaderPath, lightFragmentShaderPath);

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindVertexArray(cubeVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// 光源位置
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//渲染循环
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		Shader objectShader = setLightMenu(window);
		//Shader objectShader = Shader("gouraudLightObject.vs", "gouraudLightObject.fs");

		objectShader.use();
		objectShader.setFloat("k_a", k_a);
		objectShader.setFloat("k_d", k_d);
		objectShader.setFloat("k_s", k_s);
		objectShader.setFloat("specularStrength", specularStrength);

		objectShader.setVec3("objectColor", 1.0, 0.5f, 0.31f);
		objectShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		objectShader.setVec3("lightPos", lightPos[0], lightPos[1], lightPos[2]);
		objectShader.setVec3("viewPos", camera.Position);

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model;
		objectShader.setMat4("projection", projection);
		objectShader.setMat4("view", view);
		objectShader.setMat4("model", model);

		// 渲染物体
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		lightShader.use();
		lightShader.setMat4("projection", projection);
		lightShader.setMat4("view", view);
		model = glm::mat4();
		lightMove();
		model = glm::translate(model, glm::vec3(lightPos[0], lightPos[1], lightPos[2]));
		model = glm::scale(model, glm::vec3(0.2f));
		lightShader.setMat4("model", model);

		// 渲染光源
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
}

void lightMove()
{
	if (lenTranslate > 3.0f || lenTranslate <= -3.0f) {
		translateStep = -translateStep;
	}
	lenTranslate += translateStep;
	lightPos[0] = lenTranslate;
}

void projection(GLFWwindow *window, Shader shader) {
	shader.use();

	glm::mat4 transform = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 project = glm::mat4(1.0f);

	unsigned int transformLoc = glGetUniformLocation(shader.ID, "transform");
	unsigned int viewLoc = glGetUniformLocation(shader.ID, "view");
	unsigned int projectionLoc = glGetUniformLocation(shader.ID, "projection");

	transform = glm::translate(transform, glm::vec3(cubePos[0], cubePos[1], cubePos[2]));
	glm::vec3 tempPos = glm::vec3(eyePos[0], eyePos[1], eyePos[2]);
	view = glm::lookAt(tempPos,
		glm::vec3(cubePos[0], cubePos[1], cubePos[2]), glm::vec3(0.0f, 1.0f, 0.0f));
	static int projectionMode = 1;
	bool showPerspertive = false;
	bool showOrthographic = false;

	//ImGui_ImplGlfwGL3_NewFrame();
	{
		ImGui::Combo("Projection Mode", &projectionMode, "orthographic projection\0perspective projection\0");
		ImGui::DragFloat3("LookAt pos", eyePos, 0.005f, -20.0f, 20.0f);
		ImGui::DragFloat3("Cube Pos", cubePos, 0.005f, -3.0f, 3.0f);
	}

	if (projectionMode == 0) showOrthographic = true;
	if (projectionMode == 1) showPerspertive = true;

	if (showOrthographic) {
		ImGui::Text("Orthographic projection paras:");
		static float viewLeft = -3.0f, 
					 viewRight = 3.0f, 
					 viewTop = -3.0f, 
					 viewBottom = 3.0f, 
					 viewNear = 0.1f, 
					 viewFar = 10.0f;
		ImGui::DragFloat("Left", &viewLeft, 0.01f, -50.0f, 50.0f);
		ImGui::DragFloat("Right", &viewRight, 0.01f, -50.0f, 50.0f);
		ImGui::DragFloat("Top", &viewTop, 0.01f, -50.0f, 50.0f);
		ImGui::DragFloat("Bottom", &viewBottom, 0.01f, -50.0f, 50.0f);
		ImGui::DragFloat("Near", &viewNear, 0.01f, 0.0f, 50.0f);
		ImGui::DragFloat("Far", &viewFar, 0.01f, 0.0f, 50.0f);
		project = glm::ortho(viewLeft, viewRight, viewTop, viewBottom, viewNear, viewFar);
	}
	else {
		static float viewFov   = 45.0f,
					 viewRatio = (SCR_WIDTH) / SCR_HEIGHT,
					 viewNear  = 0.1f,
					 viewFar   = 50.0f;
		ImGui::Text("Perspertive projection paras:");
		ImGui::DragFloat("Fov", &viewFov, 0.1f, 0.0f, 180.0f);
		ImGui::DragFloat("Ratio", &viewRatio, 0.1f, 0.0f, 10.0f);
		ImGui::DragFloat("Near", &viewNear, 0.1f, -50.0f, 50.0f);
		ImGui::DragFloat("Far", &viewFar, 0.1f, -50.0f, 50.0f);
		project = glm::perspective(glm::radians(viewFov),
					(GLfloat)viewRatio, viewNear, viewFar);
	}
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(project));
	//int display_w, display_h;
	//glfwGetFramebufferSize(window, &display_w, &display_h);
	//glViewport(0, 0, display_w, display_h);
	//ImGui::Render();

}

void viewChange(GLFWwindow *window, Shader shader) {
	shader.use();
	static float degree = 0.0f;
	degree += 1.0f;
	if (degree >= 360.0f) {
		degree = 0.0f;
	}
	float x = revoluteRadius * cos(glm::radians(degree));
	float z = revoluteRadius * sin(glm::radians(degree));
	glm::mat4 transform = glm::mat4(1.0f);
	glm::mat4 view      = glm::mat4(1.0f);
	glm::mat4 project   = glm::mat4(1.0f);

	project = glm::perspective(glm::radians(45.0f),
			(GLfloat)(SCR_WIDTH) / SCR_HEIGHT, 0.1f, 100.0f);
	unsigned int transformLoc = glGetUniformLocation(shader.ID, "transform");
	unsigned int viewLoc = glGetUniformLocation(shader.ID, "view");
	unsigned int projectionLoc = glGetUniformLocation(shader.ID, "projection");

	view = glm::lookAt(glm::vec3(x, 0.0f, z),
		glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(project));
}

void useCamera(GLFWwindow *window, Shader shader) {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	glm::mat4 transform = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 project = glm::mat4(1.0f);

	unsigned int transformLoc = glGetUniformLocation(shader.ID, "transform");
	unsigned int viewLoc = glGetUniformLocation(shader.ID, "view");
	unsigned int projectionLoc = glGetUniformLocation(shader.ID, "projection");

	project = glm::perspective(glm::radians(fov),
		(GLfloat)(SCR_WIDTH) / SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetViewMatrix();
	transform = glm::translate(transform, glm::vec3(cubePos[0], cubePos[1], cubePos[2]));

	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(project));

}

void rotate(glm::mat4 &transform) {
	degreeRotate += 0.1f;
	if (degreeRotate > 360.0f) {
		degreeRotate = 0.0f;
	}
	transform = glm::rotate(transform, degreeRotate, glm::vec3(0.0f, 1.0f, 1.0f));
}

void translate(glm::mat4 &transform) {
	if (lenTranslate > 10.0f || lenTranslate <= -10.0f) {
		translateStep = -translateStep;
	}
	lenTranslate += translateStep;
	transform =  glm::translate(transform, glm::vec3(lenTranslate, 0.0f, 0.0f));
}

void scale(glm::mat4 &transform) {
	if (ratioScale > 2.0f || ratioScale <= 0.1f) {
		scaleStep = -scaleStep;
	}
	ratioScale += scaleStep;
	transform = glm::scale(transform, glm::vec3(ratioScale, ratioScale, ratioScale));
}

void revolute(glm::mat4 &transform) {
	degreeRevolute += 1.0f;
	if (degreeRevolute > 360.0f) {
		degreeRevolute = 0.0f;
	}
	float x = revoluteRadius * cos(glm::radians(degreeRevolute));
	float z = revoluteRadius * sin(glm::radians(degreeRevolute));
	transform = glm::translate(transform, glm::vec3(x, 0.0f, z));
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xOffset, double yOffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yOffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}