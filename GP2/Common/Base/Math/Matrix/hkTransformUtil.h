/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2012 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */
#ifndef HK_MATH_TRANSFORM_UTIL_H
#define HK_MATH_TRANSFORM_UTIL_H

#include <Common/Base/Math/Vector/hkVector4Util.h>
#include <Common/Base/Math/Matrix/hkMatrix3Util.h>

/// Miscellaneous operations on hkTransforms
namespace hkTransformUtil
{
	/// Sets resultOut to be the product of t1 by the inverse of t2. (resultOut = t1 * Inverse(t2))
	HK_FORCE_INLINE void HK_CALL _computeMulInverse(const hkTransform& t1, const hkTransform& t2, hkTransform& resultOut);

	/// Converts a hkQTransform to a hkTransform
	HK_FORCE_INLINE void HK_CALL _convert(const hkQTransform& from, hkTransform& to);

	/// Converts a (hkQuaternion, hkVector4) pair to a hkTransform
	HK_FORCE_INLINE void HK_CALL _convert(hkQuaternionParameter fromRot, hkVector4Parameter fromPos, hkTransform& to);

	HK_FORCE_INLINE void HK_CALL _mulTransformTransform( const hkTransform& aTb, const hkTransform& bTc, hkTransform* tOut );
};

#include <Common/Base/Math/Matrix/hkTransformUtil.inl>

#endif		//	HK_MATH_TRANSFORM_UTIL_H

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
