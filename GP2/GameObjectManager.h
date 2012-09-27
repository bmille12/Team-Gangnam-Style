#pragma once

//vector and map to hold Components
#include <vector>
#include <map>

using namespace std;
//forward decleration of Game Object
class CGameObject;

//Game Object Manager, used to hold all the game objects
class CGameObjectManager
{
public:
	//Constructor
	CGameObjectManager();
	~CGameObjectManager();

	//add game object
	void addGameObject(CGameObject * pObject);

	//get the beginning of vector for iteration
	vector<CGameObject*>::iterator getBegining();
	//get the end of the vectir for iteration
	vector<CGameObject*>::iterator getEnd();

	//find game object by name
	CGameObject * findGameObject(const string& name);

	//clear
	void clear();

	//init
	void init();
	//update
	void update(float elapsedTime);
	//rende
	void render();

private:
	//vector of game objects
	vector<CGameObject*> m_GameObjects;
	vector<CGameObject*>::iterator m_GameObjectIter;
	map<string,CGameObject*> m_GameObjectsMap;
};