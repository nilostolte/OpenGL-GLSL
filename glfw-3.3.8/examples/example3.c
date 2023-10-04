#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

#define true 1
#define false 0
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Container struct for shaders & programs.
typedef struct {
    unsigned int vertexShader;
    unsigned int fragmentShader;
    unsigned int shaderProgram;
    unsigned success;
} ShaderData;

static ShaderData nullShader = {0, 0, 0, 0};


 void ShaderDatacleanup(ShaderData s) {
    glDeleteShader(s.vertexShader);
    glDeleteShader(s.fragmentShader);
    glDeleteProgram(s.shaderProgram);
}

// Container struct for data associated with setup stage.
typedef struct {
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
} RenderSetupData;

void RenderSetupDatacleanup(RenderSetupData r) {
    glDeleteVertexArrays(1, &r.VAO);
    glDeleteBuffers(1, &r.VBO);
	glDeleteBuffers(1, &r.EBO);
}

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main(){\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\n";

const char* fragmentShaderSource = 
"#version 330 core\n"
"void main(){\n"
"    gl_FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); \n"
"}\n";

unsigned int setupVertexShader() {
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		fprintf(stderr, "Error: %s\n%s\n", "ERROR::SHADER::VERTEX::COMPILATION_FAILED",infoLog);
        return 0;
    }

    return vertexShader;
}

unsigned int setupFragmentShader() {
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    int  success;
    char infoLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		fprintf(stderr, "Error: %s\n%s\n", "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED",infoLog);
        return 0;
    }

    return fragmentShader;
}

ShaderData setupShaderProgram() {
    ShaderData result;
    result.shaderProgram = glCreateProgram();
    result.vertexShader = setupVertexShader();
    result.fragmentShader = setupFragmentShader();

    if (result.vertexShader == 0 || result.fragmentShader == 0) {
        return nullShader;
    }

    glAttachShader(result.shaderProgram, result.vertexShader);
    glAttachShader(result.shaderProgram, result.fragmentShader);
    glLinkProgram(result.shaderProgram);

    int  success;
    char infoLog[512];
    glGetProgramiv(result.shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(result.shaderProgram, 512, NULL, infoLog);
        fprintf(stderr, "Error: %s\n%s\n", "ERROR::SHADER::Program::COMPILATION_FAILED",infoLog);
        ShaderDatacleanup(result);
        return nullShader;
    }

    result.success = true;
    return result;
}

RenderSetupData setupRendering() {
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    // float vertices[] = {
        // -0.5f, -0.5f, 0.0f,  // left
         // 0.5f, -0.5f, 0.0f,  // right
         // 0.0f,  0.5f, 0.0f   // top
    // };
float vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
};
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};
    // ..:: Initialization code (done once (unless your object frequently changes)) :: ..
    // 1. bind Vertex Array Object
    RenderSetupData result;
    glGenVertexArrays(1, &result.VAO);
    glGenBuffers(1, &result.VBO);
    glGenBuffers(1, &result.EBO);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(result.VAO);


    glBindBuffer(GL_ARRAY_BUFFER, result.VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result.EBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint),
             &indices[0], GL_STATIC_DRAW);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    return result;
}

void render(ShaderData shaderData, RenderSetupData renderSetupData) {
    // Set a clear color.
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // 2. use our shader program when we want to render an object
    glUseProgram(shaderData.shaderProgram);
    glBindVertexArray(renderSetupData.VAO);
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderSetupData.EBO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // ^ Needed for Mac OS.

    GLFWwindow* window = glfwCreateWindow(
        SCR_WIDTH, SCR_HEIGHT, "Hello World OpenGL", NULL, NULL);
    if (window == NULL){
        fprintf(stderr, "Error: %s\n", "Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Initialize GLAD.
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)){
		fprintf(stderr, "Error: %s\n", "Failed to initialize GLAD");
        return -1;
    }

    ShaderData shaderData = setupShaderProgram();
    if (!shaderData.success){
        return -1;
    }

    RenderSetupData renderSetupData = setupRendering();

    // Start rendering infinitely.
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        // Render
        render(shaderData, renderSetupData);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean-up
    ShaderDatacleanup(shaderData);
    RenderSetupDatacleanup(renderSetupData);

    glfwTerminate();
    return 0;
}

/** glfw: whenever the window size changed (by OS or user resize) this callback function executes. */
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

/** Process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly. */
void processInput(GLFWwindow* window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}