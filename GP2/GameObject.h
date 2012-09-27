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

	CTransformComponent * getTransform()
	{
		return m_pTransform;
	};


	IComponent* getComponent(const string &name)
	{
		return m_ComponentMap[name];
	};

	void addComponent(IComponent * pComponent);

	void init();
	void render();
	void update(float elapsedTime);
	void setName(const string& name)
	{
		m_strName=name;
	};

	const string& getName()
	{
		return m_strName;
	};
private:
	CTransformComponent * m_pTransform;
	map<string,IComponent*> m_ComponentMap;
	vector<IComponent*> m_Components;
	string m_strName;
};