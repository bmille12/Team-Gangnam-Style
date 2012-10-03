#pragma once

#include "IComponent.h"

#include <Physics/Dynamics/Entity/hkpRigidBody.h>

class CBodyComponent:public CBaseComponent
{
public:
	CBodyComponent();
	~CBodyComponent();
private:
	hkpRigidBody* m_pRigidBody;
};