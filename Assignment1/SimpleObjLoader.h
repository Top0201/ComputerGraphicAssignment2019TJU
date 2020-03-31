#ifndef SIMPLE_OBJ_LOADER_H_
#define SIMPLE_OBJ_LOADER_H_

#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "shader.h"

using namespace std;

//顶点属性结构体
struct Vertex
{
	glm::vec3 position;
	//法向量初始化为原点
	glm::vec3 vNormal=glm::vec3(0.0f, 0.0f, 0.0f);
};

//面数据结构体
struct FaceData
{
	//组成三角面片的下标
	vector<long long> formedIndex;
	//面的法向量
	glm::vec3 fNormal;
};

//绘制mesh类
class Mesh
{
public:
	vector<Vertex> vertexData;
	GLuint VBO, VAO;
	
	Mesh() {

	}

	//构造函数
	Mesh(const vector<Vertex>& vertexData)
	{
		//绑定顶点数据
		this->vertexData = vertexData;
		this->setupBuffers();

	}

	~Mesh()
	{
		glDeleteVertexArrays(1, &this->VAO);
		glDeleteBuffers(1, &this->VBO);

	}

	void setupBuffers()
	{
		cout << vertexData.size();
		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);

		glBindVertexArray(this->VAO);
		glBindBuffer(GL_ARRAY_BUFFER,this->VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * this->vertexData.size(),
			&this->vertexData[0], GL_STATIC_DRAW);

		//顶点位置属性
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		//顶点法向量属性 ,偏移3个浮点数位置
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3*sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(1);

		//unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	
	}
	//绘制模型函数
	void draw(Shader& shader)
	{
		shader.Use();
		glBindVertexArray(this->VAO);
		glDrawArrays(GL_TRIANGLES, 0, this->vertexData.size());

		//unbind
		glBindVertexArray(0);
		glUseProgram(0);
	}
};
//用于加载obj文件的类
class ObjLoader
{
public:
	static bool loadObjFile(const char* path,
		vector<Vertex> &vertexData)
	{
		ifstream file(path);
		//读取文件不成功
		if (!file) {
			cout << "Could not open obj file:" << path << " for reading."
				<< endl;

			return false;

		}

		vector<glm::vec3> allVertices;
		vector<FaceData> faceData;

		string line;
		while (getline(file, line))
		{
			//顶点数据
			if (line.substr(0, 1) == "v") {
				istringstream s(line.substr(2));
				glm::vec3 v;
				s >> v.x;
				s >> v.y;
				s >> v.z;
				allVertices.push_back(v);

			}
			//面数据,一行有三个顶点的index信息
			else if (line.substr(0, 1) == "f") {
				istringstream index(line.substr(2));
				long long v1, v2, v3;
				index >> v1; index >> v2; index >> v3;
				FaceData face;
				//加入顶点下标
				face.formedIndex.push_back(v1-1);
				face.formedIndex.push_back(v2-1);
				face.formedIndex.push_back(v3-1);

				//计算面法向量
				glm::vec3 line1 = allVertices[v1 - 1] - allVertices[v2 - 1];
				glm::vec3 line2 = allVertices[v1 - 1] - allVertices[v3 - 1];
				face.fNormal = glm::normalize(glm::cross(line1, line2));

				faceData.push_back(face);
				
			}

			//注释忽略
			else if (line[0] == '#') {

			}

		}

		//得到绘制mesh的顶点数据
		for (int i = 0; i < faceData.size(); i++)
		{
			vector<long long>index = faceData[i].formedIndex;
			for (int j = 0; j < index.size(); j++)
			{
				Vertex vertex;
				glm::vec3 ver;
				long long temp = index[j];
				ver.x = allVertices[temp].x;
				ver.y = allVertices[temp].y;
				ver.z = allVertices[temp].z;

				vertex.position = ver;

				//计算顶点的法向量,使用该顶点的面向量累计加和
				vertex.vNormal += faceData[i].fNormal;

				vertexData.push_back(vertex);

			}
		}
		//normalize the vertex's normal
		for (int i = 0; i < vertexData.size(); i++)
		{
			glm::vec3 nor = vertexData[i].vNormal;
			vertexData[i].vNormal = glm::normalize(nor);
		}
		 
		return true;
	}

	
};


#endif // !SIMPLE_OBJ_LOADER_H_

