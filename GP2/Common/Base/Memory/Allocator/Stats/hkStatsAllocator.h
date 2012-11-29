/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2012 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */

#ifndef HK_STATS_ALLOCATOR_H
#define HK_STATS_ALLOCATOR_H

#include <Common/Base/Thread/CriticalSection/hkCriticalSection.h>

	/// Simple manager which tracks statistics and forwards all calls to another allocator.
class hkStatsAllocator : public hkMemoryAllocator
{
	//+hk.MemoryTracker(ignore=True)
	public:

		hkStatsAllocator(hkMemoryAllocator* a);

		void init(hkMemoryAllocator* a);
		virtual void* blockAlloc( int numBytes );
		virtual void blockFree( void* p, int numBytes );

		virtual void getMemoryStatistics( hkMemoryAllocator::MemoryStatistics& u );
		virtual int getAllocatedSize(const void* obj, int nbytes);
		virtual void resetPeakMemoryStatistics();

	protected:

		hkCriticalSection m_criticalSection;
		hkMemoryAllocator* m_alloc;
		MemoryStatistics m_stats;
};

#endif // HK_STATS_ALLOCATOR_H

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
