#pragma once

#include "IComponent.h"

#include <D3D10.h>
#include <D3DX10.h>

#include <string>

using namespace std;

class CMaterialComponent:public CBaseComponent
{
public: 
	CMaterialComponent();

	virtual ~CMaterialComponent();

	void SetRenderingDevice(ID3D10Device *pDevice)
	{
		m_pD3D10Device=pDevice;
	};

	void setEffectFilename(const string& name)
	{
		m_EffectName=name;
	};

	void setTechniqueName(const string& name)
	{
		m_TechniqueName=name;
	};

	virtual void init();

	void loadEffectFromFile(const string &name);
	void loadEffectFromMemory(const char * pData);
	void createVertexLayout();

	UINT getNumberOfPasses()
	{
		return m_TechniqueDesc.Passes;
	};

	void applyPass(UINT currentIndex)
	{
		m_pCurrentPass=m_pTechnique->GetPassByIndex(currentIndex);
		m_pCurrentPass->Apply(0);
	};

	void setWorldMatrix(float * pMatrix)
	{
		m_pWorldMatrixVariable->SetMatrix(pMatrix);
	};

	void setProjectionMatrix(float * pMatrix)
	{
		m_pProjectionMatrixVariable->SetMatrix(pMatrix);
	};

	void setViewMatrix(float * pMatrix)
	{
		m_pViewMatrixVariable->SetMatrix(pMatrix);
	};

	void bindVertexLayout()
	{
		m_pD3D10Device->IASetInputLayout(m_pVertexLayout);
	};
protected:
	ID3D10Device * m_pD3D10Device;
	ID3D10Effect*           m_pEffect;
	ID3D10EffectTechnique*  m_pTechnique;
	ID3D10EffectPass *m_pCurrentPass;
	ID3D10InputLayout*      m_pVertexLayout;

	string m_EffectName;
	string m_TechniqueName;

	D3D10_TECHNIQUE_DESC m_TechniqueDesc;

	ID3D10EffectMatrixVariable * m_pViewMatrixVariable;
	ID3D10EffectMatrixVariable * m_pProjectionMatrixVariable;
	ID3D10EffectMatrixVariable * m_pWorldMatrixVariable;
};