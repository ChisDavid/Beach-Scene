//
//  main.cpp
//  OpenGL Advances Lighting
//
//  Created by CGIS on 28/11/16.
//  Copyright � 2016 CGIS. All rights reserved.
//
#define TRUE 2+3

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Shader.hpp"
#include "Model3D.hpp"
#include "Camera.hpp"
#include "stdio.h"
#include <iostream>
#include <cstdlib>


#include <iostream>
#include "SkyBox.hpp"
#include <math.h>
#include <chrono>
#include <thread>


#include <Windows.h>
//#include <mmsystem.h>
using namespace std;

int glWindowWidth = 1400;
int glWindowHeight = 1000;
int retina_width, retina_height;
GLFWwindow* glWindow = NULL;

const unsigned int SHADOW_WIDTH = 2048;
const unsigned int SHADOW_HEIGHT = 2048;
int nrFelinare = 5;
float anglebird = 0;
glm::mat4 model;
GLuint modelLoc;
glm::mat4 view;
GLuint viewLoc;
glm::mat4 projection;
GLuint projectionLoc;
glm::mat3 normalMatrix;
GLuint normalMatrixLoc;
glm::mat4 lightRotation;

glm::vec3 lightDir;
GLuint lightDirLoc;
glm::vec3 lightColor;
GLuint lightColorLoc;

//MINE
//GLuint shadowMapFBO;
//GLuint depthMapTexture;

//END MINE
gps::Camera myCamera(
	glm::vec3(12.9731, 0.82f, 45.29f),
	glm::vec3(13.86f, 0.88f, 44.83f),
	glm::vec3(0.0f, 1.0f, 0.0f));
float cameraSpeed = 0.03f;
float DeltaX = 0, DeltaY;
bool pressedKeys[1024];
float fogIntensity = 4;
float angleY = 0.0f;
GLfloat lightAngle;
float sunFactor = 1;
double Tinit, Tfin;
gps::SkyBox mySkyBox;
gps::Shader skyboxShader;
gps::Model3D nanosuit;
gps::Model3D ground;
gps::Model3D sun;
gps::Model3D lightCube;
gps::Model3D screenQuad;
gps::Model3D farul;
gps::Model3D obj;
gps::Model3D plane;
gps::Model3D bird1;
gps::Model3D scena;
gps::Model3D tufis;
gps::Model3D raindrop;
gps::Model3D frunzePalmieri;


gps::Shader myCustomShader;
gps::Shader lightShader;
gps::Shader screenQuadShader;
gps::Shader depthMapShader;
gps::Shader tufisShader;
GLuint shadowMapFBO;
GLuint depthMapTexture;



FILE* f = fopen("coordonate.bin", "rb");
FILE* g = fopen("cameraTarget.bin", "rb");
bool showDepthMap;
float brightness = 0.5f;

bool scrie = false;
GLenum glCheckError_(const char* file, int line) {
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

void windowResizeCallback(GLFWwindow* window, int width, int height) {
	fprintf(stdout, "window resized to width: %d , and height: %d\n", width, height);
	//TODO	
	
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_M && action == GLFW_PRESS)
		showDepthMap = !showDepthMap;

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			pressedKeys[key] = true;
		else if (action == GLFW_RELEASE)
			pressedKeys[key] = false;
	}
}

bool begin1 = true;
float xpos0, ypos0, deltax, deltay;

float notAllowY = 0.0f;
void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	if (begin1) {
		xpos0 = xpos;
		ypos0 = ypos;
		begin1 = false;
	}
	else {
		deltax = xpos - xpos0;
		deltay = ypos - ypos0;
		if (notAllowY + deltay < 800.0f && notAllowY + deltay >-800) {

			notAllowY += deltay;
			myCamera.rotate(-0.001f * deltay, 0.001f * deltax);
		}
		xpos0 = xpos;
		ypos0 = ypos;
	}

}
int fog=1;
bool rain = false;
float dist(glm::vec3 unu, glm::vec3 doi)
{
	glm::vec3 diferenta = doi - unu;
	return sqrt(pow(diferenta.x, 2) + pow(diferenta.y, 2) + pow(diferenta.z, 2));
}

void camera_Move(glm::vec3 *position, glm::vec3* target, glm::vec3 to)
{
	float d = dist(*position, to);
	float distanta = dist(*position, to);
	glm::vec3 diferenta = to - *position;
	//while (distanta > 1)
	{
	//myCamera.move1(cameraSpeed, glm::vec3(diferenta.x / d, diferenta.y / d, diferenta.z / d));
		//std::this_thread::sleep_for(std::chrono::milliseconds(500));
		glm::vec3 direction = glm::vec3(diferenta.x/d,diferenta.y/d,diferenta.z/d);
		*position += direction;
		*target += direction;

		
		//myCamera.move1(cameraSpeed, glm::vec3(diferenta.x / d, diferenta.y / d, diferenta.z / d));

		distanta = dist(*position, to);
		
		std::cout << distanta << "\n";
	}

	
	
}
int panorama = 0;
glm::vec3 pozition(0, 0, 0);
glm::vec3 target(0, 0, 0);
bool prezentare = false;
void processMovement()
{
	if (pressedKeys[GLFW_KEY_Q]) {
		fog = -1;
		fogIntensity = 0;
		myCustomShader.useShaderProgram();
		glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "fogIntensity"), fogIntensity);

		skyboxShader.useShaderProgram();
		glUniform1f(glGetUniformLocation(skyboxShader.shaderProgram, "fogIntensity"), fogIntensity);

		tufisShader.useShaderProgram();
		glUniform1f(glGetUniformLocation(tufisShader.shaderProgram, "fogIntensity"), fogIntensity);
		std::cout << fogIntensity << "\n";
		
	}
	if (pressedKeys[GLFW_KEY_E]) {
		fog = 1;
		fogIntensity = 4;
		myCustomShader.useShaderProgram();
		glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "fogIntensity"), fogIntensity);

		skyboxShader.useShaderProgram();
		glUniform1f(glGetUniformLocation(skyboxShader.shaderProgram, "fogIntensity"), fogIntensity);
		std::cout << fogIntensity << "\n";
		std::cout << fog << "\n";
		tufisShader.useShaderProgram();
		glUniform1f(glGetUniformLocation(tufisShader.shaderProgram, "fogIntensity"), fogIntensity);
		std::cout << fog << "\n";
	}

	if (pressedKeys[GLFW_KEY_W]) {

		myCamera.move(gps::MOVE_FORWARD, cameraSpeed);

	}
	if (pressedKeys[GLFW_KEY_9])
	{
		prezentare = false;
	}
	if (pressedKeys[GLFW_KEY_0]) 
	{
		
		pozition = myCamera.cameraPosition;
		target = myCamera.cameraTarget;
	   	Tinit = glfwGetTime();
		prezentare = true;
	
		
	}
	if (pressedKeys[GLFW_KEY_J]) {
		lightAngle -= 1.0f;
	}
	if (pressedKeys[GLFW_KEY_P]) {
		sunFactor += 0.01;
		lightAngle += 1.0f;
	}
	if (pressedKeys[GLFW_KEY_O]) {
		sunFactor -= 0.01;
		lightAngle -= 1.0f;
	}

	if (pressedKeys[GLFW_KEY_F]) {
		
		if (fog == 1)
		{
			fogIntensity = fogIntensity + 2;
		}
		
		std::cout << "FogIntensity = " << fogIntensity << "\n";
		myCustomShader.useShaderProgram();
		glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "fogIntensity"), fogIntensity);

		skyboxShader.useShaderProgram();
		glUniform1f(glGetUniformLocation(skyboxShader.shaderProgram, "fogIntensity"), fogIntensity);
		
	}
	if (pressedKeys[GLFW_KEY_G]) {
	
			if (fog == 1)
			{
				fogIntensity = fogIntensity - 2;
			}

			std::cout << "FogIntensity = " << fogIntensity << "\n";
			myCustomShader.useShaderProgram();
			glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "fogIntensity"), fogIntensity);
			skyboxShader.useShaderProgram();
			glUniform1f(glGetUniformLocation(skyboxShader.shaderProgram, "fogIntensity"), fogIntensity);
	
		
		
	}
	if (glfwGetKey(glWindow, GLFW_KEY_Z)) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (glfwGetKey(glWindow, GLFW_KEY_X)) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(5.f);
	}
	if (glfwGetKey(glWindow, GLFW_KEY_C)) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		glPointSize(5.f);
	}
	if (pressedKeys[GLFW_KEY_B]) {
		
		brightness +=0.2;
		
		myCustomShader.useShaderProgram();
		glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "brightness"), brightness);
		skyboxShader.useShaderProgram();
		glUniform1f(glGetUniformLocation(skyboxShader.shaderProgram, "brightness"), brightness/2);
		tufisShader.useShaderProgram();
		glUniform1f(glGetUniformLocation(tufisShader.shaderProgram, "brightness"), brightness/2 );

	}
	if (pressedKeys[GLFW_KEY_V]) {
		
		if (brightness - 0.1 > 0)
		{
			if (brightness < 1)
			{
				brightness -= 0.1;
			}
			else
			{
				brightness -= 0.2;
			}


			std::cout << brightness << "\n";
			skyboxShader.useShaderProgram();
			glUniform1f(glGetUniformLocation(skyboxShader.shaderProgram, "brightness"), brightness/2);
			myCustomShader.useShaderProgram();
			glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "brightness"), brightness);
			tufisShader.useShaderProgram();
			glUniform1f(glGetUniformLocation(tufisShader.shaderProgram, "brightness"), brightness/2 );
		}
		
	}




	if (pressedKeys[GLFW_KEY_L]) {
		lightAngle += 1.0f;
	}
	if (pressedKeys[GLFW_KEY_UP]) {
		std::cout << myCamera.cameraPosition.y << "\n";
		myCamera.cameraPosition.y += 0.1;
		myCamera.cameraTarget.y += 0.1;
	}
	if (pressedKeys[GLFW_KEY_DOWN]) {
		std::cout << myCamera.cameraPosition.y << "\n";
		myCamera.cameraPosition.y -= 0.1;
		myCamera.cameraTarget.y -= 0.1;
	}

	if (pressedKeys[GLFW_KEY_R]) {

		std::cout << rain << "\n";
		rain =1;

	}
	if (pressedKeys[GLFW_KEY_T]) {

		std::cout << rain << "\n";
		rain = 0;

	}


	if (pressedKeys[GLFW_KEY_S]) {
		myCamera.move(gps::MOVE_BACKWARD, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_A]) {
		myCamera.move(gps::MOVE_LEFT, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_D]) {
		myCamera.move(gps::MOVE_RIGHT, cameraSpeed);
	}
}

bool initOpenGLWindow()
{
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	glWindow = glfwCreateWindow(glWindowWidth, glWindowHeight, "Miami Beach", NULL, NULL);
	if (!glWindow) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return false;
	}

	glfwSetWindowSizeCallback(glWindow, windowResizeCallback);
	glfwSetKeyCallback(glWindow, keyboardCallback);
	glfwSetCursorPosCallback(glWindow, mouseCallback);
	glfwSetInputMode(glWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwMakeContextCurrent(glWindow);

	glfwSwapInterval(1);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	//for RETINA display
	glfwGetFramebufferSize(glWindow, &retina_width, &retina_height);

	return true;
}

void initOpenGLState()
{
	glClearColor(0, 1, 0.3, 1.0);
	glViewport(0, 0, retina_width, retina_height);

	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glEnable(GL_CULL_FACE); // cull face
    //glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CCW); // GL_CCW for counter clock-wise

	glEnable(GL_FRAMEBUFFER_SRGB);
	/*glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/
}
void initObjects() {

	lightCube.LoadModel("objects/sun/Moon.obj");
	plane.LoadModel("objects/plane/plane.obj");
	bird1.LoadModel("objects/gull/bird.obj");
	scena.LoadModel("objects/Beach/plaja.obj");
	raindrop.LoadModel("objects/Beach/raindrop/drop.obj");
	frunzePalmieri.LoadModel("objects/Beach/gusti1.obj");

}

void skyBox()
{

	std::vector<const GLchar*> faces;
	faces.push_back("Textures/skybox/right.tga");
	faces.push_back("Textures/skybox/left.tga");
	faces.push_back("Textures/skybox/top.tga");
	faces.push_back("Textures/skybox/bottom.tga");
	faces.push_back("Textures/skybox/back.tga");
	faces.push_back("Textures/skybox/front.tga");

	mySkyBox.Load(faces);
	skyboxShader.loadShader("shaders/skyboxShader.vert", "shaders/skyboxShader.frag");
	skyboxShader.useShaderProgram();

}
void initShaders() {
	myCustomShader.loadShader("shaders/shaderStart.vert", "shaders/shaderStart.frag");
	myCustomShader.useShaderProgram();

	lightShader.loadShader("shaders/lightCube.vert", "shaders/lightCube.frag");
	lightShader.useShaderProgram();

	screenQuadShader.loadShader("shaders/screenQuad.vert", "shaders/screenQuad.frag");
	screenQuadShader.useShaderProgram();

	depthMapShader.loadShader("shaders/lightVertex.vert", "shaders/lightVertex.frag");
	depthMapShader.useShaderProgram();


	tufisShader.loadShader("shaders/leafShader.vert", "shaders/leafShader.frag");
	tufisShader.useShaderProgram();




	skyBox();

}

void initUniforms() {

	
	
	skyboxShader.useShaderProgram();
	glUniform1f(glGetUniformLocation(skyboxShader.shaderProgram, "fogIntensity"), fogIntensity);
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "brightness"), brightness);

	myCustomShader.useShaderProgram();
	//glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "nrFelinare"), nrFelinare);
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "brightness"), brightness);
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "fogIntensity"), fogIntensity);

	model = glm::mat4(1.0f);
	modelLoc = glGetUniformLocation(myCustomShader.shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	view = myCamera.getViewMatrix();
	viewLoc = glGetUniformLocation(myCustomShader.shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
	normalMatrixLoc = glGetUniformLocation(myCustomShader.shaderProgram, "normalMatrix");
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
	projectionLoc = glGetUniformLocation(myCustomShader.shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//set the light direction (direction towards the light)
	lightDir = glm::vec3(0.0f, 1.0f, 1.0f);
	lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	lightDirLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightDir");
	glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view * lightRotation)) * lightDir));

	//set light color
	lightColor = glm::vec3(1.0f, 1.0f, 1.0f); //white light
	lightColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor");
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

	lightShader.useShaderProgram();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));




	/// <summary>
	/// // SKYBOX
	/// </summary>

	view = myCamera.getViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.shaderProgram, "view"), 1, GL_FALSE,
		glm::value_ptr(view));

	projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.shaderProgram, "projection"), 1, GL_FALSE,
		glm::value_ptr(projection));


	tufisShader.useShaderProgram();
	glUniformMatrix4fv(glGetUniformLocation(tufisShader.shaderProgram, "view"), 1, GL_FALSE,
		glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(tufisShader.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	



}
void addObject()
{

}

void initFBO() {
	//TODO - Create the FBO, the depth texture and attach the depth texture to the FBO
	glGenFramebuffers(1, &shadowMapFBO);

	glGenTextures(1, &depthMapTexture);
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

glm::mat4 computeLightSpaceTrMatrix() {
	//TODO - Return the light-space transformation matrix
	glm::mat4 lightView = glm::lookAt(glm::inverseTranspose(glm::mat3( lightRotation)) * lightDir, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	const GLfloat near_plane = 0.1f, far_plane = 100.0f;
	glm::mat4 lightProjection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, near_plane, far_plane);
	glm::mat4 lightSpaceTrMatrix = lightProjection * lightView;

	return lightSpaceTrMatrix;
}
int sens = 1;
void drowRain(gps:: Shader shader)
{
	
	for (int i = 0; i < 1000; i++)
	{
		int x = rand() % 14 - 6;
		int z = rand() % 23 + 13;
		int y = rand() % 100;
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(x, y, z));
		glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		raindrop.Draw(shader);
		
		
	}
}
void drawObjects(gps::Shader shader, bool depthPass) {




	


	shader.useShaderProgram();
	scena.Draw(shader);

	if (rain)
	{
		drowRain(shader);
	}





	

	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(anglebird *10), glm::vec3(0.0f, -1.0f, 0.0f));
	anglebird += 0.111f;

	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	//raindrop.Draw(shader);
	
	 bird1.Draw(shader);
	

	
	model = glm::translate(glm::mat4(1.0f), glm::vec3(anglebird*10, 10.0f, 0.0f));
	model = glm::scale(model, glm::vec3(5.0f));
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	// do not send the normal matrix if we are rendering in the depth map
	if (!depthPass) {
		normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	}
	

	plane.Draw(shader);
	//model = glm::rotate(glm::mat4(1.0f), glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));
	//glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	//// do not send the normal matrix if we are rendering in the depth map
	//if (!depthPass) {
	//	normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
	//	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	//}

	//nanosuit.Draw(shader);
	//sun.Draw(shader);




	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(20.0f));
	
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	// do not send the normal matrix if we are rendering in the depth map
	if (!depthPass) {
		normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	}

	ground.Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, -1.0f, 0.0f));

	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	// do not send the normal matrix if we are rendering in the depth map
	if (!depthPass) {
		normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	}
	farul.Draw(shader);

	//model = glm::mat4(1.0f);
	//glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	
	//tufis.Draw(shader);



}
float x = 0, y = 0, z = 0, xold, yold, zold;
void renderScene() {

	// depth maps creation pass
	//TODO - Send the light-space transformation matrix to the depth map creation shader and
	//		 render the scene in the depth map
	Tfin = glfwGetTime();
	//std::cout << Tfin - Tinit << "\n";
	int t = (int)Tfin - Tinit;
	//cout << t << "\n";





	
	if (prezentare == 1)
	{


		
		fread(&x, sizeof(x), 1, f);
		fread(&y, sizeof(y), 1, f);
		fread(&z, sizeof(z), 1, f);
		if (x == xold)
		{
			prezentare = 0;
			cout << "am oprit prezentarea";
		}
		else
		{
			cout << "\n" << x << " " << y << " " << z << " \n";
			myCamera.cameraPosition.x = x;
			myCamera.cameraPosition.y = y;
			myCamera.cameraPosition.z = z;
			xold = x;
			yold = y;
			zold = z;
			fread(&x, sizeof(x), 1, g);
			fread(&y, sizeof(y), 1, g);
			fread(&z, sizeof(z), 1, g);
			myCamera.cameraTarget.x = x;
			myCamera.cameraTarget.y = y;
			myCamera.cameraTarget.z = z;
		}
	
	}
		
		
	
	

	depthMapShader.useShaderProgram();
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "lightSpaceTrMatrix"),
		1,
		GL_FALSE,
		glm::value_ptr(computeLightSpaceTrMatrix()));
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	drawObjects(depthMapShader, true);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// render depth map on screen - toggled with the M key

	if (showDepthMap) {
		glViewport(0, 0, retina_width, retina_height);

		glClear(GL_COLOR_BUFFER_BIT);

		screenQuadShader.useShaderProgram();

		//bind the depth map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMapTexture);
		glUniform1i(glGetUniformLocation(screenQuadShader.shaderProgram, "depthMap"), 0);

		glDisable(GL_DEPTH_TEST);
		screenQuad.Draw(screenQuadShader);
		glEnable(GL_DEPTH_TEST);
	}
	else {
		// final scene rendering pass (with shadows)

		glViewport(0, 0, retina_width, retina_height);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	
		myCustomShader.useShaderProgram();

		view = myCamera.getViewMatrix();
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view * lightRotation)) * lightDir));

		//bind the shadow map
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, depthMapTexture);
		glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "shadowMap"), 3);

		glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "lightSpaceTrMatrix"),
			1,
			GL_FALSE,
			glm::value_ptr(computeLightSpaceTrMatrix()));

		drawObjects(myCustomShader, false);

		tufisShader.useShaderProgram();
		glUniformMatrix4fv(glGetUniformLocation(tufisShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
	
		glUniformMatrix4fv(glGetUniformLocation(tufisShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

		frunzePalmieri.Draw(tufisShader);
		//draw a white cube around the light

		lightShader.useShaderProgram();

		glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

		//model = lightRotation;
		//std::cout << sunFactor*1000 << "\n";
		//if (sin(sunFactor)*215>24)
		//	{
			model = glm::translate(model, glm::vec3(0, sin(sunFactor) * 215, -cos(sunFactor) * 215));
	//	}
		
		//model = glm::scale(glm::vec3(4,4, 4));
		//model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

		lightCube.Draw(lightShader);


		tufisShader.useShaderProgram();
		glUniformMatrix4fv(glGetUniformLocation(tufisShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
		//model = glm::mat4(1.0f);
		glUniformMatrix4fv(glGetUniformLocation(tufisShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		tufis.Draw(tufisShader);

		mySkyBox.Draw(skyboxShader, view, projection);



		


	}

}

void cleanup() {
	glDeleteTextures(1, &depthMapTexture);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &shadowMapFBO);
	glfwDestroyWindow(glWindow);
	//close GL context and any other GLFW resources
	glfwTerminate();
}

int main(int argc, const char* argv[]) {

	if (!initOpenGLWindow()) {
		glfwTerminate();
		return 1;
	}
	
	initOpenGLState();
	initObjects();
	initShaders();
	initUniforms();
	initFBO();
	DWORD Volum = 0x11111111;
	PlaySound(TEXT("Naruto.wav"), NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
	waveOutSetVolume(NULL, Volum);
	glCheckError();
	glDisable(GL_CULL_FACE);
	while (!glfwWindowShouldClose(glWindow)) {
		processMovement();
		renderScene();

		glfwPollEvents();
		glfwSwapBuffers(glWindow);
	}

	cleanup();

	return 0;
}
