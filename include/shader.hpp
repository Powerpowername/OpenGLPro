#pragma once
#include <winInit.hpp>
//类内定义函数会被解释为内联函数，会在编译阶段就将代码嵌入代码块，所以编译阶段会慢一点，但是在类外定义则是会更简洁，但是在程序执行过程中会有调用函数的开销
/*
类内定义（内联成功）：
若函数被成功内联，在程序执行时就不会有常规的函数调用开销。
因为没有函数调用时的压栈、跳转、出栈等操作，程序可以直接执行嵌入的函数体代码，这样能提高程序的执行效率。
类外定义：
类外定义的函数在程序执行时，每次调用都会有函数调用的开销。
当调用一个函数时，程序需要保存当前的执行上下文（如寄存器的值），
将控制权转移到函数的入口地址，执行函数体，然后再恢复执行上下文并返回。
这些操作会带来一定的性能损耗。
*/
class Shader
{
private:
    void checkCompileErrors(unsigned int shader,std::string type)
    {
        int success;
        char infoLog[1024];
        if(type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader,GL_LINK_STATUS,&success);
            if(!success)
            {
                glGetProgramInfoLog(shader,1024,NULL,infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
public:
    unsigned int ID;//shader ID

    Shader(const std::string vertexFilePath,const std::string fragmentFilePath)
    {
        std::string ver = readFile(vertexFilePath);
        std::string frag = readFile(fragmentFilePath);
        const char* vertexShderSource = ver.c_str();
        const char* fragmentShaderSource = frag.c_str();
        // std::cout<<"vertexShderSource :"<< vertexShderSource <<std::endl;
        // std::cout<<"fragmentShaderSource :"<< fragmentShaderSource <<std::endl;
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader,1,&vertexShderSource,NULL);
        glCompileShader(vertexShader);
        checkCompileErrors(vertexShader,"VERTEX");
        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL);
        glCompileShader(fragmentShader);
        checkCompileErrors(fragmentShader,"FRAGMENT");

        ID = glCreateProgram();
        glAttachShader(ID,vertexShader);
        glAttachShader(ID,fragmentShader);
        glLinkProgram(ID);
        checkCompileErrors(ID,"PROGRAM");

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);        
        
    }
    ~Shader()
    {
        glDeleteProgram(ID);
    }

    void use()
    {
        glUseProgram(ID);
    }

    void setBool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setFloat(const std::string &name,float value1,float value2,float value3,float value4)
    {
        glUniform4f(glGetUniformLocation(ID, name.c_str()),value1,value2,value3,value4);
    }



    // ------------------------------------------------------------------------
    void setVec2(const std::string &name, const glm::vec2 &value) const
    { 
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec2(const std::string &name, float x, float y) const
    { 
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y); 
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string &name, const glm::vec3 &value) const
    { 
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec3(const std::string &name, float x, float y, float z) const
    { 
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); 
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string &name, const glm::vec4 &value) const
    { 
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec4(const std::string &name, float x, float y, float z, float w) const
    { 
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w); 
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string &name, const glm::mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string &name, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
};
