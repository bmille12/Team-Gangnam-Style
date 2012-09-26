#include "IComponent.h"

#include <D3D10.h>
#include <D3DX10.h>

class CTransformComponent:public CBaseComponent
{
public:
	CTransformComponent()
	{
		m_vecPosition=D3DXVECTOR3(0.0f,0.0f,0.0f);
		m_vecRotation=D3DXVECTOR3(0.0f,0.0f,0.0f);
		m_vecScale=D3DXVECTOR3(1.0f,1.0f,1.0f);
		D3DXMatrixIdentity(&m_matTranslate);
		D3DXMatrixIdentity(&m_matRotation);
		D3DXMatrixIdentity(&m_matScale);
		D3DXMatrixIdentity(&m_matWorld);
		D3DXQuaternionIdentity(&m_quatRotation);
		m_strName="TransformComponent";
	};
	virtual ~CTransformComponent(){};

	void update(float elapsedTime)
	{
		D3DXQuaternionRotationYawPitchRoll(&m_quatRotation,m_vecRotation.x,m_vecRotation.y,m_vecRotation.z);
		D3DXMatrixRotationQuaternion(&m_matRotation,&m_quatRotation);

		D3DXMatrixTranslation(&m_matTranslate,m_vecRotation.x,m_vecRotation.y,m_vecRotation.z);
		D3DXMatrixScaling(&m_matScale,m_vecScale.x,m_vecScale.y,m_vecScale.z);

		m_matWorld=m_matScale*m_matRotation*m_matTranslate;
	};

	void render()
	{
	};

	void init()
	{
	};

	void setPosition(float x,float y,float z)
	{
		m_vecPosition=D3DXVECTOR3(x,y,z);
	};

	void setRotation(float x,float y,float z)
	{
		m_vecRotation=D3DXVECTOR3(x,y,z);
		
	};

	void setScale(float x,float y,float z)
	{
		m_vecScale=D3DXVECTOR3(x,y,z);
	};

	D3DXMATRIX& getWorld()
	{
		return m_matWorld;
	};
private:
	D3DXVECTOR3 m_vecPosition;
	D3DXVECTOR3 m_vecRotation;
	D3DXVECTOR3 m_vecScale;

	D3DXMATRIX m_matTranslate;
	D3DXMATRIX m_matRotation;
	D3DXMATRIX m_matScale;

	D3DXQUATERNION m_quatRotation;

	D3DXMATRIX m_matWorld;
};