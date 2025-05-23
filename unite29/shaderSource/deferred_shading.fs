#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;//保存的是世界坐标系中的坐标
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

struct Light {
    vec3 Position;
    vec3 Color;

    float Linear;
    float Quadratic;
    float Radius;
};

const int NR_LIGHTS = 32;
uniform Light lights[NR_LIGHTS];
uniform vec3 viewPos;

void main()
{
    vec3 FragPos = texture(gPosition,TexCoords).rgb;
    vec3 Normal = texture(gNormal,TexCoords).rgb;
    vec3 Diffuse = texture(gAlbedoSpec,TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;
    
    vec3 lighting = Diffuse * 0.1f;
    vec3 viewDir = normalize(viewPos - FragColor);
    for(int i = 0;i < NR_LIGHTS;i++)
    {
        float distance = normalize(lights[i].Position - FragPos);
        if(distance < lights[i].Radius)
        {
            vec3 lightDir = normalize(lights[i].Position - FragPos);
            vec3 diffuse = max(dot(Normal,lightDir),0.0) * Diffuse * lights[i].Color;
            vec3 halfwayDir = normalize(lightDir + viewDir);
            float spec = pow(max(dot(Normal,halfwayDir,0.0f),0.0f),16.0);
            float specular = lights[i].Color * spec * Specular;
            // attenuation
            float attenuation = 1.0 / (1.0 + lights[i].Linear * distance + lights[i].Quadratic * distance * distance);
            diffuse *= attenuation;
            specular *= attenuation;
            lighting += diffuse + specular;
        }
        
    }
    FragColor = vec4(Light,1.0);
}
