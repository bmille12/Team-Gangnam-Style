/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2012 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */

// long vector types

#if defined(HK_COMPILER_HAS_INTRINSICS_IA32)
#if (HK_SSE_VERSION >= 0x50) && !defined(HK_REAL_IS_DOUBLE)

// storage type for hkMxVector
template <int M>
struct hkMxVectorStorage
{
	__m256 v[(M+1)>>1];
};

// storage type for hkMxSingle
template <int M>
struct hkMxSingleStorage
{
	__m256 s;
};

// storage type for hkMxReal
template <int M>
struct hkMxRealStorage
{
	__m256 r[(M+1)>>1];
};

// storage type for hkMxMask
template <int M>
struct hkMxMaskStorage
{
	__m256 c[(M+1)>>1];
};

// storage type for hkMxHalf8
template <int M>
struct hkMxHalf8Storage
{
	HK_ALIGN16( hkHalf h[M*8] );
};

#else

// storage type for hkMxVector
template <int M>
struct hkMxVectorStorage
{
	hkVector4 v[M];
};

// storage type for hkMxSingle
template <int M>
struct hkMxSingleStorage
{
	hkVector4 s;
};

// storage type for hkMxReal
template <int M>
struct hkMxRealStorage
{
	hkSimdReal r[M];
};

// storage type for hkMxMask
template <int M>
struct hkMxMaskStorage
{
	hkVector4Comparison c[M];
};

// storage type for hkMxMask
template <int M>
struct hkMxHalf8Storage
{
	HK_ALIGN16( hkHalf h[M*8] );
};

#endif

#else // !IA32

// storage type for hkMxVector
template <int M>
struct hkMxVectorStorage
{
	hkVector4 v[M];
};

// storage type for hkMxSingle
template <int M>
struct hkMxSingleStorage
{
	hkVector4 s;
};

// storage type for hkMxReal
template <int M>
struct hkMxRealStorage
{
	hkSimdReal r[M];
};

// storage type for hkMxMask
template <int M>
struct hkMxMaskStorage
{
	hkVector4Comparison c[M];
};

// storage type for hkMxMask
template <int M>
struct hkMxHalf8Storage
{
	HK_ALIGN16( hkHalf h[M*8] );
};

#endif

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
