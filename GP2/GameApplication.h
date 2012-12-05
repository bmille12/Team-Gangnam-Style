#pragma once
#include "Win32Window.h"
#include "Timer.h"
#include <D3D10.h>
#include <D3DX10.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "GameObjectManager.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "MaterialComponent.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "DirectionLightComponent.h"
#include "MeshComponent.h"

#include "ModelLoader.h"

#include <vector>
//Audio - Includes
#include "AudioListenerComponent.h"
#include "AudioSourceComponent.h"
#include "AudioSystem.h"

////Physics
//#include "Physics.h"
//#include "BodyComponent.h"
//#include "BoxCollider.h"

using namespace std;

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
	bool initAudio();
	CGameObject* createSky(string name, string model, string effect, string texture);
	CGameObject* createModel(string name, string model, string effect, string textureD, string textureS
	, string textureB, string textureP, float xpos, float ypos, float zpos);
	CGameObject* createTerrain(string name,string effect,string texture,float xsize,float ysize,float zsize);
	string convertInt(int number)
	{
   stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
	}
//void DisplaySomeText(LPCWSTR textToDraw, int left, int right, int top);
	bool initWindow();
	void render();
	void moveF();
	void moveB();
	void moveL();
	void moveR();
	void update();
	bool debugStatus;
	void saveGame();
	void loadGame();
	int obstacleCount;
	
private:
	//Graphics
	ID3D10Device * m_pD3D10Device;
	IDXGISwapChain * m_pSwapChain;
	ID3D10RenderTargetView * m_pRenderTargetView;
	ID3D10DepthStencilView * m_pDepthStencelView;
	ID3D10Texture2D *m_pDepthStencilTexture;

	CWin32Window * m_pWindow;

	CTimer m_Timer;
	int m_keyTimer;
	string stringy;
	string stringy2;
	
	//Get Game Object Manager
	CGameObjectManager *m_pGameObjectManager;

	CModelLoader modelloader;
};