#include <glad/glad.h>
#include <GLFW/glfw3.h>         // GLFW library
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//#include <GL/glew.h>          // GLEW library

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "ShapeData.h"
#include "ShapeGenerator.h"
#include "shader.h"
#include "camera.h"
#include "cylinder.h"


#include <iostream>
#include <tube.h>

//Matthew Trembley - Module 4 Milestone

using namespace std; // Uses the standard namespace

//Unnamed namespace
namespace {


	// Variables for window width, height, and title
	const int WINDOW_WIDTH = 800;
	const int WINDOW_HEIGHT = 600;
	const char* const WINDOW_TITLE = "Trembley Final Project";

	// Stores the GL data to a given mesh

	struct GLMesh {
		GLuint vao;			// Handle for vertex array object
		GLuint vbos[2];     // Handles for the vertex buffer objects
		GLuint nIndices;    // Number of indices of the mesh
		GLuint nVertices;
	};

	// Main GLFW window
	GLFWwindow* gWindow = nullptr;
	//Printer mesh data
	GLMesh gMesh;
	//Shader Program
	GLuint gProgramId;
	//Texture
	GLuint gTextureId;

	//Variables for Camera
	Camera gCamera(glm::vec3(-1.75f, 3.0f, 5.0f));
	float gLastX = WINDOW_WIDTH / 2.0f;
	float gLastY = WINDOW_HEIGHT / 2.0F;
	bool gFirstMouse = true;
	float fov = 45.0f;
	float cameraSpeed = 2.5f;
	bool perspective = true;

	//timing
	float gDeltaTime = 0.0f; // Time between current frame and last frame
	float gLastFrame = 0.0f;

	// Plane data
	const uint NUM_VERTICES_PER_TRI = 3;
	const uint NUM_FLOATS_PER_VERTICE = 9;
	const uint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);
	GLuint planeNumIndices;
	GLuint planeNumIndices2;
	GLuint planeVAO;
	GLuint planeIndexByteOffset;
	GLuint planeIndexByteOffset2;

	// sphere
	GLuint sphereNumIndices;
	GLuint sphereVertexArrayObjectID;
	GLuint sphereIndexByteOffset;

	GLuint sphereNumIndices2;
	GLuint sphereVertexArrayObjectID2;
	GLuint sphereIndexByteOffset2;

	//etc.
	float angle;
	static glm::vec2 gUVScale(5.0f, 5.0f);

}

/*User-defined functions to:
*initialize the program, set window size
*redraw graphics on window resizing,
*and render graphics on the screen */

void UResizeWindow(GLFWwindow* window, int width, int height);
void UProcessInput(GLFWwindow* window);
void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos);
void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void UDestroyMesh(GLMesh& mesh);
void destroyTexture(GLuint gTextureId);
unsigned int loadTexture(char const* path);
glm::mat4 projection = glm::perspective(glm::radians(fov), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.01f, 100.0f);


//**************main function**************
int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Final Project - Trembley", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, UResizeWindow);
	glfwSetCursorPosCallback(window, UMousePositionCallback);
	glfwSetScrollCallback(window, UMouseScrollCallback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// build and compile our shader zprogram
	// ------------------------------------

	Shader lightingShader("shaderfiles/6.multiple_lights.vs", "shaderfiles/6.multiple_lights.fs");
	//Shader lightCubeShader("shaderfiles/6.light_cube.vs", "shaderfiles/6.light_cube.fs");


	// positions of the point lights
	glm::vec3 pointLightPositions[] = {
		glm::vec3(-4.0f,  3.5f,  2.0f),
		glm::vec3(-3.75f, 2.0f,  2.0f),
		glm::vec3(-2.0f,  2.5f, 1.0f),
		glm::vec3(-1.0f,  3.0f, 1.0f)
	};
/*
	//cube's VAO and VBO
	unsigned int cubeVBO, cubeVAO;

	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);

	glBindVertexArray(cubeVAO);
	// position attribute for cube
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture attribute for cube
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// normal attribute for cube
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// position attribute for cube
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture attribute for cube
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// normal attribute for cube
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// lightCube's VAO and VBO
	unsigned int lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	*/

	// creates plane object------------------------------------------------------------------------------
	ShapeData plane = ShapeGenerator::makePlane(10);

	unsigned int planeVBO{}, planeVAO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);

	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, plane.vertexBufferSize() + plane.indexBufferSize(), 0, GL_STATIC_DRAW);

	GLsizeiptr currentOffset = 0;

	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, plane.vertexBufferSize(), plane.vertices);
	currentOffset += plane.vertexBufferSize();

	planeIndexByteOffset = currentOffset;

	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, plane.indexBufferSize(), plane.indices);

	planeNumIndices = plane.numIndices;

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sizeof(float) * 2));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeVBO);

	//Plane for leaves-------------------------------------------------------------------------------
	ShapeData plane2 = ShapeGenerator::makePlane(2);

	unsigned int planeVBO2{}, plantLeafVAO;
	glGenVertexArrays(1, &plantLeafVAO);
	glGenBuffers(1, &planeVBO2);

	glBindVertexArray(plantLeafVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO2);
	glBufferData(GL_ARRAY_BUFFER, plane2.vertexBufferSize() + plane2.indexBufferSize(), 0, GL_STATIC_DRAW);

	GLsizeiptr currentOffset2 = 0;

	glBufferSubData(GL_ARRAY_BUFFER, currentOffset2, plane2.vertexBufferSize(), plane2.vertices);
	currentOffset2 += plane2.vertexBufferSize();

	planeIndexByteOffset2 = currentOffset2;

	glBufferSubData(GL_ARRAY_BUFFER, currentOffset2, plane2.indexBufferSize(), plane2.indices);

	planeNumIndices2 = plane2.numIndices;

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sizeof(float) * 2));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeVBO2);

	// creates sphere object-------------------------------------------------------------------------
	ShapeData sphere = ShapeGenerator::makeSphere();

	unsigned int sphereVBO{}, sphereVAO;
	glGenVertexArrays(1, &sphereVAO);
	glGenBuffers(1, &sphereVBO);
	glBindVertexArray(sphereVAO);
	glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
	glBufferData(GL_ARRAY_BUFFER, sphere.vertexBufferSize() + sphere.indexBufferSize(), 0, GL_STATIC_DRAW);
	currentOffset = 0;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, sphere.vertexBufferSize(), sphere.vertices);
	currentOffset += sphere.vertexBufferSize();
	sphereIndexByteOffset = currentOffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, sphere.indexBufferSize(), sphere.indices);
	sphereNumIndices = sphere.numIndices;
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sizeof(float) * 6));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereVBO);

	//create Cylinder for cup------------------------------------------------------------------
	float cupVert[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	unsigned int cupVBO, cupVAO;
	unsigned int cupVBO2, cupVAO2;
	glGenVertexArrays(1, &cupVAO);
	glGenBuffers(1, &cupVBO);
	glBindVertexArray(cupVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cupVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cupVert), cupVert, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glGenVertexArrays(1, &cupVAO2);
	glGenBuffers(1, &cupVBO2);
	glBindVertexArray(cupVAO2);
	glBindBuffer(GL_ARRAY_BUFFER, cupVBO2);

	//create 2nd sphere for cup shape--------------------------------------------------------------
	ShapeData sphere2 = ShapeGenerator::makeSphere();
	unsigned int sphereVBO2{}, sphereVAO2;
	glGenVertexArrays(1, &sphereVAO2);
	glGenBuffers(1, &sphereVBO2);
	glBindVertexArray(sphereVAO2);
	glBindBuffer(GL_ARRAY_BUFFER, sphereVBO2);
	glBufferData(GL_ARRAY_BUFFER, sphere2.vertexBufferSize() + sphere2.indexBufferSize(), 0, GL_STATIC_DRAW);
	currentOffset = 0;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, sphere2.vertexBufferSize(), sphere2.vertices);
	currentOffset += sphere2.vertexBufferSize();
	sphereIndexByteOffset2 = currentOffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, sphere2.indexBufferSize(), sphere2.indices);
	sphereNumIndices2 = sphere2.numIndices;
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sizeof(float) * 6));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereVBO2);



	//cup handle made of a cylinder---------------------------------------------------------------------
	float cupVert2[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	unsigned int cupVBO3, cupVAO3;
	//unsigned int cupVBO4, cupVAO4;
	glGenVertexArrays(1, &cupVAO3);
	glGenBuffers(1, &cupVBO3);
	glBindVertexArray(cupVAO3);
	glBindBuffer(GL_ARRAY_BUFFER, cupVBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cupVert2), cupVert2, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Faux plant 
	float cupVert3[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 0.0f
	};

	unsigned int cupVBO4, cupVAO4;
	unsigned int cupVBO5, cupVAO5;
	glGenVertexArrays(1, &cupVAO4);
	glGenBuffers(1, &cupVBO4);
	glBindVertexArray(cupVAO4);
	glBindBuffer(GL_ARRAY_BUFFER, cupVBO4);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cupVert3), cupVert3, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glGenVertexArrays(1, &cupVAO5);
	glGenBuffers(1, &cupVBO5);
	glBindVertexArray(cupVAO5);
	glBindBuffer(GL_ARRAY_BUFFER, cupVBO5);

	float cupVert4[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 0.0f
	};

	unsigned int cupVBO6, cupVAO6;
	unsigned int cupVBO7, cupVAO7;
	glGenVertexArrays(1, &cupVAO6);
	glGenBuffers(1, &cupVBO6);
	glBindVertexArray(cupVAO6);
	glBindBuffer(GL_ARRAY_BUFFER, cupVBO6);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cupVert4), cupVert4, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glGenVertexArrays(1, &cupVAO7);
	glGenBuffers(1, &cupVBO7);
	glBindVertexArray(cupVAO7);
	glBindBuffer(GL_ARRAY_BUFFER, cupVBO7);

	//Printer  items ----------------------------------------
	GLfloat verts[] = {
		//Positions          //Normals
		// ------------------------------------------------------
		//Back Face          //Negative Z Normal  Texture Coords.
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	   //Front Face         //Positive Z Normal
	  -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
	   0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
	   0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
	   0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
	  -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
	  -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

	  //Left Face          //Negative X Normal
	 -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	 -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	 -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	 -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	 //Right Face         //Positive X Normal
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	 //Bottom Face        //Negative Y Normal
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	//Top Face           //Positive Y Normal
   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	unsigned int vao, vbo;
	unsigned int vao2, vbo2;
	unsigned int vao3, vbo3;
	unsigned int vao4, vbo4;
	unsigned int vao5, vbo5;
	const GLuint floatsPerVertex = 3;
	const GLuint floatsPerNormal = 3;
	const GLuint floatsPerUV = 2;

	unsigned int nVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));

	glGenVertexArrays(1, &vao); // we can also generate multiple VAOs or buffers at the same time
	glBindVertexArray(vao);

	// Create 2 buffers: first one for the vertex data; second one for the indices
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // Activates the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

	// Strides between vertex coordinates is 6 (x, y, z, r, g, b, a). A tightly packed stride is 0.
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);// The number of floats before each

	// Create Vertex Attribute Pointers
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float)* floatsPerVertex));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float)* (floatsPerVertex + floatsPerNormal)));
	glEnableVertexAttribArray(2);

	
	// load textures using utility function
	unsigned int testTex = loadTexture("images/brick.jpg");
	unsigned int planeDiffuseMap = loadTexture("images/woodgrain2.jpg");
	unsigned int cupDiffuseMap = loadTexture("images/cupbottom2.png");
	unsigned int printerFace = loadTexture("images/epson.jpg");
	unsigned int printerTop = loadTexture("images/printer top.jpg");
	unsigned int printerRight = loadTexture("images/printersideR.jpg");
	unsigned int printerLeft = loadTexture("images/printersideL.jpg");
	unsigned int printerTray = loadTexture("images/printer tray.jpg");
	unsigned int cupFace = loadTexture("images/chipFace.jpg");
	unsigned int cupFaceCYL = loadTexture("images/chipFaceCYL.jpg");
	unsigned int cupHandle = loadTexture("images/chipHandle.jpg");
	unsigned int plantBase = loadTexture("images/plant1.jpg");
	unsigned int plantBranch = loadTexture("images/plant2.jpg");
	unsigned int plantLeaf = loadTexture("images/plantLeaf2.png");
	unsigned int plantDirt = loadTexture("images/dirt.png");
	unsigned int wall = loadTexture("images/wall.jpg");


	
	

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	lightingShader.use();
	lightingShader.setInt("material.diffuse", 0);
	lightingShader.setInt("material.specular", 1);
	lightingShader.setInt("cupFaceCYL", 2);


	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		gDeltaTime = currentFrame - gLastFrame;
		gLastFrame = currentFrame;

		// input
		// -----
		UProcessInput(window);

		// render
		// ------
		glClearColor(0.0f, 0.0f, 0.f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		lightingShader.use();
		lightingShader.setVec3("viewPos", gCamera.Position);
		lightingShader.setFloat("material.shininess", 32.0f);

		// directional light
		lightingShader.setVec3("dirLight.direction", -1.0f, 3.0f, -0.1f);
		lightingShader.setVec3("dirLight.ambient", 0.0f, 0.0f, 0.02f);
		lightingShader.setVec3("dirLight.diffuse", 0.3f, 0.3f, .3f);
		lightingShader.setVec3("dirLight.specular", 0.0f, 0.0f, 0.0f);
		// key light
		lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
		lightingShader.setVec3("pointLights[0].ambient", 0.0f, 0.0f, 0.2f);
		lightingShader.setVec3("pointLights[0].diffuse", 0.5f, 0.5f, 0.5f);
		lightingShader.setVec3("pointLights[0].specular", 0.0f, 0.0f, 0.0f);
		lightingShader.setFloat("pointLights[0].constant", 1.0f);
		lightingShader.setFloat("pointLights[0].linear", 0.09);
		lightingShader.setFloat("pointLights[0].quadratic", 0.032);
		// fill light
		lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
		lightingShader.setVec3("pointLights[1].ambient", 0.7f, 0.7f, 0.7f);
		lightingShader.setVec3("pointLights[1].diffuse", 0.1f, 0.1f, 0.1f);
		lightingShader.setVec3("pointLights[1].specular", 0.0f, 0.0f, 0.2f);
		lightingShader.setFloat("pointLights[1].constant", 1.0f);
		lightingShader.setFloat("pointLights[1].linear", 0.09);
		lightingShader.setFloat("pointLights[1].quadratic", 0.032);
		// point light 3
		lightingShader.setVec3("pointLights[2].position", pointLightPositions[2]);
		lightingShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.setVec3("pointLights[2].diffuse", 0.3f, 0.3f, 0.3f);
		lightingShader.setVec3("pointLights[2].specular", 0.0f, 0.0f, 0.0f);
		lightingShader.setFloat("pointLights[2].constant", 1.0f);
		lightingShader.setFloat("pointLights[2].linear", 0.09);
		lightingShader.setFloat("pointLights[2].quadratic", 0.032);
		// point light 4
		lightingShader.setVec3("pointLights[3].position", pointLightPositions[3]);
		lightingShader.setVec3("pointLights[3].ambient", 0.09f, 0.09f, 0.09f);
		lightingShader.setVec3("pointLights[3].diffuse", 0.3f, 0.3f, 0.3f);
		lightingShader.setVec3("pointLights[3].specular", 0.1f, 0.1f, 0.1f);
		lightingShader.setFloat("pointLights[3].constant", 1.0f);
		lightingShader.setFloat("pointLights[3].linear", 0.09);
		lightingShader.setFloat("pointLights[3].quadratic", 0.032);
		// spotLight
		lightingShader.setVec3("spotLight.position", gCamera.Position);
		lightingShader.setVec3("spotLight.direction", gCamera.Front);
		lightingShader.setVec3("spotLight.ambient", 0.1f, 0.1f, 0.1f);
		lightingShader.setVec3("spotLight.diffuse", 0.7f, 0.7f, 0.7f);
		lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("spotLight.constant", 1.0f);
		lightingShader.setFloat("spotLight.linear", 0.09);
		lightingShader.setFloat("spotLight.quadratic", 0.032);
		lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(9.0f)));
		lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(12.0f)));

		// view/projection transformations
		glm::mat4 view = gCamera.GetViewMatrix();
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);

		// world transformation
		glm::mat4 model = glm::mat4(1.0f);
		lightingShader.setMat4("model", model);

		/*---------------------------------------------------------------------
		-----------------------Shape Rendering -------------------------------*/

		// setup to draw plane
		glBindTexture(GL_TEXTURE_2D, planeDiffuseMap);
		glBindVertexArray(planeVAO);
		model = model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.1f, 0.0f));
		lightingShader.setMat4("model", model);
		// draw plane
		glDrawElements(GL_TRIANGLES, planeNumIndices, GL_UNSIGNED_SHORT, (void*)planeIndexByteOffset);
		//---------------------------------------------------------------------
		// setup to draw cup bottom
		glBindTexture(GL_TEXTURE_2D, cupDiffuseMap);
		glBindVertexArray(sphereVAO);
		model = model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.6f, -1.3f));
		model = glm::scale(model, glm::vec3(0.5f));
		lightingShader.setMat4("model", model);
		// draw cup bottom
		glDrawElements(GL_TRIANGLES, sphereNumIndices, GL_UNSIGNED_SHORT, (void*)sphereIndexByteOffset);
		//---------------------------------------------------------------------
		//Draws the Printer body
		glBindTexture(GL_TEXTURE_2D, printerFace);
		glBindVertexArray(vao);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-3.35f, 0.0f, -0.8f));
		model = glm::scale(model, glm::vec3(2.6f, 1.75f, 2.0f));
		lightingShader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Draw printertop
		glBindTexture(GL_TEXTURE_2D, printerTop);
		glBindVertexArray(vao);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-3.35f, 0.42f, -0.8f));
		model = glm::scale(model, glm::vec3(2.58f, 1.0f, 1.98f));
		lightingShader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Draw printerRight side
		glBindTexture(GL_TEXTURE_2D, printerRight);
		glBindVertexArray(vao);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-2.27f, 0.0f, -0.8f));
		model = glm::scale(model, glm::vec3(0.5f, 1.75f, 1.98f));
		lightingShader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Draw printerLeft side
		glBindTexture(GL_TEXTURE_2D, printerLeft);
		glBindVertexArray(vao);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-4.42f, 0.0f, -0.8f));
		model = glm::scale(model, glm::vec3(0.5f, 1.75f, 1.98f));
		lightingShader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Draw printer Tray
		glBindTexture(GL_TEXTURE_2D, printerTray);
		glBindVertexArray(vao);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-3.50f, -0.5f, 0.4f));
		model = glm::scale(model, glm::vec3(2.0f, .1f, 2.0f));
		lightingShader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);
		//--------------------------------------------------------------------
		// Draws the Cylinder for cup
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cupFaceCYL);
		glBindVertexArray(cupVAO);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.26f, -1.3f));
		model = glm::scale(model, glm::vec3(0.5f));
		lightingShader.setMat4("model", model);
		static_meshes_3D::Cylinder C(1.0, 150, 1.25, true, true, true);
		C.render();
		//----------------------------------------------------------------------
		//Draw 2nd sphere for cup shape
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cupFace);
		glBindVertexArray(sphereVAO2);
		model = model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.3f));
		model = glm::scale(model, glm::vec3(0.5f));
		lightingShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, sphereNumIndices2, GL_UNSIGNED_SHORT, (void*)sphereIndexByteOffset2);
		//------------------------------------------------------------------------
		//Cylinder for mug handle
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cupHandle);
		glBindVertexArray(cupVAO3);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.21f, 0.17f, -1.12f));
		angle = 90.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(3.0f, 0.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.5f));
		lightingShader.setMat4("model", model);
		static_meshes_3D::Tube T2(.85, 150, .25, true, true, true);
		T2.render();
		
		//----------------------------------------------------------------------------
		//faux plant cylinder
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, plantBase);
		glBindVertexArray(cupVAO4);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.25f, -0.28f, -0.95f));
		model = glm::scale(model, glm::vec3(0.5f));
		lightingShader.setMat4("model", model);
		static_meshes_3D::Cylinder C3(1.0, 150, 1.42 , true, true, true);
		C3.render();
		//------------------------------------------------------------------------------
		//Faux plant branches
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, plantBranch);
		glBindVertexArray(cupVAO5);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.25f, -0.15f, -0.95f));
		model = glm::scale(model, glm::vec3(0.2f));
		lightingShader.setMat4("model", model);
		static_meshes_3D::Cylinder C4(.1, 150, 12.0, true, true, true);
		C4.render();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, plantBranch);
		glBindVertexArray(cupVAO2);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.15f, .35f, -1.05f));
		angle = -40.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(3.0f, 0.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.2f));
		lightingShader.setMat4("model", model);
		static_meshes_3D::Cylinder C5(.1, 150, 4.0, true, true, true);
		C5.render();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, plantBranch);
		glBindVertexArray(cupVAO6);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.33f, .38f, -.87f));
		angle = 37.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(3.0f, 0.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.2f));
		lightingShader.setMat4("model", model);
		static_meshes_3D::Cylinder C6(.1, 150, 4.0, true, true, true);
		C6.render();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, plantBranch);
		glBindVertexArray(cupVAO7);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.20f, 0.25f, -0.75f));
		angle = 50.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(3.0f, 0.0f, 0.90f));
		model = glm::scale(model, glm::vec3(0.2f));
		lightingShader.setMat4("model", model);
		static_meshes_3D::Cylinder C7(.1, 150, 4.0, true, true, true);
		C7.render();

		//------------------------------------------------------------------------------
		//Faux leaves
		// setup to draw plane
		glBindTexture(GL_TEXTURE_2D, plantLeaf);
		glBindVertexArray(plantLeafVAO);
		model = model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.55f, 1.39f, -0.77f));
		angle = 35.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.35f));
		lightingShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, planeNumIndices2, GL_UNSIGNED_SHORT, (void*)planeIndexByteOffset2);

		glBindTexture(GL_TEXTURE_2D, plantLeaf);
		glBindVertexArray(plantLeafVAO);
		model = model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.25f, 1.195f, -0.835f));
		angle = 35.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.368f));
		lightingShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, planeNumIndices2, GL_UNSIGNED_SHORT, (void*)planeIndexByteOffset2);

		glBindTexture(GL_TEXTURE_2D, plantLeaf);
		glBindVertexArray(plantLeafVAO);
		model = model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.25f, 0.9645f, -1.25f));
		angle = 25.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.3f, 0.3f));
		model = glm::scale(model, glm::vec3(0.52f));
		lightingShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, planeNumIndices2, GL_UNSIGNED_SHORT, (void*)planeIndexByteOffset2);

		glBindTexture(GL_TEXTURE_2D, plantLeaf);
		glBindVertexArray(plantLeafVAO);
		model = model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.25f, 0.70f, -1.0f));
		angle = 20.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.5f, 0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.28f));
		lightingShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, planeNumIndices2, GL_UNSIGNED_SHORT, (void*)planeIndexByteOffset2);

		glBindTexture(GL_TEXTURE_2D, plantLeaf);
		glBindVertexArray(plantLeafVAO);
		model = model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.22f, 0.63f, -0.20f));
		angle = 23.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::scale(model, glm::vec3(0.30f));
		lightingShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, planeNumIndices2, GL_UNSIGNED_SHORT, (void*)planeIndexByteOffset2);

		glBindTexture(GL_TEXTURE_2D, plantLeaf);
		glBindVertexArray(plantLeafVAO);
		model = model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.83f, 0.85f, -0.63f));
		angle = -23.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.5f, 0.9f, 0.5f));
		model = glm::scale(model, glm::vec3(0.42f));
		lightingShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, planeNumIndices2, GL_UNSIGNED_SHORT, (void*)planeIndexByteOffset2);

		glBindTexture(GL_TEXTURE_2D, plantLeaf);
		glBindVertexArray(plantLeafVAO);
		model = model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.62f, 0.815f, -0.50f));
		angle = 40.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.5f, 0.9f, 0.5f));
		model = glm::scale(model, glm::vec3(0.3f));
		lightingShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, planeNumIndices2, GL_UNSIGNED_SHORT, (void*)planeIndexByteOffset2);

		glBindTexture(GL_TEXTURE_2D, plantLeaf);
		glBindVertexArray(plantLeafVAO);
		model = model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.40f, 0.90f, -0.82f));
		angle = 5.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.01f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.25f));
		lightingShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, planeNumIndices2, GL_UNSIGNED_SHORT, (void*)planeIndexByteOffset2);

		//--------------------------------------------------------------------------------
		//"Dirt" in faux plant
		glBindTexture(GL_TEXTURE_2D, plantDirt);
		glBindVertexArray(sphereVAO);
		model = model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.25f, -0.28f, -0.95f));
		model = glm::scale(model, glm::vec3(0.5f));
		lightingShader.setMat4("model", model);
		// draw cup bottom
		glDrawElements(GL_TRIANGLES, sphereNumIndices, GL_UNSIGNED_SHORT, (void*)sphereIndexByteOffset);

		//----------------------------------------------------------------------------------
		//"Background"
		glBindTexture(GL_TEXTURE_2D, wall);
		glBindVertexArray(planeVAO);
		model = model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.1f, -2.25f));
		angle = 90.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
		lightingShader.setMat4("model", model);
		// draw plane
		glDrawElements(GL_TRIANGLES, planeNumIndices, GL_UNSIGNED_SHORT, (void*)planeIndexByteOffset);

		//-------------------------------------------------------------------------------------
		


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	//glDeleteVertexArrays(1, &cubeVAO);
	//glDeleteBuffers(1, &cubeVBO);

	//glDeleteVertexArrays(1, &cylinderVAO);
	//glDeleteBuffers(1, &cylinderVBO);

	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &planeVBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void UProcessInput(GLFWwindow* window)
{

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	float cameraOffset = cameraSpeed / gDeltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		gCamera.ProcessKeyboard(FORWARD, gDeltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		gCamera.ProcessKeyboard(BACKWARD, gDeltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		gCamera.ProcessKeyboard(LEFT, gDeltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		gCamera.ProcessKeyboard(RIGHT, gDeltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		gCamera.ProcessKeyboard(DOWN, gDeltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		gCamera.ProcessKeyboard(UP, gDeltaTime);

	glfwSetScrollCallback(window, UMouseScrollCallback);

	// change view between perspective and orthographics
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		if (perspective) {
			projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);
			perspective = false;
		}
		else {
			projection = glm::perspective(glm::radians(gCamera.Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
			perspective = true;
		}
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void UResizeWindow(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (gFirstMouse)
	{
		gLastX = xpos;
		gLastY = ypos;
		gFirstMouse = false;
	}

	float xoffset = xpos - gLastX;
	float yoffset = gLastY - ypos;

	gLastX = xpos;
	gLastY = ypos;

	gCamera.ProcessMouseMovement(xoffset, yoffset);
}

void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	cameraSpeed += (float)yoffset;
	if (cameraSpeed > 1.0f)
		cameraSpeed = 1.0f;
	if (cameraSpeed < 45.0f)
		cameraSpeed = 45.0f;

	gCamera.ProcessMouseScroll(yoffset);
	//cout << "Mouse wheel (" << xoffset << ", " << yoffset << ")" << endl;
}

void UDestroyMesh(GLMesh& mesh)
{
	glDeleteVertexArrays(1, &mesh.vao);
	glDeleteBuffers(2, mesh.vbos);
}


void destroyTexture(GLuint gTextureId)
{ glGenTextures(1, &gTextureId); }

unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

