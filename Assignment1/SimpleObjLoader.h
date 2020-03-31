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

//�������Խṹ��
struct Vertex
{
	glm::vec3 position;
	//��������ʼ��Ϊԭ��
	glm::vec3 vNormal=glm::vec3(0.0f, 0.0f, 0.0f);
};

//�����ݽṹ��
struct FaceData
{
	//���������Ƭ���±�
	vector<long long> formedIndex;
	//��ķ�����
	glm::vec3 fNormal;
};

//����mesh��
class Mesh
{
public:
	vector<Vertex> vertexData;
	GLuint VBO, VAO;
	
	Mesh() {

	}

	//���캯��
	Mesh(const vector<Vertex>& vertexData)
	{
		//�󶨶�������
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

		//����λ������
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		//���㷨�������� ,ƫ��3��������λ��
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3*sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(1);

		//unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	
	}
	//����ģ�ͺ���
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
//���ڼ���obj�ļ�����
class ObjLoader
{
public:
	static bool loadObjFile(const char* path,
		vector<Vertex> &vertexData)
	{
		ifstream file(path);
		//��ȡ�ļ����ɹ�
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
			//��������
			if (line.substr(0, 1) == "v") {
				istringstream s(line.substr(2));
				glm::vec3 v;
				s >> v.x;
				s >> v.y;
				s >> v.z;
				allVertices.push_back(v);

			}
			//������,һ�������������index��Ϣ
			else if (line.substr(0, 1) == "f") {
				istringstream index(line.substr(2));
				long long v1, v2, v3;
				index >> v1; index >> v2; index >> v3;
				FaceData face;
				//���붥���±�
				face.formedIndex.push_back(v1-1);
				face.formedIndex.push_back(v2-1);
				face.formedIndex.push_back(v3-1);

				//�����淨����
				glm::vec3 line1 = allVertices[v1 - 1] - allVertices[v2 - 1];
				glm::vec3 line2 = allVertices[v1 - 1] - allVertices[v3 - 1];
				face.fNormal = glm::normalize(glm::cross(line1, line2));

				faceData.push_back(face);
				
			}

			//ע�ͺ���
			else if (line[0] == '#') {

			}

		}

		//�õ�����mesh�Ķ�������
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

				//���㶥��ķ�����,ʹ�øö�����������ۼƼӺ�
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

