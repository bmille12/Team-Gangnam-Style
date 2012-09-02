#include "GameApplication.h"

CGameApplication::CGameApplication(void)
{
	m_pWindow=NULL;
	m_pD3D10Device=NULL;
	m_pRenderTargetView=NULL;
	m_pSwapChain=NULL;
}

CGameApplication::~CGameApplication(void)
{
	if (m_pD3D10Device)
		m_pD3D10Device->ClearState();
	if (m_pRenderTargetView)
		m_pRenderTargetView->Release();
	if (m_pSwapChain)
		m_pSwapChain->Release();
	if (m_pD3D10Device)
		m_pD3D10Device->Release();
	if (m_pWindow)
	{
		delete m_pWindow;
		m_pWindow=NULL;
	}
}

bool CGameApplication::init()
{
	if (!initWindow())
		return false;
	if (!initGraphics())
		return false;
	
	return true;
}

void CGameApplication::run()
{
	while(m_pWindow->running())
	{
		if (! m_pWindow->checkForWindowMessages())
		{
			update();
			render();
		}
	}
}

void CGameApplication::render()
{
}

void CGameApplication::update()
{
}

bool CGameApplication::initGraphics()
{
	return true;
}

bool CGameApplication::initWindow()
{
	m_pWindow=new CWin32Window();
	if (!m_pWindow->init(TEXT("Lab 1 - Create Device"),800,640,false))
		return false;
	return true;
}