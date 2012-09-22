#include "GameObject.h"

#include "IComponent.h"
#include "TransformComponent.h"

CGameObject::CGameObject()
{
	m_pTransform=new CTransformComponent();
	m_Components.push_back(m_pTransform);
	m_ComponentMap[m_pTransform->getName()]=m_pTransform;
}