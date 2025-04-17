#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
    //传入的坐标就是天空盒在世界坐标系的坐标，天空盒不需要移动，所以没有mode矩阵可以减少计算量，
    //同时传入的坐标的绝对值越大，物体越小，视野内的物体越多
    vec4 pos = projection * view * vec4(aPos,1.0);
    //片段着色器之后的操作是透视除法，在上面的操作中我们已经算出了所需要的坐标，此时z只是起到深度值的作用（当然z值需要再经过透视除法才是真正的深度值）
    gl_Position = pos.xyww;//z值直接设置为w分量，这样z值就会永远是1，即最大深度值
}


