#include <iostream>
#include <math.h>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL.h>
#include "shader.h"
#include "camera.h"
#include "Sphere.h"

using namespace std;

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
//�󶨶���
void setupBuffers(GLuint& VBO, GLuint& VAO, GLuint& EBO, const Sphere s);
//������
void setupTexture(GLuint& texture, const char* path);
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

//��ǰ�꣬��
static int day = 0;
static int  mercuryYear = 0, venusYear = 0, earthYear = 0,
marsYear = 0, jupiterYear = 0, saturnYear = 0, uranusYear = 0,
neptuneYear = 0;


// The MAIN function, from here we start the application and run the game loop
int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Planet", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	//������겶��
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);


	// Initialize GLAD to setup the OpenGL Function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize glad" << std::endl;
		return -1;
	};

	// Define the viewport dimensions
	glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_TEXTURE_1D | GL_TEXTURE_2D | GL_TEXTURE_CUBE_MAP);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
	// Build and compile our shader program
	Shader sun_shader("glsl/main3.vert.glsl", "glsl/main3.frag.glsl");//̫��
	Shader mercury_shader("glsl/main3.vert.glsl", "glsl/main3.frag.glsl");//ˮ��
	Shader venus_shader("glsl/main3.vert.glsl", "glsl/main3.frag.glsl");//����
	Shader earth_shader("glsl/main3.vert.glsl", "glsl/main3.frag.glsl");//����
	Shader mars_shader("glsl/main3.vert.glsl", "glsl/main3.frag.glsl");//����
	Shader jupiter_shader("glsl/main3.vert.glsl", "glsl/main3.frag.glsl");//ľ��
	Shader saturn_shader("glsl/main3.vert.glsl", "glsl/main3.frag.glsl");//����
	Shader uranus_shader("glsl/main3.vert.glsl", "glsl/main3.frag.glsl");//������
	Shader neptune_shader("glsl/main3.vert.glsl", "glsl/main3.frag.glsl");//������

	
	
	// Set up vertex data (and buffer(s)) and attribute pointers
	Sphere sphere(2.0f, 40, 40);

	//̫��
	GLuint sunVBO, sunVAO, sunEBO;
	setupBuffers(sunVBO, sunVAO, sunEBO,sphere);
	GLuint sun_text;
	setupTexture(sun_text, "texture/sun.jpg");

	//ˮ��
	GLuint mercuryVBO, mercuryVAO, mercuryEBO;
	setupBuffers(mercuryVBO, mercuryVAO, mercuryEBO, sphere);
	GLuint mercury_text;
	setupTexture(mercury_text, "texture/mercury.jpg");

	//����
	GLuint venusVBO, venusVAO, venusEBO;
	setupBuffers(venusVBO, venusVAO, venusEBO, sphere);
	GLuint venus_text;
	setupTexture(venus_text, "texture/venus.jpg");

	//����
	GLuint earthVBO, earthVAO, earthEBO;
	setupBuffers(earthVBO, earthVAO, earthEBO,sphere);
	GLuint earth_text;
	setupTexture(earth_text, "texture/earth.jpg");

	//����
	GLuint marsVBO, marsVAO, marsEBO;
	setupBuffers(marsVBO,marsVAO,marsEBO,sphere);
	GLuint mars_text;
	setupTexture(mars_text, "texture/mars.jpg");

	//ľ��
	GLuint jupiterVBO,jupiterVAO, jupiterEBO;
	setupBuffers(jupiterVBO, jupiterVAO, jupiterEBO, sphere);
	GLuint jupiter_text;
	setupTexture(jupiter_text,"texture/jupiter.jpg");

	//����
	GLuint saturnVBO, saturnVAO, saturnEBO;
	setupBuffers(saturnVBO, saturnVAO, saturnEBO, sphere);
	GLuint saturn_text;
	setupTexture(saturn_text, "texture/saturn.jpg");

	//������
	GLuint uranusVBO, uranusVAO, uranusEBO;
	setupBuffers(uranusVBO, uranusVAO, uranusEBO, sphere);
	GLuint uranus_text;
	setupTexture(uranus_text, "texture/uranus.jpg");

	//������
	GLuint neptuneVBO, neptuneVAO, neptuneEBO;
	setupBuffers(neptuneVBO, neptuneVAO, neptuneEBO, sphere);
	GLuint neptune_text;
	setupTexture(neptune_text, "texture/neptune.jpg");


	
	
	//ѭ��֡��
	while (!glfwWindowShouldClose(window))
	{
		//��ȡ��ǰʱ��
		GLfloat time = (GLfloat)glfwGetTime();
		//��ȡ��ǰ֡��
		GLfloat currentFrame = (GLfloat)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//�����¼�,�ƶ�����
		glfwPollEvents();
		do_movement();

		// Render
		// Clear the colorbuffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		//����ӱ任����
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
			(GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

		//����̫���任����
		sun_shader.Use();
		//��̫����ת??
		glm::mat4 model(1);
		model = glm::rotate_slow(model,time*0.05f*glm::radians(360.0f),glm::vec3(0.0f, 1.0f, 0.0f));
		
		//get location for the matrix
		GLint modelLoc = glGetUniformLocation(sun_shader.Program, "model");
		GLint viewLoc = glGetUniformLocation(sun_shader.Program, "view");
		GLint projLoc = glGetUniformLocation(sun_shader.Program, "projection");

		//pass the matrices to the shader
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		//��̫��
		glBindTexture(GL_TEXTURE_2D, sun_text);
		glBindVertexArray(sunVAO);
		glDrawElements(GL_TRIANGLES, sphere.getIndexCount(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		//ˮ��
		mercury_shader.Use();
		modelLoc = glGetUniformLocation(mercury_shader.Program, "model");
		viewLoc = glGetUniformLocation(mercury_shader.Program, "view");
		projLoc = glGetUniformLocation(mercury_shader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::vec3 mercuryLoc = glm::vec3(2.2f,0.0f,2.2f);
		model = glm::mat4(1);
		//��ת
		model = glm::rotate_slow(model, (GLfloat)mercuryYear * glm::radians(360.0f)*time*0.0001f, glm::vec3(0.0, 1.0, 0.0));
		model = glm::translate(model, mercuryLoc);
		//����
		model = glm::scale(model, glm::vec3(0.11f));
		//�Դ�
		model = glm::rotate_slow(model, (GLfloat)day * glm::radians(360.0f)*time*0.001f, glm::vec3(0.0, 1.0, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glBindTexture(GL_TEXTURE_2D, mercury_text);
		glBindVertexArray(mercuryVAO);
		glDrawElements(GL_TRIANGLES, sphere.getIndexCount(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		//����
		venus_shader.Use();
		modelLoc = glGetUniformLocation(venus_shader.Program, "model");
		viewLoc = glGetUniformLocation(venus_shader.Program, "view");
		projLoc = glGetUniformLocation(venus_shader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::vec3 venusLoc = glm::vec3(2.6f, 0.0f,2.6f);
		model = glm::mat4(1);
		//��ת
		model = glm::rotate_slow(model, (GLfloat)venusYear * glm::radians(360.0f) * time * 0.0001f, glm::vec3(0.0, 1.0, 0.0));
		model = glm::translate(model, venusLoc);
		//����
		model = glm::scale(model, glm::vec3(0.2f));
		//�Դ�
		model = glm::rotate_slow(model, (GLfloat)day * glm::radians(360.0f) * time * 0.001f, glm::vec3(0.0, 1.0, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glBindTexture(GL_TEXTURE_2D, venus_text);
		glBindVertexArray(venusVAO);
		glDrawElements(GL_TRIANGLES, sphere.getIndexCount(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		//����
		earth_shader.Use();
		modelLoc = glGetUniformLocation(earth_shader.Program, "model");
		viewLoc = glGetUniformLocation(earth_shader.Program, "view");
		projLoc = glGetUniformLocation(earth_shader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::vec3 earthLoc = glm::vec3(3.0f, 0.0f,-3.0f );
		model = glm::mat4(1);
		//��ת
		model = glm::rotate_slow(model, (GLfloat)earthYear * glm::radians(360.0f) * time * 0.0001f, glm::vec3(0.0, 1.0, 0.0));
		model = glm::translate(model, earthLoc);
		//����
		model = glm::scale(model, glm::vec3(0.16f));
		//�Դ�
		model = glm::rotate_slow(model, (GLfloat)day * glm::radians(360.0f) * time * 0.001f, glm::vec3(0.0, 1.0, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glBindTexture(GL_TEXTURE_2D, earth_text);
		glBindVertexArray(earthVAO);
		glDrawElements(GL_TRIANGLES, sphere.getIndexCount(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		//����
		mars_shader.Use();
		modelLoc = glGetUniformLocation(mars_shader.Program, "model");
		viewLoc = glGetUniformLocation(mars_shader.Program, "view");
		projLoc = glGetUniformLocation(mars_shader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::vec3 marsLoc = glm::vec3(0.0f,0.0f,3.6f );
		model = glm::mat4(1);
		//��ת
		model = glm::rotate_slow(model, (GLfloat)marsYear * glm::radians(360.0f) * time * 0.0001f, glm::vec3(0.0, 1.0, 0.0));
		model = glm::translate(model, marsLoc);
		//����
		model = glm::scale(model, glm::vec3(0.14f));
		//�Դ�
		model = glm::rotate_slow(model, (GLfloat)day * glm::radians(360.0f) * time * 0.001f, glm::vec3(0.0, 1.0, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glBindTexture(GL_TEXTURE_2D, mars_text);
		glBindVertexArray(marsVAO);
		glDrawElements(GL_TRIANGLES, sphere.getIndexCount(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		//ľ��
		jupiter_shader.Use();
		modelLoc = glGetUniformLocation(jupiter_shader.Program, "model");
		viewLoc = glGetUniformLocation(jupiter_shader.Program, "view");
		projLoc = glGetUniformLocation(jupiter_shader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::vec3 jupiterLoc = glm::vec3(-3.9f, 0.0f,-3.9f );
		model = glm::mat4(1);
		//��ת
		model = glm::rotate_slow(model, (GLfloat)jupiterYear * glm::radians(360.0f) * time * 0.0001f, glm::vec3(0.0, 1.0, 0.0));
		model = glm::translate(model, jupiterLoc);
		//����
		model = glm::scale(model, glm::vec3(0.3f));
		//�Դ�
		model = glm::rotate_slow(model, (GLfloat)day * glm::radians(360.0f) * time * 0.001f, glm::vec3(0.0, 1.0, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glBindTexture(GL_TEXTURE_2D, jupiter_text);
		glBindVertexArray(jupiterVAO);
		glDrawElements(GL_TRIANGLES, sphere.getIndexCount(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		//����
		saturn_shader.Use();
		modelLoc = glGetUniformLocation(saturn_shader.Program, "model");
		viewLoc = glGetUniformLocation(saturn_shader.Program, "view");
		projLoc = glGetUniformLocation(saturn_shader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::vec3 saturnLoc = glm::vec3(-4.7f, 0.0f, -4.7f);
		model = glm::mat4(1);
		//��ת
		model = glm::rotate_slow(model, (GLfloat)saturnYear * glm::radians(360.0f) * time * 0.0001f, glm::vec3(0.0, 1.0, 0.0));
		model = glm::translate(model, saturnLoc);
		//����
		model = glm::scale(model, glm::vec3(0.15f));
		//�Դ�
		model = glm::rotate_slow(model, (GLfloat)day * glm::radians(360.0f) * time * 0.001f, glm::vec3(0.0, 1.0, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glBindTexture(GL_TEXTURE_2D, saturn_text);
		glBindVertexArray(saturnVAO);
		glDrawElements(GL_TRIANGLES, sphere.getIndexCount(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		//������
		uranus_shader.Use();
		modelLoc = glGetUniformLocation(uranus_shader.Program, "model");
		viewLoc = glGetUniformLocation(uranus_shader.Program, "view");
		projLoc = glGetUniformLocation(uranus_shader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::vec3 uranusLoc = glm::vec3(4.95f, 0.0f, -4.95f);
		model = glm::mat4(1);
		//��ת
		model = glm::rotate_slow(model, (GLfloat)uranusYear * glm::radians(360.0f) * time * 0.0001f, glm::vec3(0.0, 1.0, 0.0));
		model = glm::translate(model, uranusLoc);
		//����
		model = glm::scale(model, glm::vec3(0.12f));
		//�Դ�
		model = glm::rotate_slow(model, (GLfloat)day * glm::radians(360.0f) * time * 0.001f, glm::vec3(0.0, 1.0, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glBindTexture(GL_TEXTURE_2D, uranus_text);
		glBindVertexArray(uranusVAO);
		glDrawElements(GL_TRIANGLES, sphere.getIndexCount(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		//������
		neptune_shader.Use();
		modelLoc = glGetUniformLocation(neptune_shader.Program, "model");
		viewLoc = glGetUniformLocation(neptune_shader.Program, "view");
		projLoc = glGetUniformLocation(neptune_shader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::vec3 neptuneLoc = glm::vec3(0.0f, 0.0f, 5.4f);
		model = glm::mat4(1);
		//��ת
		model = glm::rotate_slow(model, (GLfloat)neptuneYear * glm::radians(360.0f) * time * 0.0001f, glm::vec3(0.0, 1.0, 0.0));
		model = glm::translate(model, neptuneLoc);
		//����
		model = glm::scale(model, glm::vec3(0.1f));
		//�Դ�
		model = glm::rotate_slow(model, (GLfloat)day * glm::radians(360.0f) * time * 0.001f, glm::vec3(0.0, 1.0, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glBindTexture(GL_TEXTURE_2D, neptune_text);
		glBindVertexArray(neptuneVAO);
		glDrawElements(GL_TRIANGLES, sphere.getIndexCount(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
				

	}
	//release resources
	glDeleteVertexArrays(1, &sunVAO);
	glDeleteVertexArrays(1, &jupiterVAO);
	glDeleteBuffers(1, &sunVBO);
	glDeleteBuffers(1, &jupiterVBO);
	glDeleteBuffers(1, &sunEBO);
	glDeleteBuffers(1, &jupiterEBO);
	glDeleteTextures(1, &sun_text);
	
	//terminate
	glfwTerminate();
	return 0;

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	WIDTH = width;
	HEIGHT = height;
	glViewport(0, 0, width, height);
}

void addTime() {
	mercuryYear = (mercuryYear + 12) % 360;
	venusYear = (venusYear + 20) % 360;
	earthYear = (earthYear + 8) % 360;
	marsYear = (marsYear + 6) % 360;
	jupiterYear = (jupiterYear + 5) % 360;
	saturnYear = (saturnYear + 4) % 360;
	uranusYear = (uranusYear + 3) % 360;
	neptuneYear = (neptuneYear + 1) % 360;
	day = (day + 30) % 360;
}
void subTime() {
	mercuryYear = (mercuryYear - 12) % 360;
	venusYear = (venusYear - 20) % 360;
	earthYear = (earthYear - 8) % 360;
	marsYear = (marsYear - 6) % 360;
	jupiterYear = (jupiterYear - 5) % 360;
	saturnYear = (saturnYear - 4) % 360;
	uranusYear = (uranusYear - 3) % 360;
	neptuneYear = (neptuneYear - 1) % 360;
	day = (day - 30) % 360;
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

	//���ǻ���
	//����ֹͣ
	if (keys[GLFW_KEY_ENTER]) {
		mercuryYear = 0;
		venusYear = 0;
		earthYear = 0;
		marsYear = 0;
		jupiterYear = 0;
		saturnYear = 0;
		uranusYear = 0;
		neptuneYear = 0;
		day = 0;
	}
	if (keys[GLFW_KEY_UP]) {
		addTime();
	}
	if (keys[GLFW_KEY_DOWN]) {
		subTime();
	}
	if (keys[GLFW_KEY_LEFT]) {
		day = (day - 10) % 360;
	}
	if (keys[GLFW_KEY_RIGHT]) {
		day = (day + 10) % 360;
	}

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

//�󶨶���
void setupBuffers(GLuint& VBO, GLuint& VAO, GLuint& EBO, const Sphere s)
{
	glGenVertexArrays(1, &VAO);
	//binding VBO,VAO
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, s.getInterleavedVertexSize(), s.getInterleavedVertices(), GL_STATIC_DRAW);
	
	//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	//binding EBO
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, s.getIndexSize(), s.getIndices(), GL_STATIC_DRAW);

	glBindVertexArray(0);


}
//������
void setupTexture(GLuint& texture, const char* path)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);    // Set texture wrapping to GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// Load, create texture and generate mipmaps
	int width, height;
	unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, 
		GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE);
	
	//�ͷ���Դ
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

}

