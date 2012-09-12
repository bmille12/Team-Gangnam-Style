#pragma once
#include "Win32Window.h"
#include <D3D10.h>
#include <D3DX10.h>

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

		//Vertex Buffer - BMD
	ID3D10Buffer*           m_pVertexBuffer;
	ID3D10InputLayout*      m_pVertexLayout;

	//Effect - BMD
	ID3D10Effect*           m_pEffect;
	ID3D10EffectTechnique*  m_pTechnique;
};