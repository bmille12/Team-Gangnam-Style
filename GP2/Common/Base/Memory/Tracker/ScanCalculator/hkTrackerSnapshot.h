/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2012 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */

#ifndef HKBASE_TRACKER_SNAPSHOT_H
#define HKBASE_TRACKER_SNAPSHOT_H

#include <Common/Base/Memory/Tracker/Default/hkDefaultMemoryTracker.h>
#include <Common/Base/Algorithm/Sort/hkSort.h>
#include <Common/Base/System/StackTracer/hkStackTracer.h>

struct hkSubString;
class hkMemorySystem;

    /// This class represents a snapshot of the current memory state using both
    /// the information coming from the memory system and from the memory tracker.
    /// While this class has dynamically allocated members (arrays), we cannot use
    /// the default allocators to allocate them since allocating and deallocating them
    /// will perturb the status of the memory system which we are trying to capture.
class hkTrackerSnapshot  
{
    public:

		//+hk.MemoryTracker(ignore=True)
        HK_DECLARE_PLACEMENT_ALLOCATOR();

		typedef hkDefaultMemoryTracker::ClassAlloc ClassAlloc;

		struct Allocation
		{
			//+hk.MemoryTracker(ignore=True)
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,Allocation);

				/// ==
			hkBool operator==(const Allocation& rhs) const;
				/// !=
			hkBool operator!=(const Allocation& rhs) const;

			void* m_start;
			hk_size_t m_size;
			hkStackTracer::CallTree::TraceId m_traceId;		// If < 0 it is not valid
		};

			/// Initialize
		hkResult init(hkMemorySystem* memorySystem = HK_NULL, hkDefaultMemoryTracker* tracker = HK_NULL);
			/// Find what allocation this points to
		const Allocation* findAllocation(const void* ptrIn) const;
			/// Finds the allocation starting at address ptrIn
		const Allocation* findExactAllocation(const void* ptrIn) const;

			/// Returns HK_SUCCESS if the data appears consistent
		hkResult checkConsistent() const;

			/// Work out the total amount of memory allocated (in bytes)
		hk_size_t calcTotalAllocated() const;

			/// For each class find the allocation it is associated with. HK_NULL if not found.
		void findClassAllocation(hkArray<const Allocation*>& allocs) const;

			/// Find the class allocation at the address specified
		const ClassAlloc* findClassAllocation(const void* ptr) const;

			/// Get the call tree. Can be used with the m_traceId, to get a call stack for each allocation.
		const hkStackTracer::CallTree& getCallTree() const;

			/// Get the class allocs (ordered by start pointer)
		const hkArrayBase<ClassAlloc>& getClassAllocs() const;

			/// Write access to class allocs. If the data is changed, a call to orderClassAllocs is needed
			/// for functionality such as findClassAllocation to work
		hkArrayBase<ClassAlloc>& getClassAllocs();

			/// Order class allocs. The classAllocs must be ordered for findClassAllocation to work. Generally
			/// it is - but getClassAllocs can be used to modify allocs. If that happens, call
			/// orderClassAllocs to fix up the order.
		void orderClassAllocs();

			/// Get all of the allocations (ordered by start pointer)
		const hkArrayBase<Allocation>& getAllocations() const;

			/// Swap
		void swap(hkTrackerSnapshot& rhs);

			/// Get the memory statistics dump obtained from the memory system
			/// When we take a tracker snapshot, the memory system statistics will
			/// be dumped and stored in the snapshot for future usage (by the Memory
			/// Analyzer for instance).
			/// The returned pointer will be valid until the hkTrackerSnapshot instance
			/// is destroyed.
		const char* getMemorySystemStatistics() const;

			/// ==
		hkBool operator==(const hkTrackerSnapshot& rhs) const;
			/// !=
		hkBool operator!=(const hkTrackerSnapshot& rhs) const;

		/// Dump details (including stack trace if there is one) about an allocation
		void dumpAllocation(const Allocation& alloc, hkOstream& stream) const;

			/// Returns true if the allocation was found and removed
		hkBool removeAllocation(const void* ptr);

			/// Works out the allocations which are different
		static void HK_CALL allocationDiff(const hkTrackerSnapshot& snapA, const hkTrackerSnapshot& snapB, hkArray<Allocation>& onlyInA, hkArray<Allocation>& onlyInB);

			/// Find an allocation in an array. The array is assumed sorted by start address (as it is in the snapshot)
		static const Allocation* findAllocation(const hkArrayBase<Allocation>& allocations, const void* ptrIn);

			/// Ctor, using malloc
		hkTrackerSnapshot();
		hkTrackerSnapshot(hkMemoryAllocator* mem);
			/// Copy ctor
		hkTrackerSnapshot(const hkTrackerSnapshot& rhs);
			/// Dtor
		~hkTrackerSnapshot();

    protected:
	

		hkMemoryAllocator* m_mem;
		hkArrayBase<ClassAlloc> m_classAllocations;
		hkArrayBase<Allocation> m_allocations;

		hkStackTracer::CallTree m_callTree;
		hkArrayBase<char> m_memSysStatistics;
		hkBool m_isInitialized;
};

#include <Common/Base/Memory/Tracker/ScanCalculator/hkTrackerSnapshot.inl>

#endif // HKBASE_TRACKER_SNAPSHOT_H

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
