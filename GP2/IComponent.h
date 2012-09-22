#pragma once

#include <string>
using namespace std;

class CGameObject;

class IComponent
{
public:
	virtual ~IComponent(){};

	virtual const string& getName()=0;

	virtual void update(float elapsedTime)=0;

	virtual void render()=0;

	virtual void init()=0;

	virtual const CGameObject * getParent()=0;

	virtual void setParent(CGameObject *pParent)=0;

};

class CBaseComponent:public IComponent
{
public:
	CBaseComponent()
	{
		m_pParent=NULL;
	};

	virtual ~CBaseComponent(){};

	virtual const string& getName()
	{
		return "BaseComponent";
	};

	virtual void update(float elapsedTime)
	{
	};

	virtual void render()
	{
	};

	virtual void init()
	{
	};

	const CGameObject * getParent()
	{
		return m_pParent;
	};

	void setParent(CGameObject *pParent)
	{
		m_pParent=pParent;
	};
protected:
	CGameObject *m_pParent;
};