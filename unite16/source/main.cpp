#include <winInit.hpp>
#include <string>
#include <cmath>
#include <shader.hpp>
#include <camera.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// #include <fstream>
using std::string;
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;


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
    glfwSetCursorPosCallback(window.get(),mouse_callback);
    glfwSetScrollCallback(window.get(), scroll_callback);
    // tell GLFW to capture our mouse
    glfwSetInputMode(window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;  
    }
    Shader shader("../shaderSource/stencilTest.vs","../shaderSource/stencilTest.fs");
    Shader shaderSingleColor("../shaderSource/stencilTestBord.vs","../shaderSource/stencilTestBord.fs");
#pragma region 顶点数据
    //此处数据为正面顺时针旋转
    float cubeVertices[] = {
        // back face
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // bottom-left
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  // bottom-right
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   // top-right
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   // top-right
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,  // top-left
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // bottom-left
        // front face
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,   // top-right
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,  // bottom-right
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,   // top-right
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,  // top-left
        // left face
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,   // top-right
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-left
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,  // top-left
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-left
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,   // top-right
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  // bottom-right
                                         // right face
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,    // top-left
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   // top-right
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  // bottom-right
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  // bottom-right
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,   // bottom-left
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,    // top-left
        // bottom face
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // top-right
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,   // bottom-left
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,  // top-left
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,   // bottom-left
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // top-right
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  // bottom-right
        // top face
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top-left
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,  // top-right
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,   // bottom-right
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,   // bottom-right
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,  // bottom-left
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f  // top-left
    };
    float planeVertices[] = {
        // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
         5.0f, -0.5f, -5.0f,  2.0f, 2.0f								
    };
#pragma endregion
    // first, configure the cube's VAO (and VBO)
    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
    // plane VAO
    unsigned int planeVAO, planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    unsigned int cubeTexture = loadTexture("../../resource/marble.jpg",true);
    unsigned int floorTexture = loadTexture("../../resource/metal.png",true);
    shader.use();
    shader.setInt("texture",0);

    glEnable(GL_DEPTH_TEST);//开启深度测试
    glDepthFunc(GL_LESS);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL,1,0xFF);//模板值不为1就通过模板测试
    glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);//深度与把模板测试通过后的更新行为是glStencilFunc ref替换
    while (!glfwWindowShouldClose(window.get()))
    {
        //calculate current time
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window.get());
        glClearColor(0,0,0,1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        //bind Texture in texture target 
        shader.use();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        shader.setMat4("view",view);
        shader.setMat4("projection",projection);
        
        shaderSingleColor.use();
        model = glm::mat4(1);
        view = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(camera.Zoom),(float)SCR_WIDTH/(float)SCR_HEIGHT,1.0f,100.0f);
        shaderSingleColor.setMat4("view",view);
        shaderSingleColor.setMat4("projection",projection);
        
        glStencilMask(0x00);
        // floor
        shader.use();
        glBindVertexArray(planeVAO);
        glBindTexture(GL_TEXTURE_2D, floorTexture);//将地板纹理绑定上纹理单元0，为画地板使用
        shader.setMat4("model", glm::mat4(1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        // 开启面剔除
        glEnable(GL_CULL_FACE);
        // 剔除正面（正面为顺时针）所以要改一下
        glFrontFace(GL_CW);
        glCullFace(GL_FRONT);
        //先画物理记录模板值
        glStencilFunc(GL_ALWAYS,1,0xFF);
        glStencilMask(0xFF);
        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,cubeTexture);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //关闭面剔除
        glDisable(GL_CULL_FACE);
        //Drawing border
        glStencilFunc(GL_NOTEQUAL,1,0xFF);
        glStencilMask(0x00);//禁用模板写入操作
        glDisable(GL_DEPTH_TEST);//目前测试结果来看，禁用深度测试会将物体背面，即看不见的边框画出来，而开启深度测试则只会画出看得见的边框
        shaderSingleColor.use();
        float scale = 1.1f;
        glBindVertexArray(cubeVAO);
        // glBindTexture(GL_TEXTURE_2D, cubeTexture);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        model = glm::scale(model, glm::vec3(scale, scale, scale));
        shaderSingleColor.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(scale, scale, scale));
        shaderSingleColor.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);
        glStencilMask(0xFF);//如果不开启那么在下一次循环当中glClear()也是清楚模板缓冲的
        // glStencilFunc(GL_ALWAYS,0,0xFF);
        glEnable(GL_DEPTH_TEST);

        glfwSwapBuffers(window.get());
        glfwPollEvents();
    }
    glDeleteVertexArrays(1,&cubeVAO);
    glDeleteVertexArrays(1, &planeVAO);
    glDeleteBuffers(1,&cubeVBO);
    glDeleteBuffers(1,&planeVBO);

    glfwTerminate();
    return 0;
}
