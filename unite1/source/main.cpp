#include <iostream>
#include <memory>
#include <winInit.hpp>
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // glfwInit();
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    // glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    // GLFWwindow* window = glfwCreateWindow(SCR_WIDTH,SCR_HEIGHT,"unite1",NULL,NULL);
    // std::shared_ptr<GLFWwindow> win(windowInit(3,3,SCR_WIDTH,SCR_HEIGHT),windowDeleter());
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


    while(!glfwWindowShouldClose(window.get()))
    {
        processInput(window.get());
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window.get());
        glfwPollEvents();

    }
    glfwTerminate();
    return 0;
}



