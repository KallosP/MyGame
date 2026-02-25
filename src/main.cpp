#include "config.h"
#include "triangle_mesh.h"

// IMPORTANT NOTE: if a crash occurs and terminal output has red x in circle, but no errors in output, you might be looking at a seg fault

using namespace std;
unsigned int make_shader(const string& vertex_filepath, const string& fragment_filepath); 
unsigned int make_module(const string& filepath, unsigned int module_type); 

int main(){
    GLFWwindow* window;

    if (!glfwInit()) {
        cout << "GLFW couldn't start" << endl;
        return -1;
    }

    window = glfwCreateWindow(640, 480, "My Window", NULL, NULL);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        cout << "Couldn't load opengl" << endl;
        glfwTerminate();
        return -1;
    }

    glClearColor(0.25f, 0.5f, 0.75f, 1.0f);
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    glViewport(0,0,w,h);

    TriangleMesh* triangle = new TriangleMesh();

    unsigned int shader = make_shader(
        "shaders/vertex.txt",
        "shaders/fragment.txt"
    );

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shader);
        triangle->draw();
        glfwSwapBuffers(window);
    }

    glDeleteProgram(shader);
    glfwTerminate();

    return 0;
} 

// Creates a shader program
unsigned int make_shader(const string& vertex_filepath, const string& fragment_filepath) {
    vector<unsigned int> modules;
    modules.push_back(make_module(vertex_filepath, GL_VERTEX_SHADER));
    modules.push_back(make_module(fragment_filepath, GL_FRAGMENT_SHADER));

    unsigned int shader = glCreateProgram();
    for (unsigned int shaderModule : modules) {
        glAttachShader(shader, shaderModule);
    }
    glLinkProgram(shader);

    // The linking will sometimes fail, so need to check if it was successful
    int success;
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if(!success) {
        char errorLog[1024];
        glGetProgramInfoLog(shader, 1024, NULL, errorLog);
        cout << "Shader Linking error:\n" << errorLog << endl;
    }

    // The modules aren't needed anymore after linking
    for (unsigned int shaderModule : modules) {
        glDeleteShader(shaderModule);
    }

    return shader;
}

// Takes a file path and creates a shader module (a shader module is a piece/part of the shader program)
unsigned int make_module(const string& filepath, unsigned int module_type) { // & = pass by reference (more efficient than pass by copy). references the value from its original memory address
    ifstream file;
    stringstream bufferedLines; 
    string line;

    file.open(filepath);
    while(getline(file, line)){
        bufferedLines << line << "\n"; // writing to bufferedLines; building up a giant stringstream that holds all the source code (from /shaders) 
    }

    string shaderSource = bufferedLines.str(); // shaders source code
    const char* shaderSrc = shaderSource.c_str(); // convert to a C string
    bufferedLines.str(""); // clears the bufferedLines stream (replaces with empty string)
    file.close();

    unsigned int shaderModule = glCreateShader(module_type); // create shader module
    glShaderSource(shaderModule, 1, &shaderSrc, NULL); // attach source code to shader module
    glCompileShader(shaderModule); // compile the shader module/source code

    // The compilation will sometimes fail, so need to check if it was successful
    int success;
    glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);
    if(!success) {
        char errorLog[1024];
        glGetShaderInfoLog(shaderModule, 1024, NULL, errorLog);
        cout << "Shader Module compilation error:\n" << errorLog << endl;
    }

    return shaderModule;
} 