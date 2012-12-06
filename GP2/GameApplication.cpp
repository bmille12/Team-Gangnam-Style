#include "GameApplication.h"
#include "GameObject.h"
#include "Joypad.h"

#include "Input.h"
#include "Keyboard.h"

CGameApplication::CGameApplication(void)
{
	m_pWindow=NULL;
	m_pD3D10Device=NULL;
	m_pRenderTargetView=NULL;
	m_pSwapChain=NULL;
	m_pDepthStencelView=NULL;
	m_pDepthStencilTexture=NULL;
	m_pGameObjectManager=new CGameObjectManager();
	debugStatus = false;
}

CGameApplication::~CGameApplication(void)
{
	if (m_pD3D10Device)
		m_pD3D10Device->ClearState();

	if (m_pGameObjectManager)
	{
		delete m_pGameObjectManager;
		m_pGameObjectManager=NULL;
	}

	if (m_pRenderTargetView)
		m_pRenderTargetView->Release();
	if (m_pDepthStencelView)
		m_pDepthStencelView->Release();
	if (m_pDepthStencilTexture)
		m_pDepthStencilTexture->Release();
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
	if (!initInput())
		return false;
	if (!initAudio())
		return false;
	if (!initGame())
		return false;
	return true;
}

bool CGameApplication::initAudio()
{
	CAudioSystem::getInstance().init();
	return true;
}

bool CGameApplication::initGame()
{
    // Set primitive topology, how are we going to interpet the vertices in the vertex buffer - BMD
    //http://msdn.microsoft.com/en-us/library/bb173590%28v=VS.85%29.aspx - BMD
    m_pD3D10Device->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );	

	//add the game objects
	//We will use methods to create each type of object
	//First, the skybox
	m_pGameObjectManager->addGameObject(createSky("Sky","sphere.fbx","Environment.fx","Mars.dds"));
	//Next, the terrain
	m_pGameObjectManager->addGameObject(createTerrain("Terrain","Specular.fx","face.png",8,1,60));
	//Next, the player tank
	m_pGameObjectManager->addGameObject(createModel("Player","Tank1.fbx","DirectionLight.fx","Tank1_DF.tga","Tank1_S.tga",
	"Tank1_N.tga","Tank1_H.tga",0.0f,-10.0f,0.0f));
	//Finally, the obstacle - this will soon be done in update() and loop for constantly spawning enemies
	m_pGameObjectManager->addGameObject(createModel("Statue","humanoid.fbx","Parallax.fx", "armoredrecon_diff.png","armoredrecon_spec",
	"armoredrecon_N.png","armoredrecon_Height.png", 0,0,140));
	
	m_pGameObjectManager->addGameObject(createModel("Obstacle0","Tank1.fbx","DirectionLight.fx","Tank1_DF.tga","Tank1_S.tga",
	"Tank1_N.tga","Tank1_H.tga",0.0f,-10.0f,-100.0f));
	m_pGameObjectManager->addGameObject(createModel("Obstacle1","Tank2.fbx","DirectionLight.fx","Tank2_DF.tga","Tank2_S.tga",
	"Tank2_N.tga","Tank1_H.tga",0.0f,-10.0f,-100.0f));
	m_pGameObjectManager->addGameObject(createModel("Obstacle2","barrel.fbx","DirectionLight.fx","barrel_color_01.png","barrel_spec_01",
	"barrel_n_01.png","armoredrecon_Height.png",0.0f,-10.0f,-100.0f));
		m_pGameObjectManager->addGameObject(createModel("Obstacle3","Ammo_box.fbx","DirectionLight.fx","ammo_box_color_01.png","ammo_box_spec_01",
	"ammo_box_nmap_01.png","armoredrecon_Height.png",0.0f,-10.0f,-100.0f));
	m_pGameObjectManager->addGameObject(createModel("Obstacle4","Tank1.fbx","DirectionLight.fx","armoredrecon_diff.png","armoredrecon_spec",
	"armoredrecon_N.png","armoredrecon_Height.png",0.0f,-10.0f,-100.0f));

	//Scale models that need it
	CTransformComponent * pTransformBLAHa=m_pGameObjectManager->findGameObject("Obstacle2")->getTransform();
		pTransformBLAHa->scale(0.001f,0.001f,0.001f);
		CTransformComponent * pTransformBLAHb=m_pGameObjectManager->findGameObject("Obstacle3")->getTransform();
		pTransformBLAHb->scale(0.001f,0.001f,0.001f);
		CTransformComponent * pTransformBLAHf=m_pGameObjectManager->findGameObject("Player")->getTransform();
		pTransformBLAHf->rotate(0,1.6f,0);

	//Create the camera
	CGameObject *pCameraGameObject=new CGameObject();
	pCameraGameObject->getTransform()->setPosition(0.0f,0.0f,-50.0f);
	pCameraGameObject->setName("Camera");

	D3D10_VIEWPORT vp;
	UINT numViewports=1;
	m_pD3D10Device->RSGetViewports(&numViewports,&vp);

	CCameraComponent *pCamera=new CCameraComponent();
	pCamera->setUp(0.0f,1.0f,0.0f);
	pCamera->setLookAt(0.0f,0.0f,0.0f);
	pCamera->setFOV(D3DX_PI*0.25f);
	pCamera->setAspectRatio((float)(vp.Width/vp.Height));
	pCamera->setFarClip(1000.0f);
	pCamera->setNearClip(0.1f);
	pCameraGameObject->addComponent(pCamera);
	//Audio - Create another audio component for music
	CAudioSourceComponent *pAudio=new CAudioSourceComponent();
	//Audio -If it is an mp3 or ogg then set stream to true
	pAudio->setFilename("Sounds\\Soviet Union.mp3");
	//Audio - stream to true
	pAudio->setStream(true);
	//Audio - Add to camera, don't call play until init has been called
	///CGameObject* pO=m_pGameObjectManager->findGameObject("Player");
	pCameraGameObject->addComponent(pAudio);
	////Audio - play music audio source
	////Audio - Create another audio component for music
	//CAudioSourceComponent *pSFX=new CAudioSourceComponent();
	////Audio -If it is an mp3 or ogg then set stream to true
	//pAudio->setFilename("Sounds\laser1.mp3");
	////Audio - stream to true
	//pAudio->setStream(false);
	////Audio - Add to camera, don't call play until init has been called
	/////CGameObject* pO=m_pGameObjectManager->findGameObject("Player");
	//pCameraGameObject->addComponent(pSFX);
	////Audio - play music audio source

	//Simple C++ sound
	//PlaySound(L"C:\\Users\\Bryan\\Documents\\GitHub\\Team-Gangnam-Style\\GP2\\Sounds\\GameTheme.mp3", NULL, SND_FILENAME);

	//Audio - Attach a listener to the camera
	CAudioListenerComponent *pListener=new CAudioListenerComponent();
	pCameraGameObject->addComponent(pListener);
	m_pGameObjectManager->addGameObject(pCameraGameObject);

	CGameObject *pLightGameObject=new CGameObject();
	pLightGameObject->setName("DirectionalLight");

	CDirectionalLightComponent *pLightComponent=new CDirectionalLightComponent();
	pLightComponent->setDirection(D3DXVECTOR3(0.0f,0.0f,-1.0f));
	pLightGameObject->addComponent(pLightComponent);

	m_pGameObjectManager->addGameObject(pLightGameObject);

	m_pGameObjectManager->setMainLight(pLightComponent);

	//init, this must be called after we have created all game objects
	m_pGameObjectManager->init();
	pAudio->play();
	obstacleCount=0;
	m_keyTimer=0;
	m_Timer.start();
	return true;
}

//void CGameApplication::DisplaySomeText(LPCWSTR textToDraw, int left, int right, int top)
//{
//// Create a D3DX font object
//	//D3DXCreateFont( d3dMgr->getTheD3DDevice(), 20, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_font );
//
//	// Create a colour for the text - in this case blue
//	D3DCOLOR fontColor = D3DCOLOR_ARGB(255,0,0,255);   
//
//	// Create a rectangle to indicate where on the screen it should be drawn
//	RECT rct;
//	rct.left=left;
//	rct.right=right;
//	rct.top=top;
//	rct.bottom=rct.top+20;
//
//	// Draw some text
//	m_font->DrawText(NULL, textToDraw, -1, &rct, 0, fontColor );
//}

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

CGameObject* CGameApplication::createTerrain(string name,string effect,string texture,float xsize,float ysize,float zsize)
{
	//method for creating a terrain
	CGameObject *pObject=new CGameObject();
	//Set the name
	pObject->setName(name);
	//Creates a simple cube
	CMeshComponent *pMesh=modelloader.createCube(m_pD3D10Device,xsize,ysize,zsize);
	//Set the basic objects attributes
	pMesh->SetRenderingDevice(m_pD3D10Device);
	CMaterialComponent *pMaterial=new CMaterialComponent();
	pMaterial=new CMaterialComponent();
	pMaterial->SetRenderingDevice(m_pD3D10Device);
	pMaterial->setEffectFilename(effect);
	pMaterial->loadEnvironmentTexture(texture);
	pObject->addComponent(pMaterial);
	pObject->addComponent(pMesh);
	//Position is fixed; Only one terrain needed
	pObject->getTransform()->setPosition(0.0f,-1.0f,0.0f);
	//Returns the object
	return pObject;
}

CGameObject* CGameApplication::createSky(string name, string model, string effect, string texture)
{
	//Method will create a skybox, no need to ste position since camera/player are fixed
	CGameObject *pObject=new CGameObject();
	//Set the name
	pObject->setName(name);
	//Load in the model
	CMeshComponent *pMesh=modelloader.loadModelFromFile(m_pD3D10Device,model);
	//Set up basic model attributes
	pMesh->SetRenderingDevice(m_pD3D10Device);
	CMaterialComponent *pMaterial=new CMaterialComponent();
	pMaterial=new CMaterialComponent();
	pMaterial->SetRenderingDevice(m_pD3D10Device);
	pMaterial->setEffectFilename(effect);
	pMaterial->loadEnvironmentTexture(texture);
	pObject->addComponent(pMaterial);
	pObject->addComponent(pMesh);
	//Return the object
	return pObject;
}

CGameObject* CGameApplication::createModel(string name, string model, string effect, string textureD, string textureS
	, string textureB, string textureP, float xpos, float ypos, float zpos)
{
	//Method will be used to create game objects - player, obstacles, etc
	//Method uses many parameters for dynamic object creation

	CGameObject *pObject=new CGameObject();
	//Set the name
	pObject->setName(name);
	//Position
	pObject->getTransform()->setPosition(xpos,ypos,zpos);
	//create material
	CMaterialComponent *pMaterial=new CMaterialComponent();
	//Set up basic object attributes
	pMaterial->SetRenderingDevice(m_pD3D10Device);
	pMaterial->setEffectFilename(effect);
	pMaterial->setAmbientMaterialColour(D3DXCOLOR(0.2f,0.2f,0.2f,1.0f));
	pMaterial->loadDiffuseTexture(textureD);
	pMaterial->loadSpecularTexture(textureS);
	pMaterial->loadBumpTexture(textureB);
	pMaterial->loadParallaxTexture(textureP);
	pObject->addComponent(pMaterial);

	//Create Mesh
	//Load in model
	CMeshComponent *pMesh=modelloader.loadModelFromFile(m_pD3D10Device,model);
	pMesh->SetRenderingDevice(m_pD3D10Device);
	pObject->addComponent(pMesh);
	//return the game object
	return pObject;
}

void CGameApplication::render()
{
    // Just clear the backbuffer, colours start at 0.0 to 1.0
	// Red, Green , Blue, Alpha - BMD
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; 
	//Clear the Render Target
	//http://msdn.microsoft.com/en-us/library/bb173539%28v=vs.85%29.aspx - BMD
    m_pD3D10Device->ClearRenderTargetView( m_pRenderTargetView, ClearColor );
	m_pD3D10Device->ClearDepthStencilView(m_pDepthStencelView,D3D10_CLEAR_DEPTH,1.0f,0);

	//We need to iterate through all the Game Objects in the managers
	for(vector<CGameObject*>::iterator iter=m_pGameObjectManager->getBegining();iter!=m_pGameObjectManager->getEnd();iter++)
	{
		//grab the transform
		CTransformComponent *pTransform=(*iter)->getTransform();
		//and the geometry
		CMeshComponent *pMesh=static_cast<CMeshComponent*>((*iter)->getComponent("MeshComponent"));
		//and the material
		CMaterialComponent *pMaterial=static_cast<CMaterialComponent*>((*iter)->getComponent("MaterialComponent"));

		//do we have a matrial
		if (pMaterial)
		{
			CCameraComponent *camera=m_pGameObjectManager->getMainCamera();

			//set the matrices
			pMaterial->setProjectionMatrix((float*)camera->getProjection());
			pMaterial->setViewMatrix((float*)camera->getView());
			pMaterial->setWorldMatrix((float*)pTransform->getWorld());
			//set light colour
			pMaterial->setAmbientLightColour(D3DXCOLOR(0.5f,0.5f,0.5f,1.0f));

			//get the main light and the camera
			CDirectionalLightComponent * light=m_pGameObjectManager->getMainLight();
			pMaterial->setDiffuseLightColour(light->getDiffuseColour());
			pMaterial->setSpecularLightColour(light->getSpecularColour());
			pMaterial->setLightDirection(light->getLightDirection());
			
			pMaterial->setCameraPosition(camera->getParent()->getTransform()->getPosition());

			pMaterial->setTextures();
			pMaterial->setMaterial();
			//bind the vertex layout
			pMaterial->bindVertexLayout();
			//loop for the passes in the material
			for (UINT i=0;i<pMaterial->getNumberOfPasses();i++)
			{
				//Apply the current pass
				pMaterial->applyPass(i);
				//we have a geometry
				if (pMesh)
				{
					//Loop through all the subsets in the mesh
					for (int i=0;i<pMesh->getTotalNumberOfSubsets();i++)
					{
						//grab one of the subset
						CGeometry *pSubset=pMesh->getSubset(i);
						//bind the buffers contained in the subset
						pSubset->bindBuffers();
						//draw
						m_pD3D10Device->DrawIndexed(pSubset->getNumberOfIndices(),0,0);
					}
				}
			}
		}

	}
	//Swaps the buffers in the chain, the back buffer to the front(screen)
	//http://msdn.microsoft.com/en-us/library/bb174576%28v=vs.85%29.aspx - BMD
    m_pSwapChain->Present( 0, 0 );
}

void CGameApplication::saveGame()
{
	//Method for saving the game

	//Create locals 
	//--Output string
	string op;
	//--Input integer
	int val;
	//--Converter object(int to string)
	ostringstream con;
	//Output to text file
	ofstream outputFile;
	//Open/create save file
	outputFile.open("save.txt");
	//For now, the only value being saved will be the player health
	//Later, will save level, position, score, and possibly even active enemies
	//Get player object
	val=m_pGameObjectManager->findGameObject("Player")->getHealth();
	//Convert player health to string
	con << val;
	//Set output to that
	op = con.str();
	//Write to file
	outputFile << op + "\n";
	//Close file
	outputFile.close();
}
void CGameApplication::loadGame()
{
	//Method for saving the game

	//Create locals 
	//--Input string
	string ip;
	//Output int
	int val;
	//Converter object(string to int)
	istringstream con;
	//Input file being loaded
	ifstream inputFile;
	//Open saved game
	inputFile.open("save.txt");
	//Check that input file is open
	if ( inputFile.is_open())
	{
		//Make sure file is good for i/o operation
		while (inputFile.good())
		{
			//Read in each line of saved game. For now, 1 value. Later, we'll use an array to hold then set player values.
			getline (inputFile,ip);
		}
		//Close file
	inputFile.close();
	}
}

void CGameApplication::moveF()
{
	//Method for moving forward; debug only

if(debugStatus==true)
	{
		//Check for debug status, if true then move forward. Camera will always face main game area.
		CTransformComponent * pTransform=m_pGameObjectManager->findGameObject("Camera")->getTransform();
		pTransform->translate(0.0f,0.0f,0.1f);
	}
	else
	{
		//Else do nothing

	}
}
void CGameApplication::moveB()
{
	//Method for moving backwards; debug only
if(debugStatus==true)
	{
		//If on debug view, camera will move backwards, always facing main game area
		CTransformComponent * pTransform=m_pGameObjectManager->findGameObject("Camera")->getTransform();
		pTransform->translate(0.0f,0.0f,-0.1f);
	}
	else
		//else do nothing
	{

	}
}
void CGameApplication::moveR()
{
	//Method for moving right

	//Check debug status
	if(debugStatus==true)
	{
		//If on debug view, move camera right, it will always face main game area
		CTransformComponent * pTransform=m_pGameObjectManager->findGameObject("Camera")->getTransform();
		pTransform->translate(0.1f,0.0f,0);
	}
	else
	{
		//If not on debug, move player right
		CTransformComponent * pTransform=m_pGameObjectManager->findGameObject("Player")->getTransform();
		pTransform->translate(0.1f,0.0f,0);
	}
}
void CGameApplication::moveL()
{
	//Method for moving to the left

	//Check debug status
	if(debugStatus==true)
	{
		//While on debug view, camera will move right, always looking at the main game ares
		CTransformComponent * pTransform=m_pGameObjectManager->findGameObject("Camera")->getTransform();
		pTransform->translate(-0.1f,0.0f,0);
	}
	else
	{
		//If not on debug, player moves left
		CTransformComponent * pTransform=m_pGameObjectManager->findGameObject("Player")->getTransform();
		pTransform->translate(-0.1f,0.0f,0);
	}
}

void CGameApplication::update()
{
		//Draw the GUI
	//DisplaySomeText(L"Health: ",20,50,40);
	//CInput::getInstance().getJoypad(0)->update();
	CAudioSystem::getInstance().update();
	m_Timer.update();
	m_keyTimer++;
	//Obstacles will perpetually move 
	//For loop here: For every "obstacle" object
	//Get the transform component for each one
	for (int i=0;i<obstacleCount;i++)
	{
			stringy = "Obstacle";
			stringy2 = convertInt(i);
			stringy = stringy+stringy2;
	CGameObject* pObjector=m_pGameObjectManager->findGameObject(stringy);
				//translate it
			CTransformComponent* pTransform=pObjector->getTransform();
				pTransform->translate(0,0,-1.0f);
				pTransform->rotate(1.0f,0,0);
	}
	//Next loop will spawn obstacles
	//While there are less than X obstacles on the field
		while ( obstacleCount < 4 && m_keyTimer > 90 )
		{
	// ---X will change based on level
	//Spawn a new obstacle
			int i=obstacleCount;
			stringy = "Obstacle";
			stringy2 = convertInt(obstacleCount);
			stringy = stringy+stringy2;

			CGameObject* pObjector=m_pGameObjectManager->findGameObject(stringy);
			CTransformComponent* pTransform=pObjector->getTransform();
				pTransform->setPosition(0,-10,100);
			//Reset timer
			m_keyTimer=0;
			//iterate obstacleCount
			obstacleCount++;
	//end while loop
		}


	//if obstacle is out of bounds, delete it
	//m_pGameObjectManager->removeGameObject(gObject);

	//End loop
	


	//This code will take in the W key, only used when in debug view
		if (CInput::getInstance().getKeyboard()->isKeyDown((int)'W')/*||CInput::getInstance().getJoypad(0)->getLeftThumbStickY()>1000*/)
	{
		//Calls the move forward method
		moveF();
	}
	//This code will take in the S key, again only used for debug
	else if (CInput::getInstance().getKeyboard()->isKeyDown((int)'S')/*||CInput::getInstance().getJoypad(0)->getLeftThumbStickY()<-1000*/)  
	{
		//Calls move backward methpd
		moveB();
	}
	//Take in D key
	if (CInput::getInstance().getKeyboard()->keyPressed((int)'D')/*||CInput::getInstance().getJoypad(0)->getLeftThumbStickX()>1000*/)
	{
		//Call move right method
		moveR();
	}
	//Take in A key
	else if (CInput::getInstance().getKeyboard()->keyPressed((int)'A')/*||CInput::getInstance().getJoypad(0)->getLeftThumbStickY()<-1000*/)
	{
		//Call move left method
		moveL();
	}
	//Take in K Key
	if (CInput::getInstance().getKeyboard()->isKeyDown((int)'K'))
	{
		//For now, this key will call the Load Game function
		//Eventually this will be done via a menu
		loadGame();
	}
	//Take in T key
	if (CInput::getInstance().getKeyboard()->isKeyDown((int)'T'))
	{
		//Check for debug status
		if ( debugStatus == false )
		{
			//If not on debug, turn debug view on
			debugStatus = true;
		}
	}
	//Take in Y Key
		if (CInput::getInstance().getKeyboard()->isKeyDown((int)'Y'))
	{
		//Check for debug status
		if ( debugStatus == true )
		{
			//If on debug, turn debug view off
			debugStatus = false;
		}
	}
		if (CInput::getInstance().getKeyboard()->isKeyDown((int)'Q'))
	{
		//For now, this key will test the SFX code
		//Eventually this will be done via gameplay
		//CAudioSourceComponent* pSF=m_pGameObjectManager->findGameObject("Camera")->getComponent("pSFX");
		//pSF->play();
		
	}
	//Update GameObjectManager
	m_pGameObjectManager->update(m_Timer.getElapsedTime());

	
	
}

bool CGameApplication::initInput()
{
	CInput::getInstance().init();
	return true;
}


//initGraphics - initialise the graphics subsystem - BMD
bool CGameApplication::initGraphics()
{
	//Retrieve the size of the window, this is need to match the
	//back buffer to screen size - BMD
	RECT windowRect;
	//http://msdn.microsoft.com/en-us/library/ms633503%28v=vs.85%29.aspx -BMD
	GetClientRect(m_pWindow->getHandleToWindow(),&windowRect);

	//Calculate the width and height of the window - BMD
	UINT width=windowRect.right-windowRect.left;
	UINT height=windowRect.bottom-windowRect.top;

	//Device creation flags, used to control our the D3D10 device is created
	UINT createDeviceFlags=0;
	//If we are in a debug build then set the device creation flag to debug device
#ifdef _DEBUG
	createDeviceFlags|=D3D10_CREATE_DEVICE_DEBUG;
#endif

	//Swap Chain description - used in the creation of the swap chain
	//http://msdn.microsoft.com/en-us/library/bb173075%28v=vs.85%29.aspx - BMD

	//Initialise the swap chain description by setting all its values to zero - BMD
	DXGI_SWAP_CHAIN_DESC sd;
	//http://msdn.microsoft.com/en-us/library/aa366920%28v=vs.85%29.aspx - BMD
    ZeroMemory( &sd, sizeof( sd ) );
	//What kind of surface is contained in the swap chain, in this case something we draw too
	//http://msdn.microsoft.com/en-us/library/bb173078%28v=vs.85%29.aspx - BMD
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	//Number of buffers, if we are not full screen this will be one as the desktop
	//acts as a front buffer. If we are in full screen this will be one - BMD
	if (m_pWindow->isFullScreen())
		sd.BufferCount = 2;
	else 
		sd.BufferCount=1;
	//The handle of the window which this swap chain is linked to, this must not be NULL - BMD
	sd.OutputWindow = m_pWindow->getHandleToWindow();
	//Are we in windowed mode, arggh opposite of full screen
	sd.Windowed = (BOOL)(!m_pWindow->isFullScreen());
	//Multisampling(antialsing) parameters for the swap chain - this has performance considerations - see remarks in docs
	//http://msdn.microsoft.com/en-us/library/bb173072%28v=vs.85%29.aspx - BMD
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
	//The description of the swap chain buffer
	//http://msdn.microsoft.com/en-us/library/bb173064%28v=vs.85%29.aspx - BMD
	//width & height of the buffer - this matches the size of the window - BMD
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
	//The data format of the buffer in the swap chain, 8bits used for Red, green, blue & alpha - unsigned int(UNIFORM) - BMD
	//http://msdn.microsoft.com/en-us/library/bb173059%28v=vs.85%29.aspx
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//Refresh rate of the buffer in the swap chain - BMD
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
	
	//NB. You should get use to seeing patterns like this when programming with D3D10 
	//where we use a description object which is then used in the creation of a D3D10 resource 
	//like swap chains. Also in a real application we would check to see if some of the above
	//options are support by the graphics hardware. -BMD

	//Create D3D10 Device and swap chain 
	//http://msdn.microsoft.com/en-us/library/bb205087%28v=vs.85%29.aspx - BMD
	if (FAILED(D3D10CreateDeviceAndSwapChain(NULL, //Pointer to IDXGIAdpater, this is a display adapater on the machine this can be NULL - BMD
		D3D10_DRIVER_TYPE_HARDWARE,//Type of Driver we have, it can be a hardware device, refrence(slow) or Software(not supported yet) - BMD
		NULL, //Handle to a module that implements a software rasterizer - BMD
		createDeviceFlags,//The device creation flags we used earlier on - BMD
		D3D10_SDK_VERSION,//The version of the SDK we are using this should D3D10 - BMD
		&sd,//The memory address of the swap chain description - BMD
		&m_pSwapChain, //The memory address of the swap chain pointer, if all goes well this will be intialised after this function call - BMD
		&m_pD3D10Device)))//the memory address of the D3D10 Device, if all goes well this will be initialised after this function call - BMD
		return false;

	//NB. There are two ways of creating the device, the above way which initialises the device and swap chain at the sametime
	// or we can create a swap chain and a device seperatly and then associate a swap chain with a device. - BMD

	//Create a render target, this is a Texture which will hold our backbuffer, this will
	//enable us to link the rendertarget with buffer held in the swap chain - BMD
	ID3D10Texture2D *pBackBuffer;
	//Get a buffer from the swap chain 
	//http://msdn.microsoft.com/en-us/library/bb174570%28v=vs.85%29.aspx - BMD
	if (FAILED(m_pSwapChain->GetBuffer(0, //buffer index, 0 will get the back buffer
		__uuidof(ID3D10Texture2D),//The unique identifier of the type of pointer we want in
								  //this case a I3D10 Texture2D
		(void**)&pBackBuffer)))//A pointer to a memory address, this is cast to a void ** because this function
							   //can return back different types dependent on the 2nd param
		return false;

	D3D10_TEXTURE2D_DESC descDepth;
	descDepth.Width=width;
	descDepth.Height=height;
	descDepth.MipLevels=1;
	descDepth.ArraySize=1;
	descDepth.Format=DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count=1;
	descDepth.SampleDesc.Quality=0;
	descDepth.Usage=D3D10_USAGE_DEFAULT;
	descDepth.BindFlags=D3D10_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags=0;
	descDepth.MiscFlags=0;

	if (FAILED(m_pD3D10Device->CreateTexture2D(&descDepth,NULL,&m_pDepthStencilTexture)))
		return false;

	D3D10_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format=descDepth.Format;
	descDSV.ViewDimension=D3D10_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice=0;

	if (FAILED(m_pD3D10Device->CreateDepthStencilView(m_pDepthStencilTexture,&descDSV,&m_pDepthStencelView)))
		return false;


	//Create the Render Target View, a view is the way we access D3D10 resources
	//http://msdn.microsoft.com/en-us/library/bb173556%28v=vs.85%29.aspx - BMD
	if (FAILED(m_pD3D10Device->CreateRenderTargetView( pBackBuffer, //The resource we are creating the view for - BMD
		NULL, //The description of the view, in this case NULL - BMD
		&m_pRenderTargetView ))) // the memory address of a pointer to D3D10 Render Target - BMD
	{
		
		pBackBuffer->Release();
		return  false;
	}
	//The above Get Buffer call will allocate some memory, we now need to release it. - BMD
    pBackBuffer->Release();

	//Binds one or more render targets and depth buffer to the Output merger stage - BMD
	//http://msdn.microsoft.com/en-us/library/bb173597%28v=vs.85%29.aspx - BMD
	m_pD3D10Device->OMSetRenderTargets(1, //Number  of views - BMD
		&m_pRenderTargetView, //pointer to an array of D3D10 Render Target Views - BMD
		m_pDepthStencelView); //point to Depth Stencil buffer - BMD

    // Setup the viewport 
	//http://msdn.microsoft.com/en-us/library/bb172500%28v=vs.85%29.aspx - BMD
    D3D10_VIEWPORT vp;
    vp.Width = width;
    vp.Height = height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
	//Sets the Viewport 
	//http://msdn.microsoft.com/en-us/library/bb173613%28v=vs.85%29.aspx - BMD
    m_pD3D10Device->RSSetViewports( 1 //Number of viewports to bind
		, &vp );//an array of viewports

	return true;
}

bool CGameApplication::initWindow()
{
	m_pWindow=new CWin32Window();
	if (!m_pWindow->init(TEXT("Team Gangnam Style"),800,640,false))
		return false;
	return true;
}