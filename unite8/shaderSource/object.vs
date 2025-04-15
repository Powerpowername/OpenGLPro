#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	//转到世界坐标系中进行光照计算
	FragPos = vec3(model * vec4(aPos,1));
	Normal = mat3(transpose(inverse(model))) * aNormal;

	gl_Position = projection * view * vec4(FragPos,1);

}
