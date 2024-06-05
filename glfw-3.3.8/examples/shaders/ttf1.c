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
"uniform vec2 iResolution;\n"
"uniform float iTime;\n"
"ivec2 beziers[62] = ivec2[](\n"
"    // First glyph\n"
"    ivec2(46127, 22077), // 0  \n"
"    ivec2(46127, 33000),       \n"
"    ivec2(46127, 33000), // 2  \n"
"    ivec2(43680, 31929),       \n"
"    ivec2(40261, 31271), // 4  \n"
"    ivec2(36843, 30614),       \n"
"    ivec2(28314, 29630), // 6  \n"
"    ivec2(19416, 28541),       \n"
"    ivec2(15453, 25883), // 8  \n"
"    ivec2(11490, 23225),       \n"
"    ivec2(11565, 18475), // 10 \n"
"    ivec2(11931, 8976),        \n"
"    ivec2(25257, 8715),  // 12 \n"
"    ivec2(34994, 8744),        \n"
"    ivec2(41740, 14176), // 14 \n"
"    ivec2(46426, 17691),       \n"
"    ivec2(46127, 22077),       \n"
"                               \n"
"    // Second glyph            \n"
"    ivec2(65535, 8599),  // 17 \n"
"    ivec2(65535, 1278),        \n"
"    ivec2(65535, 1278),  // 19 \n"
"    ivec2(62843, 679),         \n"
"    ivec2(61248, 450),   // 21 \n"
"    ivec2(59653, 222),         \n"
"    ivec2(57958, 232),   // 23 \n"
"    ivec2(47456, 0),           \n"
"    ivec2(46526, 9180),  // 25 \n"
"    ivec2(35393, 0),           \n"
"    ivec2(22864, 232),   // 27 \n"
"    ivec2(12188, 323),         \n"
"    ivec2(6148, 5098),   // 29 \n"
"    ivec2(108, 9873),          \n"
"    ivec2(0, 18243),     // 31 \n"
"    ivec2(33, 30037),          \n"
"    ivec2(12230, 34510), // 33 \n"
"    ivec2(14946, 35571),       \n"
"    ivec2(19109, 36282), // 35 \n"
"    ivec2(23271, 36994),       \n"
"    ivec2(34562, 38229), // 37 \n"
"    ivec2(41204, 38944),       \n"
"    ivec2(43684, 40422), // 39 \n"
"    ivec2(46164, 41900),       \n"
"    ivec2(46127, 44968), // 41 \n"
"    ivec2(46127, 47524),       \n"
"    ivec2(46127, 47524), // 43 \n"
"    ivec2(45928, 56414),       \n"
"    ivec2(30574, 56588), // 45 \n"
"    ivec2(15320, 56762),       \n"
"    ivec2(14224, 45782), // 47 \n"
"    ivec2(3057, 45782),        \n"
"    ivec2(3057, 45782),  // 49 \n"
"    ivec2(3257, 54293),        \n"
"    ivec2(7843, 58447),  // 51 \n"
"    ivec2(15320, 65390),       \n"
"    ivec2(30973, 65535), // 53 \n"
"    ivec2(56928, 65332),       \n"
"    ivec2(57160, 48919), // 55 \n"
"    ivec2(57160, 13130),       \n"
"    ivec2(57160, 13130), // 57 \n"
"    ivec2(57260, 8482),        \n"
"    ivec2(63142, 8366),  // 59 \n"
"    ivec2(63940, 8308),        \n"
"    ivec2(65535, 8599)         \n"
");\n"
"vec2 fetchBezier(int i) {\n"
"    // Normalize point between -1.0 and 1.0\n"
"	return vec2(beziers[i] - 32768) / 32767.0;\n"
"}\n"
"vec2 solvePoly(vec2 p1, vec2 p2, vec2 p3) {\n"
"    vec2 a = p1 - (p2 * 2.0) + p3;\n"
"    vec2 b = p1 - p2;\n"
"    float ra = 1.0 / a.y;\n"
"    float rb = 0.5 / b.y;\n"
"    float d = sqrt(max(b.y * b.y - a.y * p1.y, 0.0));\n"
"    float t1 = (b.y - d) * ra;\n"
"    float t2 = (b.y + d) * ra;\n"
"    // Verify if a.y is near zero\n"
"    if (abs(a.y) < 0.0001) {\n"
"        t1 = t2 = p1.y * rb;\n"
"    }\n"
"    return vec2(\n"
"        (a.x * t1 - b.x * 2.0) * t1 + p1.x,\n"
"        (a.x * t2 - b.x * 2.0) * t2 + p1.x \n"
"    );\n"
"}\n"
"uint calcRootCode(float y1, float y2, float y3) {\n"
"    uint i1 = floatBitsToUint(y1) >> 31;\n"
"    uint i2 = floatBitsToUint(y2) >> 30;\n"
"    uint i3 = floatBitsToUint(y3) >> 29;\n"
"    uint shift = (i2 & 2u) | (i1 & ~2u);\n"
"    shift = (i3 & 4u) | (shift & ~4u);\n" 
"    return ((0x2E74u >> shift) & 0x0101u);\n"
"}\n"
"bool testCurve(uint code) {\n"
"    return (code != 0u);\n"
"}\n"
"bool testRoot1(uint code) {\n"
"    return ((code & 1u) != 0u);\n"
"}\n"
"bool testRoot2(uint code) {\n"
"    return code > 1u;\n"
"}\n"
"void updateCoverage(in vec2 pixelsPerEm, in vec2 p1, in vec2 p2, in vec2 p3, inout float coverage){\n"
"    uint code = calcRootCode(p1.y, p2.y, p3.y);\n"
"    if (testCurve(code)) {\n"
"        vec2 r = solvePoly(p1, p2, p3);\n"
"        r.x = clamp(r.x * pixelsPerEm.x + 0.5, 0.0, 1.0); \n"
"        r.y = clamp(r.y * pixelsPerEm.x + 0.5, 0.0, 1.0); \n"
"        if (testRoot1(code)) coverage += r.x;\n"
"        if (testRoot2(code)) coverage -= r.y;\n"
"    }\n"
"    p1 = vec2(p1.y, -p1.x);\n"
"    p2 = vec2(p2.y, -p2.x);\n"
"    p3 = vec2(p3.y, -p3.x);\n"
"    code = calcRootCode(p1.y, p2.y, p3.y);\n"
"    if (testCurve(code)) {\n"
"        vec2 r = solvePoly(p1, p2, p3);\n"
"        r.x = clamp(r.x * pixelsPerEm.y + 0.5, 0.0, 1.0);\n"
"        r.y = clamp(r.y * pixelsPerEm.y + 0.5, 0.0, 1.0);\n"
"        if (testRoot1(code)) coverage += r.x;\n"
"        if (testRoot2(code)) coverage -= r.y;\n"
"    }\n"
"}\n"
"void mainImage( out vec4 fragColor, in vec2 fragCoord ){\n"
"    // Normalize fragment position between -1 and 1\n"
"	vec2 position = (2.0*fragCoord-iResolution.xy)/iResolution.y;\n"
"    // Antialiasing, calculate how many pixels are used to render this glyph\n"
"    // Ref: https://gamedev.stackexchange.com/a/130933\n"
"    vec2 pixelsPerEm = vec2(\n"
"       1.0 / fwidth(position.x),\n"
"	    1.0 / fwidth(position.y)\n"
"    );\n"
"    // Zoom animation, scale is a value between 0.16 and 16.0\n"
"    float scale = abs(cos(iTime/2.0)) * 0.99 + 0.01;\n"
"    scale = scale * scale * 16.0;\n"
"    position *= scale;\n"
"    pixelsPerEm /= scale;\n"
"    // Calculate pixel coverage\n"
"    float coverage = 0.0;\n"
"    for (int i=0;i<=14;i+=2) {\n"
"        vec2 p0 = fetchBezier(i+0) - position;\n"
"        vec2 p1 = fetchBezier(i+1) - position;\n"
"        vec2 p2 = fetchBezier(i+2) - position;\n"
"        updateCoverage(pixelsPerEm, p0, p1, p2, coverage);\n"
"    }\n"
"    for (int i=17;i<=59;i+=2) {\n"
"        vec2 p0 = fetchBezier(i+0) - position;\n"
"        vec2 p1 = fetchBezier(i+1) - position;\n"
"        vec2 p2 = fetchBezier(i+2) - position;\n"
"        updateCoverage(pixelsPerEm, p0, p1, p2, coverage);\n"
"    }\n"
"    // Normalize coverage\n"
"    coverage = sqrt(clamp(abs(coverage) * 0.5, 0.0, 1.0));\n" 
"    // Final color\n"
"    fragColor = vec4(coverage, coverage, coverage, 1.0);\n"
"}\n"
"void main() {\n"
"    mainImage(gl_FragColor, gl_FragCoord.xy);\n"
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

    // float vertices[] = {
         // 0.5f,  0.5f, 0.0f,  // top right
         // 0.5f, -0.5f, 0.0f,  // bottom right
        // -0.5f, -0.5f, 0.0f,  // bottom left
        // -0.5f,  0.5f, 0.0f   // top left 
    // };
    float vertices[] = {
         1.f,  1.f, 0.0f,  // top right
         1.f, -1.f, 0.0f,  // bottom right
        -1.f, -1.f, 0.0f,  // bottom left
        -1.f,  1.f, 0.0f   // top left 
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
    GLint viewport[4];
	glGetIntegerv( GL_VIEWPORT, viewport );
    // Set a clear color.
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // 2. use our shader program when we want to render an object
    glUseProgram(shaderData.shaderProgram);
    glBindVertexArray(renderSetupData.VAO);
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderSetupData.EBO);
    GLint loc = glGetUniformLocation(shaderData.shaderProgram, "iResolution");
    // glUniform2f(loc, (viewport[2]>>1), (viewport[3]>>1));
    glUniform2f(loc, viewport[2], viewport[3]);
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
        SCR_WIDTH, SCR_HEIGHT, "Shader: implicit circle with adaptive antialiasing", NULL, NULL);
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
    GLint iTimeLoc = glGetUniformLocation(shaderData.shaderProgram, "iTime");

    // Start rendering infinitely.
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        // Set the uniform value of iTime
        float currentTime = glfwGetTime();
		glUniform1f(iTimeLoc, currentTime);
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