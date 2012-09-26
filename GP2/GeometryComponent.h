#pragma once

#include "IComponent.h"
#include "Vertex.h"

#include <D3D10.h>
#include <D3DX10.h>

#include <vector>


using namespace std;

class CGeometryComponent:public CBaseComponent
{
public:
	CGeometryComponent();
	~CGeometryComponent();

	void SetRenderingDevice(ID3D10Device *pDevice)
	{
		m_pD3D10Device=pDevice;
	};

	void init();

	void addVertex(Vertex vert);
	void addIndex(int index);

	void createVertexBuffer();
	void createIndexBuffer();

	void bindBuffers();

	int getNumberOfIndices()
	{
		return m_Indices.size();
	};

	int getNumberOfVertices()
	{
		return m_Vertices.size();
	};
private:
	ID3D10Device *m_pD3D10Device;
	ID3D10Buffer *m_pVertexBuffer;
	ID3D10Buffer *m_pIndexBuffer;

	vector<Vertex> m_Vertices;
	vector<int> m_Indices;
};