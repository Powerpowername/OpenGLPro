#include <winInit.hpp>
#include <string>
// #include <fstream>
using std::string;
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    string vertexShaderSource = readFile("D:/opPro/unite2/shaderSource/vertexShader.vs");
    string fragmentShaderSoure = readFile("D:/opPro/unite2/shaderSource/fragmentShader.fs");
    std::unique_ptr<GLFWwindow,windowDeleter> window(windowInit(3,3,SCR_WIDTH,SCR_HEIGHT));

    if(window.get() == NULL)
    {
        std::cout<<"Failed to create GLFW window"<<std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window.get());
    glfwSetFramebufferSizeCallback(window.get(),framebuffer_size_callback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;  
    }

    return 0;
}
