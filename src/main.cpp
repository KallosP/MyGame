#include "config.h"
#include "triangle_mesh.h"
#include "material.h"
//#include "linear_algebra.h"

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
    Material* material = new Material("../img/orange.jpg");
    Material* mask = new Material("../img/mask.jpg");

    unsigned int shader = make_shader(
        "shaders/vertex.txt",
        "shaders/fragment.txt"
    );

    // set texture units
    glUseProgram(shader);
    glUniform1i(glGetUniformLocation(shader, "material"), 0);
    glUniform1i(glGetUniformLocation(shader, "mask"), 1);

    glm::vec3 quad_position = {-0.2f, 0.4f, 0.0f};
    glm::vec3 camera_pos = {-5.0f, 0.0f, 3.0f};
    glm::vec3 camera_target = {0.0f, 0.0f, 0.0f};
    glm::vec3 up = {0.0f, 0.0f, 1.0f};
    unsigned int model_location = glGetUniformLocation(shader, "model"); // query the shader program for handle of "model" (allows you to access/work with the model variable defined in the shader program) (the variable doesn't exist on the CPU since the shader program is running on the GPU)
    unsigned int view_location = glGetUniformLocation(shader, "view"); // same as above but for 'view'
    unsigned int proj_location = glGetUniformLocation(shader, "projection"); // same as above but for 'view'

    glm::mat4 view = glm::lookAt(camera_pos, camera_target, up);
    glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));

    glm::mat4 projection = glm::perspective(
        45.0f, 640.f/480.0f, 0.1f, 10.0f
    );
    glUniformMatrix4fv(proj_location, 1, GL_FALSE, glm::value_ptr(projection));

    // enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glm::mat4 model = glm::mat4(1.0f); // creates identity matrix (1.0f is inserted on the diagonal of the matrix)
        model = glm::translate(model, quad_position);
        model = glm::rotate(model, (float)glfwGetTime(), {0.0f, 0.0f, 1.0f});
        glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model)); // glUniformMatrix4fv -> glUniform 4 float vector

        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shader);
        material->use(0);
        mask->use(1);
        triangle->draw();
        glfwSwapBuffers(window);
    }

    glDeleteProgram(shader);
    delete triangle;
    delete material;
    delete mask;
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