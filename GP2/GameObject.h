#pragma once

class IComponent;
class CTransformComponent;

#include <map>
#include <list>
#include <string>
using namespace std;

class CGameObject
{
public:
	CGameObject();
	~CGameObject();


private:
	CTransformComponent * m_pTransform;
	map<string,IComponent*> m_ComponentMap;
	list<IComponent*> m_Components;


};