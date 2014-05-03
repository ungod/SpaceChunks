#include "SpaceChunks.h"

GLFWwindow* window;

int main(int argc, char **argv)
{
	if (!glfwInit())
	{
		printf("GLFW Failed to Init!");
		return EXIT_FAILURE;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(640, 480, "Space Chunks", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		printf("GLFW Failed to Create the Window!");
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		printf("GLEW Failed To Init");
		return EXIT_FAILURE;
	}

	printf("OpenGL Version: (%s)", glGetString(GL_VERSION));

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return EXIT_SUCCESS;
}

SpaceChunks::SpaceChunks()
{
}


SpaceChunks::~SpaceChunks()
{
}
