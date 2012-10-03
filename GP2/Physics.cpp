#include "Physics.h"

static void HK_CALL errorReport(const char* msg, void* userContext)
{
	OutputDebugStringA(msg);
}

CPhysics::CPhysics()
{
	
}

CPhysics::~CPhysics()
{
	destroy();
}

void CPhysics::init()
{
	hkMallocAllocator baseMalloc;
	// Need to have memory allocated for the solver. Allocate 1mb for it.
	hkMemoryRouter* memoryRouter = hkMemoryInitUtil::initDefault( &baseMalloc, hkMemorySystem::FrameInfo(1024 * 1024) );
	hkBaseSystem::init( memoryRouter, errorReport );

	m_pWorld = new hkpWorld( hkpWorldCinfo() );
	hkpAgentRegisterUtil::registerAllAgents( m_pWorld->getCollisionDispatcher() );
}

void CPhysics::update(float updateTime)
{
	m_pWorld->stepDeltaTime(1.0f / updateTime);
}

void CPhysics::destroy()
{
	m_pWorld->removeReference();
	hkBaseSystem::quit();
	hkMemoryInitUtil::quit();
}