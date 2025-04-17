#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;


void main()
{
    //此处TexCoords为方向向量，所以天空盒子的大小是什么样的并不重要，纹理都能够正常采样
    FragColor = texture(skybox,TexCoords);
}

