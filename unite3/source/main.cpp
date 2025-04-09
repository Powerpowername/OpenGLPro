#include <winInit.hpp>
#include <string>
#include <cmath>
#include <shader.hpp>
// #include <fstream>
using std::string;
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
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

    Shader ourShader("D:/opPro/unite3/shaderSource/vertexShader.vs","D:/opPro/unite3/shaderSource/fragmentShader.fs");
    
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
       -0.5f, -0.5f, 0.0f,  // bottom left
       -0.5f,  0.5f, 0.0f   // top left  
    }; 

    unsigned int indices[]{ // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle   
    };

    //配置顶点缓冲对象VBO与数组缓冲对象VAO
    unsigned int VAO,VBO,EBO;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);


    while (!glfwWindowShouldClose(window.get()))
    {
        // get current time
        double timeValue = glfwGetTime();
        float greenValue = static_cast<float>(sin(timeValue) / 2.0 + 0.5);

        processInput(window.get());
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        // using shader program
        ourShader.use();
        // std::cout<<"传值给uniform"<<greenValue<<std::endl;
        ourShader.setFloat("ourColor", 0, greenValue, 0, 1);
        // indicating the method of drawing
        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES,0,3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 最后一个参数为偏移量

        glfwSwapBuffers(window.get());
        glfwPollEvents();
    }
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);

    glfwTerminate();
    return 0;
}
