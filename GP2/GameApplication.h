#pragma once
#include "Win32Window.h"
#include "Timer.h"
#include <D3D10.h>
#include <D3DX10.h>

#include "GameObjectManager.h"

#include "MaterialComponent.h"
#include "GeometryComponent.h"
#include "TransformComponent.h"

#include <vector>

using namespace std;

class CGameApplication
{
public:
	CGameApplication(void);
	~CGameApplication(void);
	bool init();
	void run();
private:
	bool initGame();
	bool initGraphics();
	bool initWindow();
	void render();
	void update();
private:
	ID3D10Device * m_pD3D10Device;
	IDXGISwapChain * m_pSwapChain;
	ID3D10RenderTargetView * m_pRenderTargetView;
	CWin32Window * m_pWindow;

	ID3D10DepthStencilView * m_pDepthStencelView;
	ID3D10Texture2D *m_pDepthStencilTexture;

	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProjection;
	CTimer m_Timer;

	//vector<CGameObject*> m_DisplayList;
	//vector<CGameObject*>::iterator m_DisplayListIter;
	CGameObjectManager *m_pGameObjectManager;
};