#include <iostream>
#include "Sphere.h"

const int MIN_SECTOR = 3;
const int MIN_STACK = 2;

Sphere::Sphere(float r, int sector, int stack)
{
	set(r, sector, stack);
}

void Sphere::set(float r, int sector, int stack)
{
	this->radius = r;
	this->sectorCount = sector;
	if (sector < MIN_SECTOR)
		this->sectorCount = MIN_SECTOR;
	this->stackCount = stack;
	if (stack < MIN_STACK)
		this->stackCount = MIN_STACK;

	buildVerticesSmooth();

}

void Sphere::setRadius(float r)
{
	radius = r;
	updateRadius();
}

void Sphere::setSector(int sector)
{
	set(radius, sector, stackCount);
}

void Sphere::setStack(int stack)
{
	set(radius, sectorCount, stack);
}

//update vertex position only
void Sphere::updateRadius()
{
    float scale = sqrtf(radius * radius / (vertices[0] * vertices[0] + vertices[1] * vertices[1] + vertices[2] * vertices[2]));

    std::size_t i, j;
    std::size_t count = vertices.size();
    for(i = 0, j = 0; i < count; i += 3, j += 8)
    {
        vertices[i]   *= scale;
        vertices[i+1] *= scale;
        vertices[i+2] *= scale;

        // for interleaved array
        interleavedVertices[j]   *= scale;
        interleavedVertices[j+1] *= scale;
        interleavedVertices[j+2] *= scale;
    }
}



//dealloc vectors
void Sphere::clearArrays()
{
	std::vector<float>().swap(vertices);
	std::vector<float>().swap(normals);
	std::vector<float>().swap(texture);
	std::vector<unsigned int>().swap(indices);
}

//x=r*cos(u)*cos(v)
//y=r*cos(u)*sin(v)
//z=r*sin(u)
//u:stack(latitude) angle(-90<=u<=90)
//v:sector(longitude) angle(0<=v<=360)

void Sphere::buildVerticesSmooth()
{
	const float PI = 3.1415926f;

	// clear memory of prev arrays
	clearArrays();

	// vertex position
	float x, y, z, xy;    
	// normal
	float nx, ny, nz, lengthInv = 1.0f / radius;   
	// texCoord
	float s, t;                                     

	//经度步数
	float sectorStep = 2 * PI / sectorCount;
	//纬度步数
	float stackStep = PI / stackCount;
	//前进角度
	float sectorAngle, stackAngle;

	for (int i = 0; i <= stackCount; ++i)
	{
		stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
		xy = radius * cosf(stackAngle);             // r * cos(u)
		z = radius * sinf(stackAngle);              // r * sin(u)

		// add (sectorCount+1) vertices per stack
		// the first and last vertices have same position and normal, but different tex coords
		for (int j = 0; j <= sectorCount; ++j)
		{
			sectorAngle = j * sectorStep;           // starting from 0 to 2pi
			// vertex position
			x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
			y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
			addVertex(x, y, z);

			// normalized vertex normal
			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;
			addNormal(nx, ny, nz);

			// vertex tex coord between [0, 1]
			s = (float)j / sectorCount;
			t = (float)i / stackCount;
			addTexture(s, t);
		}
	}

	// indices
	//  k1--k1+1
	//  |  / |
	//  | /  |
	//  k2--k2+1
	unsigned int k1, k2;
	for (int i = 0; i < stackCount; ++i)
	{
		// beginning of current stack
		k1 = i * (sectorCount + 1);     
		// beginning of next stack
		k2 = k1 + sectorCount + 1;     

		for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding 1st and last stacks
			if (i != 0)
			{
				addIndices(k1, k2, k1 + 1);   // k1---k2---k1+1
			}
			if (i != (stackCount - 1))
			{
				addIndices(k1 + 1, k2, k2 + 1); // k1+1---k2---k2+1
			}

		}
	}

	// generate interleaved vertex array as well
	buildInterleavedVertices();
}

//generate interleaved vertices:v/n/t
void Sphere::buildInterleavedVertices()
{
	std::vector<float>().swap(interleavedVertices);

	std::size_t i, j;
	std::size_t count = vertices.size();
	for (i = 0, j = 0; i < count; i += 3, j += 2)
	{
		interleavedVertices.push_back(vertices[i]);
		interleavedVertices.push_back(vertices[i + 1]);
		interleavedVertices.push_back(vertices[i + 2]);

		interleavedVertices.push_back(normals[i]);
		interleavedVertices.push_back(normals[i + 1]);
		interleavedVertices.push_back(normals[i + 2]);

		interleavedVertices.push_back(texture[j]);
		interleavedVertices.push_back(texture[j + 1]);
	}
}


//add vertex to vertex array
void Sphere::addVertex(float x, float y, float z)
{
	vertices.push_back(x);
	vertices.push_back(y);
	vertices.push_back(z);
}

//add vertex normal to normal array
void Sphere::addNormal(float nx, float ny, float nz)
{
	normals.push_back(nx);
	normals.push_back(ny);
	normals.push_back(nz);
}

//add texture cord to texture array
void Sphere::addTexture(float s, float t)
{
	texture.push_back(s);
	texture.push_back(t);
}

//add triangle indices to array
void Sphere::addIndices(unsigned int i1, unsigned int i2, unsigned int i3)
{
	indices.push_back(i1);
	indices.push_back(i2);
	indices.push_back(i3);
}

//calculate every face's normal
//std::vector<float> Sphere::computeFaceNormal(float x1, float y1, float z1,  // v1
//	float x2, float y2, float z2,  // v2
//	float x3, float y3, float z3)  // v3
//{
//	const float EPSILON = 0.000001f;
//
//	std::vector<float> normal(3, 0.0f);     // default return value (0,0,0)
//	float nx, ny, nz;
//
//	// find 2 edge vectors: v1-v2, v1-v3
//	float ex1 = x2 - x1;
//	float ey1 = y2 - y1;
//	float ez1 = z2 - z1;
//	float ex2 = x3 - x1;
//	float ey2 = y3 - y1;
//	float ez2 = z3 - z1;
//
//	// cross product: e1 x e2
//	nx = ey1 * ez2 - ez1 * ey2;
//	ny = ez1 * ex2 - ex1 * ez2;
//	nz = ex1 * ey2 - ey1 * ex2;
//
//	// normalize only if the length is > 0
//	float length = sqrtf(nx * nx + ny * ny + nz * nz);
//	if (length > EPSILON)
//	{
//		// normalize
//		float lengthInv = 1.0f / length;
//		normal[0] = nx * lengthInv;
//		normal[1] = ny * lengthInv;
//		normal[2] = nz * lengthInv;
//	}
//
//	return normal;
//}
