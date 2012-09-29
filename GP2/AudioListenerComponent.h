#pragma once

#include "IComponent.h"

#include <D3D10.h>
#include <D3DX10.h>
#include <fmod.hpp>


class CAudioListenerComponent:public CBaseComponent
{
public:
	CAudioListenerComponent();
	~CAudioListenerComponent();

	void update(float elapsedTime);

	void setAudioSystem(FMOD::System * pAudioSystem)
	{
		m_pAudioSystem=pAudioSystem;
	};
private:
	D3DXVECTOR3 m_vecLastPosition;
	D3DXVECTOR3 m_vecVelocity;
	D3DXVECTOR3 m_vecForward;

	FMOD::System * m_pAudioSystem;
};