/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2012 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */

#ifndef HK_MATH_SYMMETRIC_MATRIX3_UTIL_H
#define HK_MATH_SYMMETRIC_MATRIX3_UTIL_H

#include <Common/Base/Math/SymmetricMatrix/hkSymmetricMatrix3.h>
#include <Common/Base/Math/Vector/hkVector4Util.h>

/// Miscellaneous operations involving symmetric 3x3 matrices
namespace hkSymmetricMatrix3Util
{
	/// Sets mtxOut = outer product of the given vector, i.e [xx, yy, zz], [xy, yz, zx]
	HK_FORCE_INLINE void HK_CALL _computeOuterProduct(hkVector4Parameter v, hkSymmetricMatrix3& mtxOut);

	/// Projects the tensor on the given direction, i.e. computes Transpose(d) * mtx * d
	HK_FORCE_INLINE const hkSimdReal HK_CALL _computeProjection(const hkSymmetricMatrix3& mtx, hkVector4Parameter vD);

	/// Sets mtxOut = Transpose(R) * Diag(a) * R, where Diag(a) is a diagonal matrix with diagonal elements a.
	HK_FORCE_INLINE void HK_CALL _computeInverseBasisTransform(const hkMatrix3& R, hkVector4Parameter diagA, hkSymmetricMatrix3& mtxOut);

	/// Solves the linear system: mtx * v = b, for v.
	HK_FORCE_INLINE void HK_CALL _solve(const hkSymmetricMatrix3& mtx, hkVector4Parameter b, hkVector4& solutionOut);

	/// Sets inverseOut = Inverse(t).
	HK_FORCE_INLINE void HK_CALL _computeInverse(const hkSymmetricMatrix3& t, hkSymmetricMatrix3& inverseOut);

	/// Computes vOut = t * vIn
	HK_FORCE_INLINE void HK_CALL _multiply(const hkSymmetricMatrix3& t, hkVector4Parameter vIn, hkVector4& vOut);

	/// Solves the linear system: mtx * v = b for v, where |v| < vMax per component.
	/// The problem is a LCP and the returned solution is an approximation computed via Jacobi iterations.
	/// The number of iterations is given as a template parameter
	template <int NIter>
	HK_FORCE_INLINE void HK_CALL _solve(const hkSymmetricMatrix3& mtx, hkVector4Parameter b, hkVector4Parameter vMax, hkVector4& solutionOut);
};

#include <Common/Base/Math/SymmetricMatrix/hkSymmetricMatrix3Util.inl>

#endif	//	HK_MATH_SYMMETRIC_MATRIX3_UTIL_H

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
