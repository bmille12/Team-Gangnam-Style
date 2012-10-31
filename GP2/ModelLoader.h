#pragma once

#define FBXSDK_NEW_API

#include <string>
#include <d3d10.h>
#include <d3dx10.h>
#include "Vertex.h"

class CGeometryComponent;

using namespace std;


class CModelLoader
{
public:
	CModelLoader();
	~CModelLoader();

	CGeometryComponent * loadModelFromFile(ID3D10Device *pDevice,const string& filename);
	CGeometryComponent * createCube(ID3D10Device *pDevice,float width, float height, float length);  
private:
	CGeometryComponent * loadFbxModelFromFile(ID3D10Device *pDevice,const string& filename);

	void computeTangents(Vertex *pVerts,int vertexCount);
};