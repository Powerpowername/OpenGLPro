#pragma once 
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>
#include <fstream>
#include <camera.hpp>
#include <stb_image.h>
#include <vector>
extern Camera camera;
extern float lastX;
extern float lastY;
extern bool firstMouse;
extern float deltaTime;	
/// @brief 配置glfw版本号，创建并返回一个窗口指针
/// @param glfwVerMajor 
/// @param glfwVerMinor 
/// @return 
GLFWwindow* windowInit(int glfwVerMajor,int glfwVerMinor,int width,int height,int multiSample = 1)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,glfwVerMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,glfwVerMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, multiSample);
    GLFWwindow* window = glfwCreateWindow(width,height,"window",NULL,NULL);//此处为堆区分配的内存
//    // 添加以下两行
// std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
// std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

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
    std::string content;
    try
    {
        //此处不用二进制打开会导致\r\n转\n，于是abc\r\n测出的长度是5，但是实际读出的只有4个于是就会产生failbit错误
        std::fstream file(filePath, std::ios::in | std::ios::binary);
        file.exceptions(std::fstream::failbit | std::fstream::badbit);
        if (!file.is_open())
        {
            std::cout << "无法打开文件" << std::endl;
            return "";
        }
        file.seekg(0, std::ios::end);        // 将文件读取指针移动到文件末尾，方便计算文件大小
        std::streamsize size = file.tellg(); // 读取文件大小，单位为char
        file.seekg(0, std::ios::beg);

        content.resize(size);
        file.read(&content[0], size);
        // 检查读取是否成功
        if (!file)
        {
            std::cerr << "文件读取不完整，可能存在错误。" << std::endl;
            file.clear(); 
        }
    }
    catch (const std::fstream::failure &e)
    {
        std::cerr << "文件读取发生错误: " << e.what() << std::endl;
    }

    return content;
}

#pragma region 窗口事件
void framebuffer_size_callback(GLFWwindow* window,int width,int height)
{
    glViewport(0,0,width,height);
}
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window,true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window,double xposIn,double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    camera.ProcessMouseMovement(xoffset,yoffset);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
#pragma endregion

/// @brief 查看glad当中最新的错误
void gladCheckerror()
{
    int error = glGetError();
    if(error == GL_NO_ERROR)
        std::cout<<"No Error"<<std::endl;
    else if(error == GL_INVALID_OPERATION)
        std::cout<<"invalid opreation"<<std::endl;
    else 
        std::cout<<"unKnown Error"<<std::endl;
}
/// @brief 
/// @param path 图片路径
/// @param reverse 判断是否延x轴翻转
/// @return 纹理对象的ID
unsigned int loadTexture(const char*  path,bool reverse)
{
    unsigned int textureID;
    glGenTextures(1,&textureID);
    int width,height,nrComponents;
    stbi_set_flip_vertically_on_load(reverse); 
    unsigned char* data = stbi_load(path,&width,&height,&nrComponents,0);
    if(data)
    {
        GLenum format;
        if(nrComponents == 1)
            format = GL_RED;
        else if(nrComponents == 3)
            format = GL_RGB;
        else if(nrComponents == 4)
            format = GL_RGBA;
        glBindTexture(GL_TEXTURE_2D,textureID);
        glTexImage2D(GL_TEXTURE_2D,0,format,width,height,0,format,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);//生成多级渐近纹理

        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    //此处可以看出纹理对象开辟的内存（glGenTextures）实在堆区开辟的，这个id是在栈区的对象，被return之后就会被销毁，但是这块是传值出去的，所以不要紧
    return textureID;
}
