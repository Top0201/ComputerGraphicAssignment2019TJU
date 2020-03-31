#version 410 core

struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform sampler2D ourTexture;
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    //ambient
    vec3 ambient=light.ambient*material.ambient;

    //diffuse
    vec3 norm=normalize(Normal);
    //light direction
    vec3 lightDir=normalize(light.position-FragPos);
    //the angle between the normal and light direction
    float diff=max(dot(norm,lightDir),0.0);
    vec3 diffuse=light.diffuse*(diff*material.diffuse);

    //specular
    //view's direction
    vec3 viewDir=normalize(viewPos-FragPos);
    //the reflection's direction
    vec3 reflectDir=reflect(-lightDir,norm);
    float spec=pow(max(dot(viewDir,reflectDir),0.0),
        material.shininess);
    vec3 specular=light.specular*(spec*material.specular);

    vec3 result=ambient+diffuse+specular;

   
    color= texture(ourTexture, TexCoord)*vec4(result, 1.0f);
    
}
