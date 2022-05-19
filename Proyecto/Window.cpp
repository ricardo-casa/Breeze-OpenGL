#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	muevex = 0.0f;
	muevez = 0.0f;
	mueveHelix = 30.0f;
	mueveHeliy = 5.0f;
	mueveHeliz = -65.0f;
	apagaP = 1.0f;
	apagaM = 1.0f;
	apagal = 1.0f;
	apagaC = 1.0f;
	apagaA1 = 0.0f;
	flag = 1.0f;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicializaci�n de GLFW
	if (!glfwInit())
	{
		printf("Fall� inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Proyecto", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tama�o de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Fall� inicializaci�n de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se est� usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}




void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_T && action == GLFW_PRESS)
	{
		if (theWindow->apagal == 1.0) {
			theWindow->apagal = 0.0;
		}
		else {
			theWindow->apagal = 1.0;
		}
	}
	if (key == GLFW_KEY_U && action == GLFW_PRESS)
	{
		if (theWindow->apagaC == 1.0) {
			theWindow->apagaC = 2.0;
		}
		else if(theWindow->apagaC == 2.0){
			theWindow->apagaC = 3.0;
		}
		else {
			theWindow->apagaC = 1.0;
		}
	}
	
	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		if (theWindow->apagaA1 == 1.0) {
			theWindow->apagaA1 = 0.0;
		}
		else {
			theWindow->apagaA1 = 1.0;
		}
	}
	
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (theWindow->apagaP == 1.0) {
			theWindow->apagaP = 0.0;
		}
		else {
			theWindow->apagaP = 1.0;
		}
	}
	
	if (key == GLFW_KEY_M && action == GLFW_PRESS)
	{
		if (theWindow->apagaM == 1.0) {
			theWindow->apagaM = 0.0;
		}
		else {
			theWindow->apagaM = 1.0;
		}
	}
	
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			printf("se solto la tecla %d'\n", key);
		}
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}



Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
