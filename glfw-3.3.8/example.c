#include <glad/glad.h> // GLAD: https://github.com/Dav1dde/glad ... GLAD 2 also works via the web-service: https://gen.glad.sh/ (leaving all checkbox options unchecked)
#include <GLFW/glfw3.h>
 
#include <stdlib.h> 
 
int main()
{	
	// (1) GLFW: Initialise & Configure
	// -----------------------------------------
	if (!glfwInit())
		exit(EXIT_FAILURE);
 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
 
	int monitor_width = mode->width; // Monitor's width.
	int monitor_height = mode->height;
 
	int window_width = (int)(monitor_width * 0.5f); // Window size will be 50% the monitor's size...
	int window_height = (int)(monitor_height * 0.5f); // ... Cast is simply to silence the compiler warning.
 
	GLFWwindow* window = glfwCreateWindow(window_width, window_height, "GLFW Test Window – Changing Colours", NULL, NULL);
	// GLFWwindow* window = glfwCreateWindow(window_width, window_height, "Drawing Basic Shapes - Buffer Objects & Shaders", glfwGetPrimaryMonitor(), NULL); // Full Screen Mode ("Alt" + "F4" to Exit!)
 
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
 
	glfwMakeContextCurrent(window); // Set the window to be used and then centre that window on the monitor. 
	glfwSetWindowPos(window, (monitor_width - window_width) / 2, (monitor_height - window_height) / 2);
 
	glfwSwapInterval(1); // Set VSync rate 1:1 with monitor's refresh rate.
 
	// (2) GLAD: Load OpenGL Function Pointers
	// -------------------------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) // For GLAD 2 use the following instead: gladLoadGL(glfwGetProcAddress)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
 
	// (3) Enter the Main-Loop
	// --------------------------------	
	float red = 0.0f; // Set individual colours here: OpenGL RGB [0, 1] = Actual RGB / 255
	float green = 0.35f;
	float blue = 0.7f;
 
	int red_dir = 1; // Colour change direction.
	int green_dir = 1;
	int blue_dir = 1;
 
	while (!glfwWindowShouldClose(window)) // Main-Loop
	{
		red += 0.02f * red_dir; // Increase or decrease colour values.
		green += 0.03f * green_dir;
		blue += 0.04f * blue_dir;
 
		if (red > 1 || red < 0) // Reverse the colour changing direction.
			red_dir = -red_dir;
 
		if (green > 1 || green < 0)
			green_dir = -green_dir;
 
		if (blue > 1 || blue < 0)
			blue_dir = -blue_dir;
 
		//std::cout << "Red: " << red << " --- Green: " << green << " --- Blue: " << blue << std::endl;
 
		glClearColor(red , green, blue, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT); // Clear the screen with... red, green, blue.
 
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	/* glfwDestroyWindow(window) // Call this function to destroy a specific window */
	glfwTerminate(); // Destroys all remaining windows and cursors, restores modified gamma ramps, and frees resources.
 
	exit(EXIT_SUCCESS); // Function call: exit() is a C/C++ function that performs various tasks to help clean up resources.
}
