#include <iostream>
#include <cmath>

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other includes
#include "shader.h"
#include "camera.h"
#include "SimpleObjLoader.h"


// Function prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//���̻ص�����
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
//����ƶ��ص�����
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//�����ֻص�����
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//�����ƶ�����
void do_movement();

// Window dimensions
GLuint WIDTH = 800, HEIGHT = 600;

//camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

//���������¼
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

//��Դ��Ϣ
glm::vec3 lightPos(0.8f, 1.0f, -0.2f);

//֡��ʱ���
GLfloat deltaTime = 0.0f;    // Time between current frame and last frame
GLfloat lastFrame = 0.0f;      // Time of last frame

using namespace std;

// The MAIN function, from here we start the application and run the game loop
int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Camera3", NULL, NULL);
	if (!window) {
		cout << "Error::GLFW could not create window!" << endl;
		glfwTerminate();
		return -1;
	}
	//�����Ĵ��ڵ�contextָ��Ϊ��ǰcontext
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	//��겶��ͣ���ڳ�����
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// Initialize GLAD to setup the OpenGL Function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//���ô��ڲ���
	glViewport(0, 0, WIDTH, HEIGHT);

	//��ʼ��obj�ļ��м�������
	ObjLoader loader;
	vector<Vertex> vertexData;

	//�ļ����ز��ɹ����˳�����
	if (!loader.loadObjFile("eight.uniform.obj", vertexData)) {
		cout << "Could not load objmodel, exit now." << endl;
		system("pause");
		exit(-1);
	}
	//����mesh����
	Mesh mesh(vertexData);

	//������ɫ��shader
	Shader shader("lighting.vert.glsl", "lighting.material.glsl");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	//��ʼ��Ϸ��ѭ��
	while (!glfwWindowShouldClose(window)) {
		//��ȡ��ǰ֡��
		GLfloat currentFrame = (GLfloat)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//�����¼�,�ƶ�����
		glfwPollEvents();
		do_movement();

		//�����ɫ������������Ϊָ����ɫ
		glClearColor(0.2f, 0.3f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.Use();
		
		//����������ʣ��Բ�ͬ�����ɫ����ϵ��
		//material properties
		GLint matAmbientLoc = glGetUniformLocation(shader.Program, "material.ambient");
		GLint matDifussLoc = glGetUniformLocation(shader.Program, "material.diffuse");
		GLint matSpecularLoc = glGetUniformLocation(shader.Program, "material.specular");
		GLint matShineLoc = glGetUniformLocation(shader.Program, "material.shininess");
		
		glUniform3f(matAmbientLoc, 0.4f, 0.2f,0.7f);
		glUniform3f(matDifussLoc, 0.0f, 0.0f, 0.0f);
		glUniform3f(matSpecularLoc, 0.6f, 0.8f, 0.2f);
		glUniform1f(matShineLoc, 32.0f);

		//light and cemara properties
		GLint viewPosLoc = glGetUniformLocation(shader.Program, "material.viewPos");
		GLint lightAmbientLoc = glGetUniformLocation(shader.Program, "light.ambient");
		GLint lightDiffuseLoc = glGetUniformLocation(shader.Program, "light.diffuse");
		GLint lightSpecularLoc = glGetUniformLocation(shader.Program, "light.specular");
		GLint lightPosLoc = glGetUniformLocation(shader.Program, "light.position");

		glUniform3f(lightAmbientLoc, 1.0f, 0.8f, 0.2f);
		glUniform3f(lightDiffuseLoc, 0.3f, 1.0f, 0.5f);
		glUniform3f(lightSpecularLoc, 0.3f, 1.0f, 0.4f);
		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);
		
	
		//camera transformation
		//�ӱ任����
		glm::mat4 view = camera.GetViewMatrix();
		//ͶӰ����
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
			(GLfloat)WIDTH / (GLfloat)HEIGHT, 1.0f, 100.0f);
		glm::mat4 model(1);

		//get location for the matrix
		GLint modelLoc = glGetUniformLocation(shader.Program, "model");
		GLint viewLoc = glGetUniformLocation(shader.Program, "view");
		GLint projLoc = glGetUniformLocation(shader.Program, "projection");

		//pass the matrices to the shader
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		//����ģ��
		mesh.draw(shader);
		glBindVertexArray(0);
		glUseProgram(0);

		//��������
		glfwSwapBuffers(window);

	}	
	//�ͷ���Դ
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	WIDTH = width;
	HEIGHT = height;
	glViewport(0, 0, width, height);
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void do_movement()
{
	// Camera controls
	GLfloat cameraSpeed = 1.0f * deltaTime;
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to left
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
