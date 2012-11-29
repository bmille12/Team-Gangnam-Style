#pragma once
#include "Win32Window.h"
#include "Timer.h"
#include <D3D10.h>
#include <D3DX10.h>
//#include <fmod.hpp>

#include "GameObjectManager.h"

#include "MaterialComponent.h"
#include "GeometryComponent.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
//#include "AudioListenerComponent.h"

#include <vector>

using namespace std;
//using namespace FMOD;

class CGameApplication
{
public:
	CGameApplication(void);
	~CGameApplication(void);
	bool init();
	void run();
private:
	bool initInput();
	bool initGame();
	bool initGraphics();
	bool initWindow();
	void render();
	void update();
	CGameObject* createObject(string name, string diffuse, string effect, float oX, float oY, float oZ
		, float gX, float gY, float gZ);
private:
	//Graphics
	ID3D10Device * m_pD3D10Device;
	IDXGISwapChain * m_pSwapChain;
	ID3D10RenderTargetView * m_pRenderTargetView;
	ID3D10DepthStencilView * m_pDepthStencelView;
	ID3D10Texture2D *m_pDepthStencilTexture;

	CWin32Window * m_pWindow;

	CTimer m_Timer;
	
	//Get Game Object Manager
	CGameObjectManager *m_pGameObjectManager;

	//Audio
	//System *m_pAudioSystem;
};