/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2012 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */

#ifndef HAVOK_BASE_DLL_H
#define HAVOK_BASE_DLL_H

#include <Common/Base/Memory/System/Util/hkMemoryInitUtil.h>

class hkMemoryRouter;
class hctFilterDescriptor;


	/// Each DLL (filters and filter manager) contains an instance of this class.
	/// It handles the setup of shared singletons, versioning and initialization.
class hctBaseDll
{
		// IMPORTANT : All virtual methods must remain virtual
		// The reason for that is that their implementation must remain in the DLL in question rather than in the caller.

	public:

			/// Constructor. Takes an HMODULE as parameter: this is the value returned by the Windows call LoadLibrary()
		hctBaseDll (HMODULE dllModule);

			/// Returns the DLL module that was passed on construction.
		virtual HMODULE getDllModule () const;
	
			/// Returns the version of this DLL. Used to version changes in DLL interfaces.
		virtual unsigned int getDllVersion () const;

			/// Initializes the DLL with the singletons stored in "baseSystemInfo" - this forces the DLL to use those singletons
			/// instead of DLL-specific ones.
		virtual void initDll ( const hkMemoryInitUtil::SyncInfo& baseSystemInfo, hkError* errorStream );

			/// Should be called whenever a new thread is created - it ensures the static thread DLL in the DLL is updated.
		virtual void initThread ( hkMemoryRouter* m_memoryRouter );

			/// Should be called before unloading the DLL - it calls hkBaseSystem::quit().
		virtual void quitDll ();

		virtual bool isInitialized();

			/// This static, non-virtual method returns the current version at compile time
		static unsigned int getCurrentDllVersion ();

	protected:

		HMODULE m_dllModule;
};

#endif // HAVOK_FILTER_DLL_H

/*
 * Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20120831)
 * 
 * Confidential Information of Havok.  (C) Copyright 1999-2012
 * Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
 * Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
 * rights, and intellectual property rights in the Havok software remain in
 * Havok and/or its suppliers.
 * 
 * Use of this software for evaluation purposes is subject to and indicates
 * acceptance of the End User licence Agreement for this product. A copy of
 * the license is included with this software and is also available at www.havok.com/tryhavok.
 * 
 */
