#include "AudioListenerComponent.h"

#include "TransformComponent.h"
#include "GameObject.h"

CAudioListenerComponent::CAudioListenerComponent()
{
	m_vecForward=D3DXVECTOR3(0.0f,0.0f,1.0f);
	m_vecVelocity=D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_vecLastPosition=D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_pAudioSystem=NULL;
	m_strName="AudioListenerComponent";
}

CAudioListenerComponent::~CAudioListenerComponent()
{

}

void CAudioListenerComponent::update(float elapsedTime)
{
	CTransformComponent *pTransform=getParent()->getTransform();

	FMOD_VECTOR pos;
	FMOD_VECTOR vel;
	FMOD_VECTOR forward;
	FMOD_VECTOR up;

	up.x=0.0f;
	up.y=1.0f;
	up.z=0.0f;

	pos.x=pTransform->getPosition().x;
	pos.y=pTransform->getPosition().y;
	pos.z=pTransform->getPosition().z;
	
	m_vecForward=pTransform->getPosition()-m_vecLastPosition;
	D3DXVec3Normalize(&m_vecForward,&m_vecForward);
	forward.x=m_vecForward.x;
	forward.y=m_vecForward.y;
	forward.z=m_vecForward.z;

	m_vecVelocity=(pTransform->getPosition()-m_vecLastPosition)*1000/elapsedTime;
	vel.x=m_vecVelocity.x;
	vel.y=m_vecVelocity.y;
	vel.z=m_vecVelocity.z;

	m_pAudioSystem->set3DListenerAttributes(0,&pos,&vel,&forward,&up);
	
	m_vecLastPosition=pTransform->getPosition();
}