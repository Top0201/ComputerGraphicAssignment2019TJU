#ifndef GEOMETRY_SPHERE_H
#define GEOMETRY_SPHERE_H

#include <vector>
#include <iostream>
#include <glad/glad.h>
using namespace std;

class Sphere
{
public:
	Sphere(float radius = 1.0f, int sectorCount = 36, int stackCount = 18);
	~Sphere(){}

	//getter
	float getRadius() const { return radius; }
	int getSector() const { return sectorCount; }
	int getStack() const { return stackCount; }

	//setter
	void set(float r, int sector, int stack);
	void setRadius(float r);
	void setSector(int sector);
	void setStack(int stack);
	// for vertex data
	unsigned int getVertexCount() const { return (unsigned int)vertices.size() / 3; }
	unsigned int getNormalCount() const { return (unsigned int)normals.size() / 3; }
	unsigned int getTexCoordCount() const { return (unsigned int)texture.size() / 2; }
	unsigned int getIndexCount() const { return (unsigned int)indices.size(); }

	unsigned int getVertexSize() const { return (unsigned int)vertices.size() * sizeof(float); }
	unsigned int getNormalSize() const { return (unsigned int)normals.size() * sizeof(float); }
	unsigned int getTexCoordSize() const { return (unsigned int)texture.size() * sizeof(float); }
	unsigned int getIndexSize() const { return (unsigned int)indices.size() * sizeof(unsigned int); }
	
	const float* getVertices() const { return vertices.data(); }
	const float* getNormals() const { return normals.data(); }
	const float* getTexCoords() const { return texture.data(); }
	const unsigned int* getIndices() const { return indices.data(); }
	

	// for interleaved vertices: V/N/T
	unsigned int getInterleavedVertexCount() const { return getVertexCount(); }    // # of vertices
	unsigned int getInterleavedVertexSize() const { return (unsigned int)interleavedVertices.size() * sizeof(float); }    // # of bytess
	const float* getInterleavedVertices() const { return interleavedVertices.data(); }


private:
	float radius;
	int sectorCount;
	int stackCount;
	vector<float> vertices;
	vector<float> normals;
	vector<float> texture;
	vector<unsigned int> indices;

	//interleaved
	vector<float> interleavedVertices;

	//functions
	void updateRadius();
	void buildVerticesSmooth();
	void buildInterleavedVertices();
	void clearArrays();
	void addVertex(float x, float y, float z);
	void addNormal(float x, float y, float z);
	void addTexture(float s, float t);
	void addIndices(unsigned int i1, unsigned int i2, unsigned int i3);
	/*vector<float> computeFaceNormal(float x1, float y1, float z1,
		float x2, float y2, float z2,
		float x3, float y3, float z3);*/



};



#endif // !SPHERE_H

