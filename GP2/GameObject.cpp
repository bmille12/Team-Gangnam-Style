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

	m_ComponentMap.clear();
}

void CGameObject::addComponent(IComponent * pComponent)
{
		m_Components.push_back(pComponent);
		m_ComponentMap[pComponent->getName()]=pComponent;
		pComponent->setParent(this);
};


void CGameObject::init()
{
	for (vector<IComponent*>::iterator iter=m_Components.begin();iter!=m_Components.end();iter++)
	{
		(*iter)->init();
	}
}

void CGameObject::render()
{
	for (vector<IComponent*>::iterator iter=m_Components.begin();iter!=m_Components.end();iter++)
	{
		(*iter)->render();
	}
}

void CGameObject::update(float elapsedTime)
{
	for (vector<IComponent*>::iterator iter=m_Components.begin();iter!=m_Components.end();iter++)
	{
		(*iter)->update(elapsedTime);
	}
}