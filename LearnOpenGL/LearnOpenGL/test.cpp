// GLAD is responsible for defining methods (function pointers) from memory so we don't need to
#include <glad/glad.h>

// GLFW is the multiplatform lib responsible for managing windows 
#include <GLFW/glfw3.h>
#include <iostream>

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main()
{
	// Initialize GLFW
	glfwInit();

	// -------- Configure GLFW]

	// Tell GLFW that we are using opengl 3.3 (major.mini)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// We want to use the core-profile (not the immediate one)
	// This way we'll get access to only a smaller subset of features without backward-compatible features we don't need
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // --- MacOS specific line

	// Creates a window --- This is just a window by GLFW, we'll still need to render things inside it
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Makes the window our current context -- This way opengl will draw it 
	glfwMakeContextCurrent(window);

	// Initializing GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Set the rendering size
	// glViewport((lower left corner [x, y]), width, height);
	// This Viewport setting DOESN'T need to match the GLFW settings
	glViewport(0, 0, 800, 600);

	// Register to the resize glfw window callback 
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// ------ THE RENDER LOOP
	// Check if the window was closed
	while (!glfwWindowShouldClose(window))
	{
		// Call the input process every render frame
		processInput(window);

		// Rendering commands here...

		// Set the clear color buffer bit color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// Then clear the Color buffer bit of the screen to apply the new color
		glClear(GL_COLOR_BUFFER_BIT);

		/*
		* 'glClearColor' is a state-setting function <-- it sets things
		* 'glClear' is a state-using function <-- it uses previous set things
		*/

		// Possible buffer bits: 
		// * GL_COLOR_BUFFER_BIT 
		// * GL_DEPTH_BUFFER_BIT 
		// * GL_STENCIL_BUFFER_BIT

		// Swaps the color buffer
		// OpenGL works with Double Buffer
		// * The first\front buffer shows the final image
		// * The second\back buffer is written by the render loop
		// When the render loop finishes writting to the back buffer we shall then swap the two buffers
		glfwSwapBuffers(window);

		// Check if any events were triggered
		// * keyboard
		// * Mouse
		// Then updates the window state, and calls the corresponding functions (registered callbacks)
		glfwPollEvents();
	}

	// Deallocates all resources allocated to the GLFW
	glfwTerminate();

	return 0;
}

/*!
* Process user's input
*/
void processInput(GLFWwindow* window)
{
	// If the user is pressing ESCAPE
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		// Set the "WindowShouldClose" to true, thus breaking the render loop
		glfwSetWindowShouldClose(window, true);
	}
}


/*!
* Method used as callback every time the GLFW window is resized
  We'll use this to resize the viewport every time the GLFW window gets resized
*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// Always resize the viewport to match our GLFW size --- NOT A RULE
	glViewport(0, 0, width, height);
}
