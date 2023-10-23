// GLAD is responsible for defining methods (function pointers) from memory so we don't need to
#include <glad/glad.h>

// GLFW is the multiplatform lib responsible for managing windows 
#include <GLFW/glfw3.h>
#include <iostream>

#define __WIREFRAME__

// Prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
" FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);" // Fixed orange-ish color
"}\n"
"";


int main()
{
	// Initialize GLFW
	glfwInit();

	// -------- Configure GLFW

	// Tell GLFW that we are using opengl 3.3 (major.mini)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// We want to use the core-profile (not the immediate one)
	// This way we'll get access to only a smaller subset of features without backward-compatible features we don't need
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // --- MacOS specific line
#endif

	// Creates a window --- This is just a window by GLFW, we'll still need to render things inside it
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Makes the window our current context -- This way opengl will draw it 
	glfwMakeContextCurrent(window);

	// Register to the resize glfw window callback 
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Initializing GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// ---- Dealing with the Vertex Shader ---------
	// Responsible for transforming 3D coordinates into different 3D coordinates
	// it also allows us to do some basic processing on the vertex attributes

	// Unique ID corresponding to the Vertex Shader
	// Generate a vertex Shader and return its generated ID
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Now attach the shader source code to the vertex shader instance we've just created
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// 0: The vertex shader object
	// 1: How many strings we're passing as source code
	// 2: Actual source code
	// 3: Can leave it as NULL

	// Then compile it
	glCompileShader(vertexShader);

	// ----- Checking for compilation success -----
	//      -------------------------------
	int success; // status
	char infoLog[512]; // Container for error messages (if any)
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		// Retrieve error logs
		glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// ---- Dealing with fragment shader -----
	//      -------------------------------

	// Calculates the final color of a pixel
	// The fragment shader contains data about the 3D scene that influences the final pixel color
	// * lights, shadow, color of the light and so on
	// 
	// Advanced shader techniques are usually done here

	// Unique ID corresponding to the Fragment Shader
	// Generate a fragment Shader and return its generated ID
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Now attach the shader source code to the vertex shader instance we've just created
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	// 0: The vertex shader object
	// 1: How many strings we're passing as source code
	// 2: Actual source code
	// 3: Can leave it as NULL

	// Then compile it
	glCompileShader(fragmentShader);

	// -- Checking for compilation success ---
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		// Retrieve error logs
		glGetShaderInfoLog(fragmentShader, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// ----- Linking both shaders (vertex->fragment) ------
	//      -------------------------------

	// Unique ID corresponding to the Shader Program
	unsigned int shaderProgram = glCreateProgram();

	// Attach previously created vertex\fragment shaders to the ShaderProgram
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	// Now link attached shaders
	glLinkProgram(shaderProgram);

	// -- Checking for linking success ---
	//      -------------------------------
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (!success)
	{
		// Retrieve error logs
		glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::SHADER::LINKING::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Delete the shaders (standalone) as they're already linked to the program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// ----- Draw the Triangle ----------
	//      ------------------

	float vertices[] = {
		0.5f, 0.5f, 0.0f, // top right
		0.5f, -0.5f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f // top left
	};
	unsigned int indices[] = { // note that we start from 0!
		0, 1, 3, // first triangle
		1, 2, 3 // second triangle
	};

	// Unique ID corresponding to the vertex array object (VAO)
	// VAO is used as for holding VBOs, so
	// * we just configure VBOs once
	// * bind then to the associated VAO
	// * bind the VAO when we want to use its settings (defined in VBO) to draw something
	// * unbind the VAO when we dosn't want to draw it anymore
	unsigned int VAO;

	// Fill VAO with the generated ID by calling glGenVertexArrays, thus generating a VAO buffer
	glGenVertexArrays(1, &VAO);

	// Binding the VAO -- From now on, we should configure the VBO(s) that go/goes inside it
	// And then unbind the VAO for later use

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	// Unique ID corresponding to the vertex buffer object (VBO) we have to use 
	// in order to allocate memory in the GPU
	// VBOs are used to send as much data as possible at once from the CPU to the GPU
	// ** This is important because sending data from the CPU is slow, so we want to send as much as possible at once
	unsigned int VBO;

	// Fill VBO with the generated ID by calling glGenBuffers, thus generating a Buffer
	glGenBuffers(1, &VBO);

	// Bind the generated buffer to the 'GL_ARRAY_BUFFER' target
	// 'GL_ARRAY_BUFFER' is the type of Vertex Buffers 
	// * So we're telling OpenGL to use our VBO Buffer as a vertex buffer
	// * From now on, every time we refer to GL_ARRAY_BUFFER, openGL will use our VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Now copy our buffer data (vertices array) to our buffer's memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// GLEnum type can be:
	// * GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
	// * GL_STATIC_DRAW : the data is set only once and used many times.
	// * GL_DYNAMIC_DRAW : the data is changed a lot and used many times.
	// It's VERY IMPORTANT to use the correct GLEnum type, because if you tell opengl that your data is
	// going to change a lot (GL_DYNAMIC_DRAW) then it'll write your data in a faster memory


	// Create Element Buffer Object
	// * Responsible for enabling Indexed Drawing -- It stores indices that OpenGL uses to decice what vertices to draw
	unsigned int EBO;
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// ------ Linking the vertex attributes -------
	//      -------------------------------

	// Now that the Shaders are linked to the program, we want to tell opengl how to interpret them
	// Our Vertex Shader requires an Input Vector3 param, it is stored in memory like:
	// * Vertex1
	// ** X (4 bytes) - 0->4 
	// ** Y (4 bytes) - 4->8
	// ** Z (4 bytes) - 8->12 <---- 0->12, so 12 is our stride size
	// * Vertex2
	// ** X (4 bytes) - 12->16 <---- Starts from 12, so our offset between vertex is 0 (zero)
	// ** Y (4 bytes) - 16->20
	// ** Z (4 bytes) - 20->24
	// * Vertex3
	// ** X (4 bytes) - 24->28
	// ** Y (4 bytes) - 28->32
	// ** Z (4 bytes) - 32->36

	const int location = 0; // Value retrieve from the vertrex shader: "layout (location = 0) in vec3 aPos;" 
	const int vec_size = 3; // Our data is composed of 3 floats
	const int offset = 0; // Space between vertex in our code structure
	const bool normalize_data = GL_FALSE; // We don't want the shader 

	// Tell opengl how to interprete our vertex data
	glVertexAttribPointer(location, vec_size, GL_FLOAT, normalize_data, vec_size * sizeof(float), (void*)offset);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// Then apply the newly configured vertexAttribPointer to the array 0
	glEnableVertexAttribArray(location);

	// note that this is allowed, the call to glVertexAttribPointer registered 
	//VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

#ifdef __WIREFRAME__
	// Draw using wireframes
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

	// ------ THE RENDER LOOP
	// Check if the window was closed
	while (!glfwWindowShouldClose(window))
	{
		// Call the input process every render frame
		processInput(window);

		// ------ Rendering commands here... -----
		//      --------------------------

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


		// Now that the shaderProgram is created and linked, we can use it
		glUseProgram(shaderProgram);

		glBindVertexArray(VAO);
		// From now on, every rendering call will use this program (thus its shaders)

		// Render the triangle
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		// Render elements <--- Render triangles from an index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, sizeof(vertices), GL_UNSIGNED_INT, 0);



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

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

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
	// Set the rendering size
	// glViewport((lower left corner [x, y]), width, height);
	// This Viewport setting DOESN'T need to match the GLFW settings
	// 
	// Always resize the viewport to match our GLFW size --- NOT A RULE
	glViewport(0, 0, width, height);
}
