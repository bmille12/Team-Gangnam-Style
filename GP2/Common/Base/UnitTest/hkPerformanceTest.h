/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2012 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */
#ifndef HK_BASE_PERFORMANCE_TEST_H
#define HK_BASE_PERFORMANCE_TEST_H
//HK_REFLECTION_PARSER_EXCLUDE_FILE

#include <Common/Base/System/Stopwatch/hkStopwatch.h>


	#define HK_PERFORMANCE_TEST_REGISTER(func, menu, path) extern const hkPerformanceTestEntry register##func = { func, #func, menu path }




//#define HK_PERFORMANCE_TEST_HIGHRES_VMX_TIMER

	struct hkPerformanceTestInput
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE, hkPerformanceTestInput);

		hkPerformanceTestInput();

		/// initialize the m_data to the range of [-1,1]
		void setDataToMinusOneOne();

		/// Random vectors between -1 and 1, a test can modify the data if necessary (but all variants will see the modified)
		HK_FORCE_INLINE hkVector4* HK_RESTRICT vectors() const { return m_data; }

		HK_FORCE_INLINE hkVector4& vec(int index) const { return m_data[index]; }

		/// The data, before each test will be executed, setDataToMinusOneOne() will be called;
		/// Each test may modify the data to fit its current needs, see also m_dataInOriginalState.
		hkVector4* HK_RESTRICT m_data;			

		/// Number of random vectors, simdreals and vectors in the user buffers
		int m_numVectors;

		/// User data, will be set to zero for every test (but it will not be set for each variant)
		mutable void* m_userData;

		/// An optional user object, e.g. can be created in variant 0 and than used by the subsequent variants
		mutable hkRefPtr<hkReferencedObject> m_userObject;

		/// Indicates that the input data is in the range [-1,1].
		/// Should you ever need to modify the input data, you should do so only if m_dataInOriginalState is set
		/// and clear the flag when the data is modified.
		mutable hkBool m_dataInOriginalState;

		/// As the performance tests are run multiple times, this variable indicate the iteration of running a variant of a test.
		int m_runIndex;
	};

	struct hkPerformanceTestOutput
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE, hkPerformanceTestOutput);

		hkPerformanceTestOutput();

		virtual ~hkPerformanceTestOutput();

		/// Empty virtual function, can be used to break the optimizer.
		virtual void* barrier(const void* d);

		void startTimer();
		void stopTimer();
		void startOverheadTimer();	
		void stopOverheadTimer();
		void resetTimer();
		void resetOverheadTimer();
		void resetBothTimers();

		hkUint32 getTicks() const;
		hkUint32 getOverheadTicks() const;

		/// Number of supported platforms
		enum { NUM_PLATFORMS = 18, TEST_NAME_MAX_LENGTH = 256 };

		/// get unique index for the reference timing of the platform running on.
		static int HK_CALL getReferenceTickIndex(int testIndex);

		static hkUint64 HK_CALL getTicksPerSecond();

		/// The name of the test for text output purposes
		char m_testName[TEST_NAME_MAX_LENGTH];

		/// The number of tests performed
		hkUint32 m_numTests;

		//
		//	Helper variable which can be used by the test to avoid bad compiler optimizations
		//

		/// A buffer of input.m_numVectors vectors initialized to 1, can be used to output data to make sure the compiler does not optimize the code away.
		hkVector4* HK_RESTRICT m_vectorsOut;

		/// A return value, useful for breaking the compiler optimizations
		hkUint32 m_resultInt[2];

		/// A return value, useful for breaking the compiler optimizations
		hkSimdReal m_resultSimdReal[2];

		/// A return value, useful for breaking the compiler optimizations
		hkVector4 m_resultVector4[2];

		/// The optional reference performance, checked for regressions
		hkUint32 m_referenceTicks;

		volatile void* m_barrier;

		hkBool m_timerValid;
		hkBool m_overheadTimerValid;

	protected:

#ifdef HK_PERFORMANCE_TEST_HIGHRES_VMX_TIMER
		hkUint64 m_timerStart;
		hkUint64 m_timerElapsed;
		hkUint64 m_overheadTimerStart;
		hkUint64 m_overheadTimerElapsed;
#else
		hkStopwatch m_timer;			///< Not for public use, please use startTimer() and stopTimer().
		hkStopwatch m_overheadTimer;	///< A stopwatch which can be used to time the overhead of the test loop.
#endif
	};



	struct hkPerformanceTestEntry
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE_CLASS, hkPerformanceTestEntry );

		/// This function is called with variant=0... until the function returns HK_FAILURE
		typedef hkResult (HK_CALL *hkPerformanceTestFunction)( const hkPerformanceTestInput& input, int variant, hkPerformanceTestOutput& output );

		static void HK_CALL runTests(hkPerformanceTestEntry const*const*const tests);

		hkPerformanceTestFunction m_func;
		const char* m_name;
		const char* m_path;
	};
	extern const hkPerformanceTestEntry* hkPerformanceTestDatabase[];



#endif // HK_BASE_PERFORMANCE_TEST_H

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
