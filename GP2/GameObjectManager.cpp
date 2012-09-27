#include "GameObjectManager.h"
#include "GameObject.h"

CGameObjectManager::CGameObjectManager()
{
}

CGameObjectManager::~CGameObjectManager()
{
	clear();
}

void CGameObjectManager::addGameObject(CGameObject * pObject)
{
	m_GameObjects.push_back(pObject);
	m_GameObjectsMap[pObject->getName()]=pObject;
}

vector<CGameObject*>::iterator CGameObjectManager::getBegining()
{
	return m_GameObjects.begin();
}

vector<CGameObject*>::iterator CGameObjectManager::getEnd()
{
	return m_GameObjects.end();
}

CGameObject * CGameObjectManager::findGameObject(const string& name)
{
	return m_GameObjectsMap[name];
}

void CGameObjectManager::clear()
{
	m_GameObjectIter=m_GameObjects.begin();
	while(m_GameObjectIter!=m_GameObjects.end())
	{
		if ((*m_GameObjectIter))
		{
			delete (*m_GameObjectIter);
			m_GameObjectIter=m_GameObjects.erase(m_GameObjectIter);
		}
		else
		{
			m_GameObjectIter++;
		}
	}

	m_GameObjectsMap.clear();
}

void CGameObjectManager::init()
{
	for(m_GameObjectIter=m_GameObjects.begin();m_GameObjectIter!=m_GameObjects.end();m_GameObjectIter++)
	{
		(*m_GameObjectIter)->init();
	}
}
	
void CGameObjectManager::update(float elapsedTime)
{
	for(m_GameObjectIter=m_GameObjects.begin();m_GameObjectIter!=m_GameObjects.end();m_GameObjectIter++)
	{
		(*m_GameObjectIter)->update(elapsedTime);
	}
}

void CGameObjectManager::render()
{
	for(m_GameObjectIter=m_GameObjects.begin();m_GameObjectIter!=m_GameObjects.end();m_GameObjectIter++)
	{
		(*m_GameObjectIter)->render();
	}
}