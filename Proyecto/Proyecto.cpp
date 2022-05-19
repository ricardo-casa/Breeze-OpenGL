/*
Semestre 2022-2
Proyecto
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include <irrKlang.h>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "Light.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"

using namespace irrklang;


const float toRadians = 3.14159265f / 180.0f;

// variables

float puerta;

// ***** variables para mover textura ****
float cordTexU;
float cordTexV;

// ***** variables para animaionde jett *****
float yJett;
float rotJett;
float banderajett;

// ***** variables para animaion de humo *****

float rotHumo;
float rotHumo2;
float scaHumo;
float banderahumo;
float movxHumo;
float movzHumo;

// ***** variables para animaion de arma*****

float rotArma;
float scaArma;
float banderaArma;

// **** variables para animacion del tiro *****
float scaTiro;
float scaTiro2;
float xTiro;
float yTiro;
float xTiro2;
float yTiro2;
float angulot;
float banderaTiro;
float banderaTiro2;
float banderaTiroS;
float banderaTiro2S;

// *** variables animacion tortuga *****

float banderaTor;
float movxTor;
float movyTor;
float movzTor;
float anguloTor;


// ***** camara ***
float yawG;
float yaw1;
float pitch1;
float yaw3;
float pitch3;

//***** iluminacion **

float dia;
float cont;
float cont2;
float bandI;
float bandI2;
float red;
float green;
float blue;

//**** animacion avatar ***

float bandA;
float movxP;
float movzP;
float rotP;
float bdP;
float biP;
float pdP;
float piP;
float band1;
float band2;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

// ***** texturas ******
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture pgTexture;
Texture escalonTexture;
Texture paredTexture;
Texture paredATexture;
Texture aguaTexture;
Texture phoenixTexture;

// **** modelos ***
Model contenedor_M;
Model caja_M;
Model marco_M;
Model puerta_M;
Model torre_M;
Model mesa_M;
Model compu_M;
Model humo_M;
Model jett_M;
Model tiro_M;
Model gr_M;
Model ak47_M;
Model pistol_M;
Model rifle_M;
Model faro_M;
Model antorcha_M;
Model shotgun_M;
Model machP_M;
Model heavyP_M;
Model tortuga_M;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;
Material Material_phoenix;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
DirectionalLight mainLight_night;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-15.0f, 0.0f, -15.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		15.0f, 0.0f, -15.0f,	15.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-15.0f, 0.0f, 15.0f,	0.0f, 15.0f,	0.0f, -1.0f, 0.0f,
		15.0f, 0.0f, 15.0f,		15.0f, 15.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int floorGIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorGVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f
	};

	

	unsigned int vegetacionIndices[] = {
		0, 1, 2,
		0, 2, 3,
		4,5,6,
		4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
	};
	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(floorGVertices, floorGIndices, 32, 6);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj5);

}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void CrearDado()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		4, 5, 6,
		6, 7, 4,
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,
	};
	
	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.14f,  0.63f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.27f,	0.63f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.27f,	0.81f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.14f,	0.81f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.27f,  0.63f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.4f,	0.63f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.4f,	0.81f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.27f,	0.81f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.52f,  0.63f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		0.4f,	0.63f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		0.4f,	0.81f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.52f,	0.81f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.1f,  0.63f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	0.14f,	0.63f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.14f,	0.81f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.1f,	0.81f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.14f,  0.63f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	0.27f,	0.63f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	0.27f,	0.65f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.14f,	0.65f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.14f,  0.81f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	0.25f,	0.81f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	0.25f,	0.99f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.14f,	0.99f,		0.0f,	-1.0f,	0.0f,

	};

	unsigned int cubo_indicesCuerpo[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		4, 5, 6,
		6, 7, 4,
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,
	};

	GLfloat cubo_verticesCuerpo[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.8f,  0.3f,	0.149f,  0.38f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.8f,  0.3f,		0.27f,	0.38f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.8f,  0.3f,		0.27f,	0.62f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.8f,  0.3f,	0.149f,	0.62f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.8f,  0.3f,	    0.27f,  0.38f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.8f,  -0.3f,	0.33f,	0.38f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.8f,  -0.3f,	0.33f,	0.62f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.8f,  0.3f,	    0.27f,	0.62f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.8f, -0.3f,	0.33f,  0.38f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.8f, -0.3f,		0.47f,	0.38f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.8f, -0.3f,		0.47f,	0.62f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.8f, -0.3f,	0.33f,	0.62f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.8f,  -0.3f,	0.47f,  0.38f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.8f,  0.3f,	0.52f,	0.38f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.8f,  0.3f,	0.52f,	0.62f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.8f,  -0.3f,	0.47f,	0.62f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.8f,  0.3f,	0.33f,  0.38f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.8f,  0.3f,	0.47f,	0.38f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.8f,  -0.3f,	0.47f,	0.41f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.8f,  -0.3f,	0.33f,	0.41f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.8f,  0.3f,	0.0f,  0.0f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.8f,  0.3f,	0.0f,	0.0f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.8f,  -0.3f,	0.0f,	0.0f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.8f,  -0.3f,	0.0f,	0.0f,		0.0f,	-1.0f,	0.0f,

	};

	unsigned int cubo_indicesBD[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		4, 5, 6,
		6, 7, 4,
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,
	};

	GLfloat cubo_verticesBD[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.2f, -1.5f,  0.2f,	0.82f,  0.45f,		0.0f,	0.0f,	-1.0f,	//0
		0.2f, -1.5f,  0.2f,		0.89f,	0.45f,		0.0f,	0.0f,	-1.0f,	//1
		0.2f,  0.1f,  0.2f,		0.89f,	0.72f,		0.0f,	0.0f,	-1.0f,	//2
		-0.2f,  0.1f,  0.2f,	0.82f,	0.72f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.2f, -1.5f,  0.2f,	    0.89f,  0.45f,		-1.0f,	0.0f,	0.0f,
		0.2f, -1.5f,  -0.2f,	0.93f,	0.45f,		-1.0f,	0.0f,	0.0f,
		0.2f,  0.1f,  -0.2f,	0.93f,	0.72f,		-1.0f,	0.0f,	0.0f,
		0.2f,  0.1f,  0.2f,	    0.89f,	0.72f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.2f, -1.5f, -0.2f,	0.93f,  0.45f,		0.0f,	0.0f,	1.0f,
		0.2f, -1.5f, -0.2f,		0.98f,	0.45f,		0.0f,	0.0f,	1.0f,
		0.2f,  0.1f, -0.2f,		0.98f,	0.72f,		0.0f,	0.0f,	1.0f,
		-0.2f,  0.1f, -0.2f,	0.93f,	0.72f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.2f, -1.5f,  -0.2f,	0.77f,  0.45f,		1.0f,	0.0f,	0.0f,
		-0.2f, -1.5f,  0.2f,	0.82f,	0.45f,		1.0f,	0.0f,	0.0f,
		-0.2f,  0.1f,  0.2f,	0.82f,	0.72f,		1.0f,	0.0f,	0.0f,
		-0.2f,  0.1f,  -0.2f,	0.77f,	0.72f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.2f, -1.5f,  0.2f,	0.89f,  0.38f,		0.0f,	1.0f,	0.0f,
		0.2f,  -1.5f,  0.2f,	0.89f,	0.45f,		0.0f,	1.0f,	0.0f,
		 0.2f,  -1.5f,  -0.2f,	0.94f,	0.45f,		0.0f,	1.0f,	0.0f,
		-0.2f, -1.5f,  -0.2f,	0.94f,	0.38f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.2f, 0.1f,  0.2f,	0.89f,  0.79f,		0.0f,	-1.0f,	0.0f,
		 0.2f,  0.1f,  0.2f,	0.89f,	0.72f,		0.0f,	-1.0f,	0.0f,
		  0.2f, 0.1f,  -0.2f,	0.93f,	0.72f,		0.0f,	-1.0f,	0.0f,
		 -0.2f, 0.1f,  -0.2f,	0.93f,	0.79f,		0.0f,	-1.0f,	0.0f,

	};

	unsigned int cubo_indicesBI[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		4, 5, 6,
		6, 7, 4,
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,
	};

	GLfloat cubo_verticesBI[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.2f, -1.5f,  0.2f,	0.64f,  0.45f,		0.0f,	0.0f,	-1.0f,	//0
		0.2f, -1.5f,  0.2f,		0.72f,	0.45f,		0.0f,	0.0f,	-1.0f,	//1
		0.2f,  0.1f,  0.2f,		0.72f,	0.72f,		0.0f,	0.0f,	-1.0f,	//2
		-0.2f,  0.1f,  0.2f,	0.64f,	0.72f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.2f, -1.5f,  0.2f,	    0.72f,  0.45f,		-1.0f,	0.0f,	0.0f,
		0.2f, -1.5f,  -0.2f,	0.76f,	0.45f,		-1.0f,	0.0f,	0.0f,
		0.2f,  0.1f,  -0.2f,	0.76f,	0.72f,		-1.0f,	0.0f,	0.0f,
		0.2f,  0.1f,  0.2f,	    0.72f,	0.72f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.2f, -1.5f, -0.2f,	0.59f,  0.45f,		0.0f,	0.0f,	1.0f,
		0.2f, -1.5f, -0.2f,		0.54f,	0.45f,		0.0f,	0.0f,	1.0f,
		0.2f,  0.1f, -0.2f,		0.54f,	0.72f,		0.0f,	0.0f,	1.0f,
		-0.2f,  0.1f, -0.2f,	0.59f,	0.72f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.2f, -1.5f,  -0.2f,	0.59f,  0.45f,		1.0f,	0.0f,	0.0f,
		-0.2f, -1.5f,  0.2f,	0.64f,	0.45f,		1.0f,	0.0f,	0.0f,
		-0.2f,  0.1f,  0.2f,	0.64f,	0.72f,		1.0f,	0.0f,	0.0f,
		-0.2f,  0.1f,  -0.2f,	0.59f,	0.72f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.2f, -1.5f,  0.2f,	0.64f,  0.45f,		0.0f,	1.0f,	0.0f,
		0.2f,  -1.5f,  0.2f,	0.64f,	0.38f,		0.0f,	1.0f,	0.0f,
		 0.2f,  -1.5f,  -0.2f,	0.59f,	0.38f,		0.0f,	1.0f,	0.0f,
		-0.2f, -1.5f,  -0.2f,	0.59f,	0.45f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.2f, 0.1f,  0.2f,	0.64f,  0.72f,		0.0f,	-1.0f,	0.0f,
		 0.2f,  0.1f,  0.2f,	0.64f,	0.79f,		0.0f,	-1.0f,	0.0f,
		  0.2f, 0.1f,  -0.2f,	0.59f,	0.79f,		0.0f,	-1.0f,	0.0f,
		 -0.2f, 0.1f,  -0.2f,	0.59f,	0.72f,		0.0f,	-1.0f,	0.0f,

	};

	unsigned int cubo_indicesPD[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		4, 5, 6,
		6, 7, 4,
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,
	};

	GLfloat cubo_verticesPD[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.25f, -1.5f,  0.3f,	0.34f,  0.1f,		0.0f,	0.0f,	-1.0f,	//0
		0.25f, -1.5f,  0.3f,	0.4f,	0.1f,		0.0f,	0.0f,	-1.0f,	//1
		0.25f,  0.1f,  0.3f,	0.4f,	0.38f,		0.0f,	0.0f,	-1.0f,	//2
		-0.25f,  0.1f,  0.3f,	0.34f,	0.38f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.25f, -1.5f,  0.3f,	0.14f,  0.1f,		-1.0f,	0.0f,	0.0f,
		0.25f, -1.5f,  -0.3f,	0.2f,	0.1f,		-1.0f,	0.0f,	0.0f,
		0.25f,  0.1f,  -0.3f,	0.2f,	0.38f,		-1.0f,	0.0f,	0.0f,
		0.25f,  0.1f,  0.3f,	0.14f,	0.38f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.25f, -1.5f, -0.3f,	0.27f,  0.1f,		0.0f,	0.0f,	1.0f,
		0.25f, -1.5f, -0.3f,	0.2f,	0.1f,		0.0f,	0.0f,	1.0f,
		0.25f,  0.1f, -0.3f,	0.2f,	0.38f,		0.0f,	0.0f,	1.0f,
		-0.25f,  0.1f, -0.3f,	0.27f,	0.38f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.25f, -1.5f,  -0.3f,	0.27f,  0.1f,		1.0f,	0.0f,	0.0f,
		-0.25f, -1.5f,  0.3f,	0.34f,	0.1f,		1.0f,	0.0f,	0.0f,
		-0.25f,  0.1f,  0.3f,	0.34f,	0.38f,		1.0f,	0.0f,	0.0f,
		-0.25f,  0.1f,  -0.3f,	0.27f,	0.38f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.25f, -1.5f,  0.3f,	0.27f,  0.01f,		0.0f,	1.0f,	0.0f,
		0.25f,  -1.5f,  0.3f,	0.21f,	0.01f,		0.0f,	1.0f,	0.0f,
		0.25f,  -1.5f,  -0.3f,	0.21f,	0.1f,		0.0f,	1.0f,	0.0f,
		-0.25f, -1.5f,  -0.3f,	0.27f,	0.1f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.25f, 0.1f,  0.3f,	0.64f,  0.72f,		0.0f,	-1.0f,	0.0f,
		 0.25f,  0.1f,  0.3f,	0.64f,	0.79f,		0.0f,	-1.0f,	0.0f,
		  0.25f, 0.1f,  -0.3f,	0.59f,	0.79f,		0.0f,	-1.0f,	0.0f,
		 -0.25f, 0.1f,  -0.3f,	0.59f,	0.72f,		0.0f,	-1.0f,	0.0f,

	};

	unsigned int cubo_indicesPI[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		4, 5, 6,
		6, 7, 4,
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,
	};

	GLfloat cubo_verticesPI[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.25f, -1.5f,  0.3f,	0.41f,  0.1f,		0.0f,	0.0f,	-1.0f,	//0
		0.25f, -1.5f,  0.3f,	0.47f,	0.1f,		0.0f,	0.0f,	-1.0f,	//1
		0.25f,  0.1f,  0.3f,	0.47f,	0.37f,		0.0f,	0.0f,	-1.0f,	//2
		-0.25f,  0.1f,  0.3f,	0.41f,	0.37f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.25f, -1.5f,  0.3f,	0.47f,  0.1f,		-1.0f,	0.0f,	0.0f,
		0.25f, -1.5f,  -0.3f,	0.53f,	0.1f,		-1.0f,	0.0f,	0.0f,
		0.25f,  0.1f,  -0.3f,	0.53f,	0.37f,		-1.0f,	0.0f,	0.0f,
		0.25f,  0.1f,  0.3f,	0.47f,	0.37f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.25f, -1.5f, -0.3f,	0.6f,  0.1f,		0.0f,	0.0f,	1.0f,
		0.25f, -1.5f, -0.3f,	0.53f,	0.1f,		0.0f,	0.0f,	1.0f,
		0.25f,  0.1f, -0.3f,	0.53f,	0.37f,		0.0f,	0.0f,	1.0f,
		-0.25f,  0.1f, -0.3f,	0.6f,	0.37f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.25f, -1.5f,  -0.3f,	0.53f,  0.1f,		1.0f,	0.0f,	0.0f,
		-0.25f, -1.5f,  0.3f,	0.66f,	0.1f,		1.0f,	0.0f,	0.0f,
		-0.25f,  0.1f,  0.3f,	0.66f,	0.37f,		1.0f,	0.0f,	0.0f,
		-0.25f,  0.1f,  -0.3f,	0.53f,	0.37f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.25f, -1.5f,  0.3f,	0.6f,  0.01f,		0.0f,	1.0f,	0.0f,
		0.25f,  -1.5f,  0.3f,	0.54f,	0.01f,		0.0f,	1.0f,	0.0f,
		0.25f,  -1.5f,  -0.3f,	0.54f,	0.1f,		0.0f,	1.0f,	0.0f,
		-0.25f, -1.5f,  -0.3f,	0.6f,	0.1f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.25f, 0.1f,  0.3f,	0.64f,  0.72f,		0.0f,	-1.0f,	0.0f,
		 0.25f,  0.1f,  0.3f,	0.64f,	0.79f,		0.0f,	-1.0f,	0.0f,
		  0.25f, 0.1f,  -0.3f,	0.59f,	0.79f,		0.0f,	-1.0f,	0.0f,
		 -0.25f, 0.1f,  -0.3f,	0.59f,	0.72f,		0.0f,	-1.0f,	0.0f,

	};

	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(dado);

	Mesh* dadoCuerpo = new Mesh();
	dadoCuerpo->CreateMesh(cubo_verticesCuerpo, cubo_indicesCuerpo, 192, 36);
	meshList.push_back(dadoCuerpo);

	Mesh* dadoBD = new Mesh();
	dadoBD->CreateMesh(cubo_verticesBD, cubo_indicesBD, 192, 36);
	meshList.push_back(dadoBD);

	Mesh* dadoBI = new Mesh();
	dadoBI->CreateMesh(cubo_verticesBI, cubo_indicesBI, 192, 36);
	meshList.push_back(dadoBI);

	Mesh* dadoPD = new Mesh();
	dadoPD->CreateMesh(cubo_verticesPD, cubo_indicesPD, 192, 36);
	meshList.push_back(dadoPD);

	Mesh* dadoPI = new Mesh();
	dadoPI->CreateMesh(cubo_verticesPI, cubo_indicesPI, 192, 36);
	meshList.push_back(dadoPI);
}


int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	CrearDado();


	GLuint uniformTextureOffset = 0;
	uniformTextureOffset = shaderList[0].getoffsetLocation();


	camera = Camera(glm::vec3(-10.0f, 5.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 60.0f, 0.0f, 0.5f, 0.5f);


	//***************Sonido******************************
	ISoundEngine* sonido = createIrrKlangDevice();
	sonido->setSoundVolume(0.30f);

	ISound* bree = sonido->play2D("Sonidos/Breeze.mp3", true, false, true);
	ISound* mar = sonido->play2D("Sonidos/mar.mp3", true, false, true);
	ISound* noche= sonido->play2D("Sonidos/noche.mp3", true, true, true);
	ISound* compu = sonido->play2D("Sonidos/compu.mp3", true, true, true);

	bree->setVolume(0.5);
	mar->setVolume(0.5);
	noche->setVolume(0.5);
	compu->setVolume(1.0);

	// ****** texturas ******
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso_Breze.tga");
	pisoTexture.LoadTextureA();
	pgTexture = Texture("Textures/pisog.tga");
	pgTexture.LoadTextureA();
	escalonTexture = Texture("Textures/escalon.tga");
	escalonTexture.LoadTextureA();
	paredTexture = Texture("Textures/pared.tga");
	paredTexture.LoadTextureA();
	paredATexture = Texture("Textures/paredA.tga");
	paredATexture.LoadTextureA();
	aguaTexture = Texture("Textures/agua.tga");
	aguaTexture.LoadTextureA();
	phoenixTexture = Texture("Textures/Textura_phoenix.tga");
	phoenixTexture.LoadTextureA();

	// ***** modelos *****
	contenedor_M = Model();
	contenedor_M.LoadModel("Models/contenedor.obj");
	caja_M = Model();
	caja_M.LoadModel("Models/caja.obj");
	marco_M = Model();
	marco_M.LoadModel("Models/marco.obj");
	puerta_M = Model();
	puerta_M.LoadModel("Models/puerta.obj");
	torre_M = Model();
	torre_M.LoadModel("Models/torre.obj");
	mesa_M = Model();
	mesa_M.LoadModel("Models/mesa.obj");
	compu_M = Model();
	compu_M.LoadModel("Models/compu.obj");
	humo_M = Model();
	humo_M.LoadModel("Models/humo.obj");
	jett_M = Model();
	jett_M.LoadModel("Models/jett.obj");
	tiro_M = Model();
	tiro_M.LoadModel("Models/tiro.obj");
	gr_M = Model();
	gr_M.LoadModel("Models/gunRoom.obj");
	ak47_M = Model();
	ak47_M.LoadModel("Models/ak47.obj");
	pistol_M = Model();
	pistol_M.LoadModel("Models/pistol.obj");
	rifle_M = Model();
	rifle_M.LoadModel("Models/rifle.obj");
	shotgun_M = Model();
	shotgun_M.LoadModel("Models/shotgun.obj");
	machP_M = Model();
	machP_M.LoadModel("Models/machine_pistol.obj");
	heavyP_M = Model();
	heavyP_M.LoadModel("Models/heavyPistol.obj");
	faro_M = Model();
	faro_M.LoadModel("Models/faro.obj");
	antorcha_M = Model();
	antorcha_M.LoadModel("Models/antorcha.obj");
	tortuga_M = Model();
	tortuga_M.LoadModel("Models/tortuga.obj");


	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/Daylight Box_Right.tga");
	skyboxFaces.push_back("Textures/Skybox/Daylight Box_Left.tga");
	skyboxFaces.push_back("Textures/Skybox/Daylight Box_Bottom.tga");
	skyboxFaces.push_back("Textures/Skybox/Daylight Box_Top.tga");
	skyboxFaces.push_back("Textures/Skybox/Daylight Box_Front.tga");
	skyboxFaces.push_back("Textures/Skybox/Daylight Box_Back.tga");

	std::vector<std::string> skyboxFacesN;
	skyboxFacesN.push_back("Textures/Skybox/skyboxNight_front.tga");
	skyboxFacesN.push_back("Textures/Skybox/skyboxNight_back.tga");
	skyboxFacesN.push_back("Textures/Skybox/skyboxNight_bottom.tga");
	skyboxFacesN.push_back("Textures/Skybox/skyboxNight_top.tga");
	skyboxFacesN.push_back("Textures/Skybox/skyboxNight_right.tga");
	skyboxFacesN.push_back("Textures/Skybox/skyboxNight_left.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);
	Material_phoenix = Material(1.0f, 128);

	//luz direccional, sólo 1 y siempre debe de existir

	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,  // luz de dia
		0.9f, 0.9f,
		0.0f, 0.0f, 0.0f);

	mainLight_night = DirectionalLight(0.35f, 0.35f, 0.35f,  // luz de noche
		0.3f, 0.3f,
		0.0f, 0.0f, 0.0f);

	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f, // luz galeria
		1.0f, 1.0f,
		-45.0f, 2.0f, -10.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	pointLights[1] = PointLight(1.0f, 1.0f, 0.0f,
		0.8f, 0.8f,
		20.0f, 3.0f, 15.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	pointLights[2] = PointLight(1.0f, 0.0f, 0.0f,
		0.7f, 0.7f,
		-4.3f, 4.0f, 0.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;
	
	unsigned int spotLightCount = 0;

	//luz de computadora
	spotLights[0] = SpotLight(0.0f, 0.0f, 1.0f,
		0.1f, 1.0f,
		-30.0f, 1.0, -14.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		10.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(1.0f, 0.0f, 0.0f,
		1.0f, 2.0f,
		18.0f, 9.0f, -17.5f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	//luz del faro
	spotLights[2] = SpotLight(1.0f, 1.0f, 1.0f,
		0.1f, 2.0f,
		0.0f, 10.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		10.0f);
	spotLightCount++;

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);


	//*** puerta ****
	puerta = -1.55;

	// *** agua ***
	cordTexU = 0.0f;
	cordTexV = 0.0f;

	// *** jett ***
	yJett = -1.9f;
	rotJett = 0.0f;
	banderajett = 1.0;

	// **** arma ****
	scaArma = 2.0f;
	rotArma = 0.0f;
	banderahumo = 1.0f;

	// **** arma ****

	rotHumo = 0.0f;
	rotHumo2 = 0.0f;
	scaHumo = 0.0;
	banderahumo = 1.0f;
	movxHumo = -12.0f;
	movzHumo = 15.0f;

	// **** tiro ****
	scaTiro = 1.0f;
	scaTiro2 = 0.0;
	xTiro = -25.0f;
	xTiro2 = -40.0f;
	yTiro = 1.8f;
	yTiro2 = -0.5f;
	angulot = 0.0f;
	banderaTiro = 1.0f;
	banderaTiro2 = 1.0f;
	banderaTiroS = 1.0f;
	banderaTiro2S = 2.0f;

	// *** tortuga ****

	banderaTor = 1.0f;
	movxTor = 13.0f;
	movyTor = 0.0f;
	movzTor = 17.0f;
	anguloTor = 0.0f;


	// **** textura ***
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

	// **** camara ****
	glm::vec3 camara1 = glm::vec3(-10.0f, 5.0f, 0.0f);
	glm::vec3 camaraG = glm::vec3(-25.0f, 3.0f, -0.0f);
	glm::vec3 camara3 = glm::vec3(-10.0, 2.0f, -10.0f);
	yawG = 180.0f;
	yawG = 60.0f;
	pitch1 = 0.0f;

	// *** iluminacion 
	
	dia = 1.0f;
	cont = 1.0f;
	cont2 = 0.0f;
	bandI = 1.0;
	red = 1.0f;
	green = 1.0f;
	blue = 1.0f;

	// **** aniamcion phoenix

	bandA = 1.0f;
	movxP = -10.0f;
	movzP = -10.0f;
	rotP = 0.0f;
	bdP = -30.0f;
	biP = 30.0f;
	pdP = 30.0f;
	piP = -30.0f;
	band1 = 1.0f;
	band2 = 1.0f;

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

	

		glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

		// ***** moviemiento de puerta ****
		if (mainWindow.getapagaP() == 0.0f) {
			if (puerta < 4.0f) {
				puerta += 0.05 * deltaTime;
			}
		}
		else {
			if (puerta > -1.55f) {
				puerta -= 0.05 * deltaTime;
			}
		}

		// ***** muscica computadora ****
		if (mainWindow.getapagaM() == 1.0f) {
			compu->setIsPaused(true);
			bree->setVolume(0.5);
			mar->setVolume(0.5);
			noche->setVolume(0.5);
		}
		else {
			compu->setIsPaused(false);
			bree->setVolume(0.3);
			mar->setVolume(0.3);
			noche->setVolume(0.3);
		}

		// ****** animacion basica jett ******
		if (mainWindow.getapagaA1() == 1.0) {
			if (banderajett == 1.0f) {
				if (yJett < 5.0f) {
					yJett += 0.1 * deltaTime;
					rotJett += 10.0 * deltaTime;
				}
				else {
					banderajett = 0.0f;
				}
			}
			else if (banderajett == 0.0f) {
				if (yJett > -1.9) {
					yJett -= 0.1 * deltaTime;
					rotJett += 10.0 * deltaTime;
				}
				else {

				}
			}
		}
		else {
			banderajett = 1.0;

		}
		// **** animacion basica arma******

		if (banderaArma == 1.0f) {
			if (scaArma > 0.0f) {
				scaArma -= 0.004f;
			}
			else
			{
				banderaArma = 0.0f;
			}
		}
		else if (banderaArma == 0.0f) {
			if (scaArma < 0.8f) {
				scaArma += 0.004f;
			}
			else
			{
				banderaArma = 1.0f;
			}

		}
		
		if (rotArma < 360.0f) {
			rotArma += 3.0 * deltaTime;
		}
		else {
			rotArma = 0.0f;
		}

		// **** animacion compeja humo ****

		if (banderahumo == 1.0f) {
			if (scaHumo < 4.0f) {
				scaHumo += 0.01f * deltaTime;
			}
			else {
				banderahumo = 2.0f;
			}

		}else if (banderahumo == 2.0f){
			if (movzHumo > -15.0f) {
				movzHumo -= 0.05f * deltaTime;
			}
			else {
				if (rotHumo2 < 45.0f) {
					rotHumo2 += 1.0f;
					movxHumo += (1.0f * cos(rotHumo2 * toRadians)) * 0.025f * deltaTime;
					movzHumo -= (1.0f * sin(rotHumo2 * toRadians)) * 0.025f * deltaTime;
				}
				else {
					banderahumo = 3.0f;
				}
			}
		}else if (banderahumo == 3.0f) {
			if (movxHumo < 0.0f) {
				movxHumo += 0.05f * deltaTime;
			}
			else {
				banderahumo = 4.0f;
			}
		}else if (banderahumo == 4.0f) {
			if (scaHumo > 0.0f){
				scaHumo -= 0.01f * deltaTime;

			}else{
				movxHumo = -12.0f;
				movzHumo = 15.0f;
				banderahumo = 1.0f;
			}
		}

		if (rotHumo < 360.0f) {
			rotHumo += 3.0 * deltaTime;
		}
		else {
			rotHumo = 0.0f;
		}
		// **** animacion compleja tiro *******

		if (banderaTiro == 1.0f) {
			if (xTiro > -40.0f) {
				xTiro -= 0.03 * deltaTime;
				yTiro = 2.0f + (sin( (0.5f + angulot) * toRadians) * deltaTime);
			}else {
				banderaTiro = 2.0f;
			}
		} else if (banderaTiro == 2.0f) {
			if (xTiro < -25.0f) {
				xTiro += 0.03 * deltaTime;
				yTiro = 2.0f + (sin((0.5f + angulot) * toRadians) * deltaTime);
			}
			else {
				banderaTiro = 1.0f;
			}
		}

		if (banderaTiro2 == 1.0f) {
			if (xTiro2 < -25.0f) {
				xTiro2 += 0.03 * deltaTime;
				yTiro2 = 0.0f + (sin((0.5f + angulot) * toRadians) * deltaTime);
			}
			else {
				banderaTiro2 = 2.0f;
			}
			
		}
		else if (banderaTiro2 == 2.0f) {
			if (xTiro2 > -40.0f) {
				xTiro2 -= 0.03 * deltaTime;
				yTiro2 = 0.0f + (sin((0.5f + angulot) * toRadians) * deltaTime);
			}
			else {
				banderaTiro2 = 1.0f;
			}
		}

		if (angulot < 360.0f) {
			angulot += 2.0 * deltaTime;
		}else {
			angulot = 0.0f;
		}
		
		if (banderaTiroS == 1.0f) {
			if (scaTiro > 0.0f) {
				scaTiro -= 0.005 * deltaTime;
			}
			else {
				banderaTiroS = 2.0f;
			}
		}else if (banderaTiroS == 2.0f) {
			if (scaTiro < 1.0f) {
				scaTiro += 0.005 * deltaTime;
			}
			else {
				banderaTiroS = 1.0f;
			}
		}
		
		if (banderaTiro2S == 1.0f) {
			if (scaTiro2 > 0.0f) {
				scaTiro2 -= 0.005 * deltaTime;
			}
			else {
				banderaTiro2S = 2.0f;
			}
		}
		else if (banderaTiro2S == 2.0f) {
			if (scaTiro2 < 1.0f) {
				scaTiro2 += 0.005 * deltaTime;
			}
			else {
				banderaTiro2S = 1.0f;
			}
		}

		// ******** animacion tortuga *******

		if (banderaTor == 1.0f) {
			if (anguloTor > -1080.f) {
				anguloTor -= 1.0f;
				movxTor += (0.8f * cos(anguloTor * toRadians)) * 0.025 * deltaTime;
				movzTor += (0.8f * sin(anguloTor * toRadians)) * 0.025 * deltaTime;
				movyTor += 0.008 * deltaTime;
			}
			else {
				banderaTor = 2.0f;
			}
		}else if (banderaTor == 2.0f) {
			if (anguloTor > -2160.0f) {
				anguloTor -= 1.0f;
				movxTor += (0.8f * cos(anguloTor * toRadians)) * 0.025 * deltaTime;
				movzTor += (0.8f * sin(anguloTor * toRadians)) * 0.025 * deltaTime;
				movyTor -= 0.008 * deltaTime;
			}
			else {
				anguloTor = 0.0f;
				banderaTor = 1.0f;
			}
		}
		

		// **** animacion phoenix ****

		if (bandA == 1.0f){
			if (movzP < 7.0f) {
				movzP += 0.03 * deltaTime;
			}
			else {
				if (rotP < 90.0f) {
					rotP += 0.5f * deltaTime;
					movxP += (1.0f * cos(rotP * toRadians)) * 0.025 *deltaTime;
					movzP += (1.0f * sin(rotP * toRadians)) * 0.025 *deltaTime;
				}
				else {
					bandA = 2.0f;
				}
			}
		}
		else if (bandA == 2.0f) {
			if (movxP < 8.0f) {
				movxP += 0.03 * deltaTime;
			}
			else {
				if (rotP < 180.0f) {
					rotP += 0.5f * deltaTime;
					movxP -= (1.0f * cos(rotP * toRadians)) * 0.025 * deltaTime;
					movzP -= (1.0f * sin(rotP * toRadians)) * 0.025 * deltaTime;
				}
				else {
					bandA = 3.0f;
				}
			}
		}
		else if (bandA == 3.0f) {
			if (movzP > -7.5f) {
				movzP -= 0.03 * deltaTime;
			}
			else {
				if (rotP < 270.0f) {
					rotP += 0.5f * deltaTime;
					movxP += (1.0f * cos(rotP * toRadians)) * 0.025 * deltaTime;
					movzP += (1.0f * sin(rotP * toRadians)) * 0.025 * deltaTime;
				}
				else {
					bandA = 4.0f;
				}
			}
		}
		else if (bandA == 4.0f) {
			if (movxP > -7.5f) {
				movxP -= 0.03 * deltaTime;
			}
			else {
				if (rotP < 360.0f) {
					rotP += 0.5f * deltaTime;
					movxP -= (1.0f * cos(rotP * toRadians)) * 0.025 * deltaTime;
					movzP -= (1.0f * sin(rotP * toRadians)) * 0.025 * deltaTime;
				}
				else {
					rotP = 0.0;
					bandA = 1.0f;
				}
			}
		}
		//****** mover brazos y piernas de phoenix *****
		if (band1 == 1.0f) {
			if (bdP < 30.0f) {
				bdP += 1.0 * deltaTime;
				piP += 1.0 * deltaTime;
			}
			else {
				band1 = 0.0f;
			}
		}
		else if(band1 == 0.0f){
			if (bdP > -30.0f) {
				bdP -= 1.0 * deltaTime;
				piP -= 1.0 * deltaTime;
			}
			else {
				band1 = 1.0f;
			}
		}

		if (band2 == 1.0f) {
			if (biP > -30.0f) {
				biP -= 1.0 * deltaTime;
				pdP -= 1.0 * deltaTime;
			}
			else {
				band2 = 0.0f;
			}
		}
		else if (band2 == 0.0f) {
			if (biP < 30.0f) {
				biP += 1.0 * deltaTime;
				pdP += 1.0 * deltaTime;
			}
			else {
				band2 = 1.0f;
			}
		}

		//Recibir eventos del usuario
		glfwPollEvents();

		// ***************** camaras **********
	
		if (mainWindow.getapagaC() == 1.0f) { // camara xyz
			camera = Camera(camara1, glm::vec3(0.0f, 1.0f, 0.0f), yaw1, pitch1, 0.5f, 0.5f);
			camera.keyControl(mainWindow.getsKeys(), deltaTime);
			camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
			camara1 = camera.getCameraPosition();
			yaw1 = camera.getCameraYaw();
			pitch1 = camera.getCameraPitch();
		}
		else if (mainWindow.getapagaC() == 2.0f) { //camra de galeria
			camera = Camera(camaraG, glm::vec3(0.0f, 1.0f, 0.0f), yawG, 0.0f, 0.5f, 0.5f);
			camera.mouseControl(mainWindow.getXChange(), 0);
			yawG = camera.getCameraYaw();

		}
		else if (mainWindow.getapagaC() == 3.0f) { // camara en 3 persona
			camera = Camera(camara3, glm::vec3(0.0f, 1.0f, 0.0f), yaw3,pitch3, 0.5f, 0.5f);
			camera.keyControl(mainWindow.getsKeys(), deltaTime);
			camera.mouseControl(mainWindow.getXChange(), 0);
			camara3 = camera.getCameraPosition();
			yaw3 = camera.getCameraYaw();
			pitch3 = camera.getCameraPitch();
		}

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// variables para show de luces
		red = rand() % 256 / 255.0f;
		green = rand() % 256 / 255.0f;
		blue = rand() % 256 / 255.0f;

		//información al shader de fuentes de iluminación

		if (dia == 1.0f) { // dia
			if (now < (30.0f * cont)) {
				if (bandI == 1.0f) {
					bree->setIsPaused(false);
					noche->setIsPaused(true);
					skybox = Skybox(skyboxFaces);
					shaderList[0].SetDirectionalLight(&mainLight);
					spotLightCount = 1.0f;
					bandI = 0.0f;
				}
			}
			else {
				cont2 = cont2 + 2.0f;
				dia = 0.0f;
				bandI = 1.0f;
			}
		}
		else if (dia == 0.0f){ // noche
			if (now < (30.0f * cont2)) { 
				if (bandI == 1.0f) {
					bree->setIsPaused(true);
					noche->setIsPaused(false);
					skybox = Skybox(skyboxFacesN);
					shaderList[0].SetDirectionalLight(&mainLight_night);
					spotLightCount = 3.0f;
					bandI = 0.0f;
				}
				
				spotLights[1] = SpotLight(red, green, blue,
					1.0f, 2.0f,
					18.0f, 9.0f, -17.5f,
					0.0f, -1.0f, 0.0f,
					1.0f, 0.0f, 0.0f,
					15.0f);

				pointLights[2] = PointLight(red, green, blue,
					0.7f, 0.7f,
					-4.3f, 4.0f, 0.0f,
					0.3f, 0.2f, 0.1f);
				
			}
			else {
				cont = cont + 2.0f;
				dia = 1.0f;
				bandI = 1.0f;
			}
		}

		spotLights[0] = SpotLight(red, green, blue,
			1.0f, 1.3f,
			-30.0f, 1.0, -13.0f,
			0.0f, 0.0f, 1.0f,
			0.5f, 0.5f, 0.0f,
			20.0f);

		// *** apagar luces point
		if (mainWindow.getapagal() == 1.0f) {
			shaderList[0].SetPointLights(pointLights, pointLightCount);
		}
		else {
			shaderList[0].SetPointLights(pointLights, 0);
		}

		shaderList[0].SetSpotLights(spotLights, spotLightCount);
		
		// ***** declaracion de modelos ****
		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		

		// *** piso ***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.0f, 1.5f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		// ***  pared front ***
		model = glm::mat4(1.0);
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(4.0f, -22.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 1.0f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		// ***  pared  back D ***
		model = glm::mat4(1.0);
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-4.0f, -22.5f, -12.1f));
		model = glm::scale(model, glm::vec3(0.4f, 1.0f, 0.695f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		// ***  pared back I ***
		model = glm::mat4(1.0);
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-4.0f, -22.5f, 12.6f));
		model = glm::scale(model, glm::vec3(0.4f, 1.0f, 0.66f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		// ***  pared derecha ***
		model = glm::mat4(1.0);
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -22.5f, 4.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.0f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		// ***  pared izquierda ***
		model = glm::mat4(1.0);
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -22.5f, -4.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.0f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();
		
		// ***  piso escalon ***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.7f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 1.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		escalonTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		// ***   escalon b ***
		model = glm::mat4(1.0);
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-1.85f, 7.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.01f, 1.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		escalonTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		// ***   escalon f ***
		model = glm::mat4(1.0);
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(1.85f, 7.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.01f, 1.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		escalonTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		// ***   escalon d ***
		model = glm::mat4(1.0);
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 7.5f, 1.85f));
		model = glm::scale(model, glm::vec3(0.5f, 1.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		escalonTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		// ***   escalon i ***
		model = glm::mat4(1.0);
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 7.5f, -1.85f));
		model = glm::scale(model, glm::vec3(0.5f, 1.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		escalonTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		// *** marco de puerta  ***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-23.9f, -1.55f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		marco_M.RenderModel();

		// *** puerta ***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-24.2f, puerta, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		puerta_M.RenderModel();
		
		// *** piso galeria***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-37.5f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.0f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pgTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();

		// *** techo galeria***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-37.5f, 4.50f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.0f, 1.5f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pgTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();

		// *** pared back galeria***
		model = glm::mat4(1.0);
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-1.25f, -52.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.325f, 1.0f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pgTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();

		// *** pared derecha galeria***
		model = glm::mat4(1.0);
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-37.5f, -15.0f, 1.25f));
		model = glm::scale(model, glm::vec3(1.5f, 1.0f, 0.325f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		
		// *** pared izquierda galeria***
		model = glm::mat4(1.0);
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-37.5f, -15.0f, -1.25f));
		model = glm::scale(model, glm::vec3(1.5f, 1.0f, 0.325f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();


		// *** torre ***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.9f, 0.0f));
		model = glm::scale(model, glm::vec3(6.5f, 4.5f, 6.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		torre_M.RenderModel();

		// *** contenedor ***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.9f, 20.5f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		contenedor_M.RenderModel();
		
		// *** caja 1***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(6.0f, -1.7f, 6.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		caja_M.RenderModel();

		// *** caja 2***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, -2.0f, -15.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		caja_M.RenderModel();

		// *** caja 3 ***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, -2.0f, -12.9f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f,1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		caja_M.RenderModel();

		// *** caja 4 ***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-12.9f, -2.0f, -15.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		caja_M.RenderModel();

		// *** caja 5 ***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, 0.1f, -15.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		caja_M.RenderModel();

		// *** caja 5 ***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-20.0f, -2.0f, 15.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::rotate(model, -45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		caja_M.RenderModel();

		// *** caja 6 ***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-38.0f, -2.0f, -10.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::rotate(model, -225 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		caja_M.RenderModel();

		// *** caja 7***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-42.0f, -2.0f, -10.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::rotate(model, -45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		caja_M.RenderModel();

		// *** caja 8***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-49.0f, -2.0f, -10.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::rotate(model, -135 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		caja_M.RenderModel();

		// *** antorcha 1***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-45.0f, -1.99f, -10.0f));
		model = glm::scale(model, glm::vec3(3.0f, 4.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		antorcha_M.RenderModel();

		// *** antorcha 2***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(22.1f, 3.0f, 15.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.50f));
		model = glm::rotate(model, 30 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		antorcha_M.RenderModel();

		// *** antorcha 3***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-3.3f, 4.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.50f));
		model = glm::rotate(model, 30 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		antorcha_M.RenderModel();

		// *** mesa ***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, -1.9f, -11.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.7f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		mesa_M.RenderModel();

		// *** compu ***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, 0.0f, -11.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		compu_M.RenderModel();

		// *** humo ***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movxHumo, 0.0f, movzHumo));
		model = glm::scale(model, glm::vec3(scaHumo, scaHumo, scaHumo));
		model = glm::rotate(model, rotHumo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotHumo2 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		humo_M.RenderModel();

		// *** jett ***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, yJett, 13.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, rotJett * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		jett_M.RenderModel();


		// *** cuerpo phoenix***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movxP, 0.4f, movzP));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, rotP * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_phoenix.UseMaterial(uniformSpecularIntensity, uniformShininess);
		phoenixTexture.UseTexture();
		meshList[6]->RenderMesh();

		// *** cabeza phoenix***
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.3, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_phoenix.UseMaterial(uniformSpecularIntensity, uniformShininess);
		phoenixTexture.UseTexture();
		meshList[5]->RenderMesh();

		// *** brazoD phoenix***
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.7f, 0.6, 0.0f));
		model = glm::rotate(model, bdP * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_phoenix.UseMaterial(uniformSpecularIntensity, uniformShininess);
		phoenixTexture.UseTexture();
		meshList[7]->RenderMesh();

		// *** brazoI phoenix***
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.7f, 0.6, 0.0f));
		model = glm::rotate(model, biP * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_phoenix.UseMaterial(uniformSpecularIntensity, uniformShininess);
		phoenixTexture.UseTexture();
		meshList[8]->RenderMesh();

		// *** piernaD phoenix***
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.25f, -0.9, 0.0f));
		model = glm::rotate(model, pdP * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_phoenix.UseMaterial(uniformSpecularIntensity, uniformShininess);
		phoenixTexture.UseTexture();
		meshList[9]->RenderMesh();

		// *** piernaI phoenix***
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.25f, -0.9, 0.0f));
		model = glm::rotate(model, piP * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_phoenix.UseMaterial(uniformSpecularIntensity, uniformShininess);
		phoenixTexture.UseTexture();
		meshList[10]->RenderMesh();
		
		// *** tiro 1 ***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(xTiro, yTiro, 14.5f));
		model = glm::scale(model, glm::vec3(scaTiro, scaTiro, scaTiro));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		tiro_M.RenderModel();

		// *** tiro 2 ***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(xTiro2, yTiro2, 14.5f));
		model = glm::scale(model, glm::vec3(scaTiro2, scaTiro2, scaTiro2));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		tiro_M.RenderModel();

		// *** ak47 animacion ***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-40.3f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(scaArma, scaArma, scaArma));
		model = glm::rotate(model, rotArma * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		ak47_M.RenderModel();


		// *** cuarto de armas ***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-49.3f, -1.9f, 11.9f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		gr_M.RenderModel();

		// *** ak47 ***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-52.3f, 2.5f, 5.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		ak47_M.RenderModel();

		// *** ak47 2***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-52.3f, 2.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		ak47_M.RenderModel();

		// *** ak47 3***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-52.3f, 2.5f, -5.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		ak47_M.RenderModel();

		// ***ak47 4 * **
			model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-52.3f, 2.5f, -10.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		ak47_M.RenderModel();

		// *** rifle ***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-52.3f, 1.5f, 2.5f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		rifle_M.RenderModel();

		// *** rifle 2***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-52.3f, 1.5f, -2.5f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		rifle_M.RenderModel();

		// *** rifle 3***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-52.3f, 1.5f, -7.5f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		rifle_M.RenderModel();

		// *** shotgun ***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-52.3f, 0.5f, 5.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shotgun_M.RenderModel();

		// *** shotgun 2***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-52.3f, 0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shotgun_M.RenderModel();

		// *** shotgun 3***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-52.3f, 0.5f, -5.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shotgun_M.RenderModel();

		// *** machine pistola***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-52.3f, 0.0f, 2.5f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		machP_M.RenderModel();

		// *** machine pistola 2***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-52.3f, 0.0f, -2.5f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		machP_M.RenderModel();

		// *** machine pistola 3***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-52.3f, 0.0f, -7.5f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		machP_M.RenderModel();

		// *** heavy pistola***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-52.3f, -0.5f, 5.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		heavyP_M.RenderModel();

		// *** heavy pistola 2***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-52.3f, -0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		heavyP_M.RenderModel();

		// *** heavy pistola 3***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-52.3f, -0.5f, -5.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		heavyP_M.RenderModel();

		// *** pistol ***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-52.3f, -1.0f, 2.5f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		pistol_M.RenderModel();

		// *** pistol 2***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-52.3f, -1.0f, -2.5f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		pistol_M.RenderModel();

		// *** pistol 3***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-52.3f, -1.0f, -7.5f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		pistol_M.RenderModel();

		// *** faro ***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, -1.9f, -19.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		faro_M.RenderModel();

		// *** tortuga ***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movxTor, -1.9f + movyTor ,movzTor));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, anguloTor * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		tortuga_M.RenderModel();

		// *** pared interna 1 ***
		model = glm::mat4(1.0);
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(3.0f, -16.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.7f, 1.0f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		paredTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		// *** pared interna 2 ***
		model = glm::mat4(1.0);
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(10.0f, -13.5f, -1.0f));
		model = glm::scale(model, glm::vec3(0.4f, 1.0f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		paredTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();


		cordTexU += 0.005f;
		cordTexV += 0.005f;

		if (cordTexU > 1.0f) {
			cordTexU = 0.0f;
			cordTexV = 0.0f;
		}

		toffset = glm::vec2(cordTexU, cordTexV);
		// *** agua ***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.2f, 0.0f));
		model = glm::scale(model, glm::vec3(100.0f, 1.0f, 100.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		aguaTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();
		glDisable(GL_BLEND);


		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
