#include <winInit.hpp>
#include <string>
#include <cmath>
#include <shader.hpp>
#include <camera.hpp>
#include <model.hpp>
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

    // 添加GLAD初始化
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;  
    }
    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);
    //E:/OpenGLPro/unite25
    Shader shader("E:/OpenGLPro/unite25/shaderSource/shader.vs","E:/OpenGLPro/unite25/shaderSource/shader.fs");
    // load models
    // -----------
    Model ourModel("E:/OpenGLPro/resource/objects/backpack/backpack.obj");
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window.get()))
    {
         // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window.get());

        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // don't forget to enable shader before setting uniforms
        shader.use();

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        shader.setMat4("model", model);
        ourModel.Draw(shader);


        glfwSwapBuffers(window.get());
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}