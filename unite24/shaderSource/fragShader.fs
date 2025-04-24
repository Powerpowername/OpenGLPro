#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D depthMap;
uniform float near_plane;
uniform float far_plane;

float LinearizeDepth(float depth)
{
    /*
    在 OpenGL 中，深度缓冲区存储的深度值范围是 [0, 1]，
    而归一化设备坐标（NDC）下的深度值范围是 [-1, 1]。
    这行代码的作用就是把 [0, 1] 范围的深度值转换到 [-1, 1] 范围。
    */
    float z = depth * 2.0 - 1.0;
    //可视化深度值时，透视投影的深度值需要线性化
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));

}

void main()
{
    float depthValue = texture(depthMap,TexCoords).r;
    // FragColor = vec4(vec3(LinearizeDepth(depthValue) / far_plane), 1.0); // perspective
    FragColor = vec4(vec3(depthValue),1.0);// orthographic
}
