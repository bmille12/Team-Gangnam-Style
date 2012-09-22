#pragma once

class IComponent;
class CTransformComponent;

#include <map>
#include <vector>
#include <string>
using namespace std;

class CGameObject
{
public:
	CGameObject();
	~CGameObject();

	const CTransformComponent * getTransform()
	{
		return m_pTransform;
	};


	const IComponent* getComponent(const string &name)
	{
		return m_ComponentMap[name];
	};

	void addComponent(IComponent * pComponent);

	void addChild(CGameObject *pChild)
	{
		m_Children.push_back(pChild);
		pChild->m_pParent=this;
	};
private:
	CTransformComponent * m_pTransform;
	map<string,IComponent*> m_ComponentMap;
	vector<IComponent*> m_Components;
	vector<CGameObject*> m_Children;

	CGameObject *m_pParent;
};