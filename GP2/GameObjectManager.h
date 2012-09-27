#pragma once

#include <vector>
#include <map>

using namespace std;

class CGameObject;

class CGameObjectManager
{
public:
	CGameObjectManager();
	~CGameObjectManager();

	void addGameObject(CGameObject * pObject);

	vector<CGameObject*>::iterator getBegining();
	vector<CGameObject*>::iterator getEnd();

	CGameObject * findGameObject(const string& name);

	void clear();

	void init();
	void update(float elapsedTime);
	void render();

private:
	vector<CGameObject*> m_GameObjects;
	vector<CGameObject*>::iterator m_GameObjectIter;
	map<string,CGameObject*> m_GameObjectsMap;
};