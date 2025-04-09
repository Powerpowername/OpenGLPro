#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>
#include <fstream>
void framebuffer_size_callback(GLFWwindow* window,int width,int height)
{
    glViewport(0,0,width,height);
}
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window,true);
}
/// @brief 配置glfw版本号，创建并返回一个窗口指针
/// @param glfwVerMajor 
/// @param glfwVerMinor 
/// @return 
GLFWwindow* windowInit(int glfwVerMajor,int glfwVerMinor,int width,int height)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,glfwVerMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,glfwVerMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width,height,"unite1",NULL,NULL);//此处为堆区分配的内存

    return window;
}

struct windowDeleter
{
    void operator()(GLFWwindow* window) const
    {
        if(window)
        {
            glfwDestroyWindow(window);
        }
    }
};


std::string readFile(const std::string& filePath)
{
    std::fstream file(filePath,std::ios::in);
    if(!file.is_open())
    {   
        std::cout<<"无法打开文件"<<std::endl;
        return "";
    }
    file.seekg(0,std::ios::end);//将文件读取指针移动到文件末尾，方便计算文件大小
    std::streamsize size = file.tellg();//读取文件大小，单位为char
    file.seekg(0,std::ios::beg);

    std::string content(size,'\0');
    file.read(&content[0],size);
    
    return content;
}