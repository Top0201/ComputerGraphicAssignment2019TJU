#version 410 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord;

uniform mat4 model;//模型变换矩阵
uniform mat4 view;//模型视图矩阵
uniform mat4 projection;//投影变换矩阵

void main()
{
    
   gl_Position = projection * view *  model * vec4(position, 1.0f);
   FragPos = vec3(model * vec4(position, 1.0f));
   Normal = mat3(transpose(inverse(model))) * normal;  
   TexCoord = vec2(texCoord.x, 1.0f - texCoord.y);
}
