#include "GameObject.h"

#include "IComponent.h"
#include "TransformComponent.h"

CGameObject::CGameObject()
{
	CBaseComponent::CBaseComponent();
	m_pTransform=new CTransformComponent();
	m_pTransform->setParent(this);
	m_Components.push_back(m_pTransform);
	m_ComponentMap[m_pTransform->getName()]=m_pTransform;
}

CGameObject::~CGameObject()
{
	vector<IComponent*>::iterator iter=m_Components.begin();
	while(iter!=m_Components.end())
	{
		if ((*iter))
		{
			delete (*iter);
			(*iter)=NULL;
			iter=m_Components.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}

void CGameObject::addComponent(IComponent * pComponent)
{
		m_Components.push_back(pComponent);
		m_ComponentMap[pComponent->getName()]=pComponent;
		pComponent->setParent(this);
};
