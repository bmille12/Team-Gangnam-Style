#include "GameApplication.h"

struct Vertex
{
    D3DXVECTOR3 Pos;
};




CGameApplication::CGameApplication(void)
{
	m_pWindow=NULL;
	m_pD3D10Device=NULL;
	m_pRenderTargetView=NULL;
	m_pSwapChain=NULL;
	m_pEffect=NULL;
	m_pTechnique=NULL;
}

CGameApplication::~CGameApplication(void)
{
	if (m_pD3D10Device)
		m_pD3D10Device->ClearState();

	if (m_pVertexBuffer)
		m_pVertexBuffer->Release();
	if (m_pVertexLayout)
		m_pVertexLayout->Release();
	if (m_pEffect)
		m_pEffect->Release();
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
	if (!initGame())
		return false;
	return true;
}

bool CGameApplication::initGame()
{
	//Set the shader flags - BMD
    DWORD dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    // Set the D3D10_SHADER_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program. - BMD
    dwShaderFlags |= D3D10_SHADER_DEBUG;
#endif
	//Create the effect - BMD
	//http://msdn.microsoft.com/en-us/library/bb172658%28v=vs.85%29.aspx -BMD
	if( FAILED(D3DX10CreateEffectFromFile( TEXT("ScreenSpace.fx"), //The filename of the effect - BMD
		NULL, //An array of shader macros we leave this NULL - BMD
		NULL, //ID3D10Include*, this allows to include other files when we are compiling the effect - BMD
		"fx_4_0", //A string which specfies the effect profile to use, in this case fx_4_0(Shader model 4) - BMD
		dwShaderFlags, //Shader flags, this can be used to add extra debug information to the shader - BMD
		0,//Fx flags, effect compile flags set to zero - BMD
        m_pD3D10Device, //ID3D10Device*, the direct3D rendering device - BMD
		NULL, //ID3D10EffectPool*, a pointer to an effect pool allows sharing of variables between effects - BMD
		NULL, //ID3DX10ThreadPump*, a pointer to a thread pump this allows multithread access to shader resource - BMD
		&m_pEffect, //ID3D10Effect**, a pointer to a memory address of the effect object. This will be initialised after this - BMD
		NULL, //ID3D10Blob**, a pointer to a memory address of a blob object, this can be used to hold errors from the compilation - BMD
		NULL )))//HRESULT*, a pointer to a the result of the compilation, this will be NULL - BMD
	{
		//If the creation of the Effect fails then a message box will be shown
        MessageBox( NULL,
                    TEXT("The FX file cannot be located.  Please run this executable from the directory that contains the FX file."), 
					TEXT("Error"), 
					MB_OK );
        return false;
    }

	//Get the technique called Render from the effect, we need this for rendering later on - BMD
	//A technique is a way of drawing, a technique can have many passes
	//http://msdn.microsoft.com/en-us/library/bb173708%28v=vs.85%29.aspx
	m_pTechnique=m_pEffect->GetTechniqueByName("Render");

    // Define the input layout of the vertex, this is so we can bind a vertex to the pipeline - BMD
    D3D10_INPUT_ELEMENT_DESC layout[] =
    {
		
        { "POSITION", //Name of the semantic, this helps to bind the vertex inside the Vertex Shader - BMD
		0, //The index of the semantic, see above - BMD
		DXGI_FORMAT_R32G32B32_FLOAT, //The format of the element, in this case 32 bits of each sub element - BMD
		0, //Input slot - BMD
		0, //Offset, this will increase as we add more elements(such texture coords) to the layout - BMD
		D3D10_INPUT_PER_VERTEX_DATA, //Input classification - BMD
		0 }, //Instance Data slot - BMD
    };
	//Number of elements in the layout - BMD
    UINT numElements = sizeof( layout ) / sizeof(D3D10_INPUT_ELEMENT_DESC);
	//Get the Pass description, we need this to bind the vertex to the pipeline - BMD
    D3D10_PASS_DESC PassDesc;
    m_pTechnique->GetPassByIndex( 0 )->GetDesc( &PassDesc );
	//Create Input layout to describe the incoming buffer to the input assembler - BMD
    if (FAILED(m_pD3D10Device->CreateInputLayout( layout, //The layout describing our vertices - BMD
		numElements, //The number of elements in the layout
		PassDesc.pIAInputSignature,//Input signature of the description of the pass - BMD
        PassDesc.IAInputSignatureSize, //The size of this Signature size of the pass - BMD
		&m_pVertexLayout ))) //The pointer to an address of Vertex Layout - BMD
	{
		return false;
	}

    // Set the input layout for the Input Assembler- BMD
    m_pD3D10Device->IASetInputLayout( m_pVertexLayout );

    // Some vertices - BMD
    Vertex vertices[] =
    {
        D3DXVECTOR3( 0.0f, 0.5f, 0.5f ),
        D3DXVECTOR3( 0.5f, -0.5f, 0.5f ),
        D3DXVECTOR3( -0.5f, -0.5f, 0.5f ),
    };
    
    //The description of the Buffer, this is a common pattern you will see when
    //creating memory buffer in D3D10, this is an example of how to create a
    //Vertex memory buffer - BMD
    //http://msdn.microsoft.com/en-us/library/bb204896%28VS.85%29.aspx - BMD
    D3D10_BUFFER_DESC bd;
    bd.Usage = D3D10_USAGE_DEFAULT;//Usuage flag,this describes how the buffer is read/written to. Default is the most common - BMD
    bd.ByteWidth = sizeof( Vertex ) * 3;//The size of the buffer, this is the size of one vertex * by the num of vertices -BMD
    bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;//BindFlags, says how the buffer is going to be used. In this case as a Vertex Buffer - BMD
    bd.CPUAccessFlags = 0;//CPUAccessFlag, sepcfies if the CPU can access the resource. 0 means no CPU access - BMD
    bd.MiscFlags = 0;//MiscCreation flags, this will be zero most of the time - BMD
    
    //This is used to supply the initial data for the buffer - BMD
    //http://msdn.microsoft.com/en-us/library/bb172456%28VS.85%29.aspx - BMD
    D3D10_SUBRESOURCE_DATA InitData;
    //A pointer to the initial data
    InitData.pSysMem = vertices;
    
    //Create the Buffer using the buffer description and initial data - BMD
    //http://msdn.microsoft.com/en-us/library/bb173544%28v=VS.85%29.aspx - BMD
    if (FAILED(m_pD3D10Device->CreateBuffer( 
		&bd, //Memory address of a buffer description - BMD
		&InitData, //Memory address of the initial data - BMD
		&m_pVertexBuffer )))//A pointer to a memory address of a buffer, this will be initialise after - BMD
		return false;

    //Get the stride(size) of the a vertex, we need this to tell the pipeline the size of one vertex - BMD
    UINT stride = sizeof( Vertex );
    //The offset from start of the buffer to where our vertices are located - BMD
    UINT offset = 0;
    //Bind the vertex buffer to input assembler stage - BMD
    //http://msdn.microsoft.com/en-us/library/bb173591%28v=VS.85%29.aspx - BMD
    m_pD3D10Device->IASetVertexBuffers( 
		0, //The input slot to bind, zero indicates the first slot - BMD
		1, //The number of buffers - BMD
		&m_pVertexBuffer, //A pointer to an array of vertex buffers - BMD
		&stride, //Pointer to an array of strides of vertices in the buffer - BMD
		&offset );//Pointer to an array of offsets to the vertices in the vertex buffers - BMD

    // Set primitive topology, how are we going to interpet the vertices in the vertex buffer - BMD
    //http://msdn.microsoft.com/en-us/library/bb173590%28v=VS.85%29.aspx - BMD
    m_pD3D10Device->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );	
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
    // Just clear the backbuffer, colours start at 0.0 to 1.0
	// Red, Green , Blue, Alpha - BMD
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; 
	//Clear the Render Target
	//http://msdn.microsoft.com/en-us/library/bb173539%28v=vs.85%29.aspx - BMD
    m_pD3D10Device->ClearRenderTargetView( m_pRenderTargetView, ClearColor );
	//All drawing will occur between the clear and present - BMD
	 
	//Get the Description of the technique, we need this in order to
	//loop through each pass in the technique - BMD
    D3D10_TECHNIQUE_DESC techDesc;
    m_pTechnique->GetDesc( &techDesc );
	//Loop through the passes in the technique - BMD
    for( UINT p = 0; p < techDesc.Passes; ++p )
    {
		//Get a pass at current index and apply it -BMD
        m_pTechnique->GetPassByIndex( p )->Apply( 0 );
		//Draw call
        m_pD3D10Device->Draw( 3, //Number of vertices
        0 );// Start Location in the vertex buffer
    }
	//Swaps the buffers in the chain, the back buffer to the front(screen)
	//http://msdn.microsoft.com/en-us/library/bb174576%28v=vs.85%29.aspx - BMD
    m_pSwapChain->Present( 0, 0 );
}

void CGameApplication::update()
{
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
		NULL); //point to Depth Stencil buffer - BMD

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
	if (!m_pWindow->init(TEXT("Lab 1 - Triangle"),800,640,false))
		return false;
	return true;
}