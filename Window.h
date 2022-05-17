#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getmuevex() { return muevex; }
	GLfloat getmuevez() { return muevez; }
	GLfloat getmueveHelix() { return mueveHelix; }
	GLfloat getmueveHeliy() { return mueveHeliy; }
	GLfloat getmueveHeliz() { return mueveHeliz; }
	GLfloat getapagaP() { return apagaP; }
	GLfloat getapagaM() { return apagaM; }
	GLfloat getapagal() { return apagal; }
	GLfloat getapagaC() { return apagaC; }
	GLfloat getapagaA1() { return apagaA1; }
	GLfloat getflag() { return flag; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }

	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLfloat muevex;
	GLfloat muevez;
	GLfloat mueveHelix;
	GLfloat mueveHeliy;
	GLfloat mueveHeliz;
	GLfloat apagaP;
	GLfloat apagaM;
	GLfloat apagal;
	GLfloat apagaC;
	GLfloat apagaA1;
	GLfloat flag;
	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

	

	

};

