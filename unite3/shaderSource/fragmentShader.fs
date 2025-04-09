#version 330 core
out vec4 FragColor;
uniform vec4 ourColor;

void main()
{
    FragColor = ourColor;
    // FragColor = vec4(1,1,1,1);

}
