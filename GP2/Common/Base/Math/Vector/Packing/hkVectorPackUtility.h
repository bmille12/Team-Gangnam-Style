/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2012 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */
#ifndef HKMATH_VECTOR_PACK_UTILITY_H
#define HKMATH_VECTOR_PACK_UTILITY_H

#include <Common/Base/Types/Geometry/Aabb/hkAabb.h>
#include <Common/Base/Math/Vector/hkIntVector.h>
//HK_REFLECTION_PARSER_EXCLUDE_FILE

/// Helper class to pack vectors in various formats
struct hkVectorPackUtility
{
	/// decode an unsigned integer into 3 floats, the precision being NUM_BITS_X, NUM_BITS_Y, #totalBitsInT-NUM_BITS_X-NUM_BITS_Y.
	/// To use this method you have to include
	/// #include <Common/Base/Math/Vector/hkIntVector.h>
	template< typename T, int NUM_BITS_X, int NUM_BITS_Y >
	static HK_FORCE_INLINE void decode3( const T& i, hkVector4* HK_RESTRICT vectorOut );

	/// Decode 3 fractions between [0..1.0]
	template< typename T, int NUM_BITS_X, int NUM_BITS_Y >
	static HK_FORCE_INLINE void decodeFractions3( const T& i, hkVector4* HK_RESTRICT vectorOut );

	/// Encode 
	template< typename T, int NUM_BITS_X, int NUM_BITS_Y >
	static HK_FORCE_INLINE void	encode3(const hkAabb& aabb, hkVector4Parameter vertex, T* HK_RESTRICT output);

	/// Store all components of vector to the stream if the corresponding bit in the mask m is set, returns the pointer
	/// after the last element written.
	HK_FORCE_INLINE hkReal*	filteredStoreToStream( hkVector4Parameter vector, hkVector4Comparison::Mask m, hkReal* dest );
};



#include <Common/Base/Math/Vector/Packing/hkVectorPackUtility.inl>

#endif //HKMATH_VECTOR_PACK_UTILITY_H

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
