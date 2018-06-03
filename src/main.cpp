// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

#include <time.h>

#include <glm/gtc/matrix_transform.hpp>

#include "common/shader.hpp"
#include "common/texture.hpp"
#include "common/controls.hpp"
#include "common/objloader.hpp"
#include "common/vboindexer.hpp"

#include "glcontext.hpp"
#include "time.hpp"
#include "model.hpp"
#include "scene.hpp"
#include "componentMeshRenderer.hpp"

// Include GLM
#include <glm/glm.hpp>
using namespace glm;


Model model;
Model model2;
Scene scene;


const int WIDTH = 1024;
const int HEIGHT = 768;


int main(int argc, char *argv[])
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(WIDTH, HEIGHT, "Mario 64 C++", nullptr, nullptr);
	if( window == nullptr ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible."
			   	"Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Create a VAO
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// A texture
	GLuint image = loadDDS("../media/mario.dds");
	if (!image) {
		fprintf(stderr, "Failed to load sample texture");
		return -1;
	}
	// Load dummy model
	model.importFromObj("../media/350z.obj");
	model2.importFromDae("../media/skeltest.dae");

	// Load some shaders
	GLuint programID = LoadShaders("vertex.glsl", "fragment.glsl");
	GLuint MMatrixID = glGetUniformLocation(programID, "M");
	GLuint VMatrixID = glGetUniformLocation(programID, "V");
	GLuint PMatrixID = glGetUniformLocation(programID, "P");
	GLuint MVPMatrixID = glGetUniformLocation(programID, "MVP");

	GLContext::instance()->setCurrentProgram(programID);


	// Create the game timer
	Time::begin();


	// Ssample scene
	GameObject *obj = scene.spawn();
	obj->scale = vec3(0.2f);
	obj->addComponent<ComponentMeshRenderer>();
	ComponentMeshRenderer *renderer =  (ComponentMeshRenderer*) obj->getComponents().get(-1);
	renderer->setModel(&model);

	GameObject *obj2 = scene.spawn();
	obj2->scale = vec3(0.15f);
	obj2->position = vec3(15, 0, 0);
	obj2->addComponent<ComponentMeshRenderer>();
	renderer = (ComponentMeshRenderer*)obj2->getComponents().get(-1);
	renderer->setModel(&model);
	
	GameObject *obj3 = scene.spawn();
	obj3->scale = vec3(0.1f);
	obj3->position = vec3(-15, 0, 0);
	obj3->addComponent<ComponentMeshRenderer>();
	renderer = (ComponentMeshRenderer*)obj3->getComponents().get(-1);
	renderer->setModel(&model2);

	// GAME LOOP
	do {
		Time::beginTick();

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glEnable(GL_CULL_FACE);

		// Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(programID);

		// Matrices
		glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		glm::mat4 View = glm::lookAt(
			glm::vec3(sinf(Time::time()) * 5.0f, 3.0f, 3.0f), // Cam position
			glm::vec3(0, 0, 0), // Cam looking at
			glm::vec3(0, 1, 0)  // Up
		);

		// Debug FPS cam
		computeMatricesFromInputs();
		Projection = getProjectionMatrix();
		View = getViewMatrix();


		scene.gameObjects.get(0)->position = vec3(sinf(Time::time() * 5), 0, 0);

		scene.gameObjects.get(2)->rotation = quat(vec3(0, Time::time(), 0));
		
		scene.update();

		// Matrix
		glm::mat4 Model = glm::mat4(1.0f);
		glm::mat4 mvp = Projection * View * Model;

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0);

	// Cleanup VBO and shader
	//glDeleteBuffers(1, &vertexbuffer);
	//glDeleteBuffers(1, &uvbuffer);
	//glDeleteBuffers(1, &normalbuffer);
	//glDeleteBuffers(1, &elementbuffer);
	glDeleteProgram(programID);
	glDeleteTextures(1, &image);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

