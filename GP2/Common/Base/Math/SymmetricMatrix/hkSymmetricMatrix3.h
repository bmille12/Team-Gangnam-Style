/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2012 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */

#ifndef HK_MATH_SYMMETRIC_MATRIX3_H
#define HK_MATH_SYMMETRIC_MATRIX3_H

/// A 3x3 symmetric matrix stored as [m00, m11, m22] [m01, m12, m20], i.e. as the diagonal and off-diagonal vectors.
class hkSymmetricMatrix3
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_MATH, hkSymmetricMatrix3);
		HK_DECLARE_REFLECTION();

	public:

		/// Sets this = scale * mtx
		HK_FORCE_INLINE void setMul(hkSimdRealParameter scale, const hkSymmetricMatrix3& mtx);

		/// Sets this *= scale
		HK_FORCE_INLINE void mul(hkSimdRealParameter scale);

		/// Sets this = ta + tb
		HK_FORCE_INLINE void setAdd(const hkSymmetricMatrix3& ta, const hkSymmetricMatrix3& tb);

		/// Sets this += Diag(a), where Diag(a) is a diagonal matrix with diagonal elements a.
		HK_FORCE_INLINE void addDiag(hkVector4Parameter diagA);
		HK_FORCE_INLINE void addDiag(hkSimdRealParameter diagA);

		/// Sets this tensor from the given matrix m. The matrix is assumed to be symmetric
		HK_FORCE_INLINE void setFromSymmetricMatrix(const hkMatrix3& m);

		/// Gets the 3x3 matrix equivalent to this tensor
		HK_FORCE_INLINE void getMatrix(hkMatrix3& m);

		/// Sets this = Diagonal(v)
		HK_FORCE_INLINE void setDiagonal(hkVector4Parameter v);

	public:

		hkVector4 m_diag;		///< Diagonal elements, i.e. [m00, m11, m22]
		hkVector4 m_offDiag;	///< Off-diagonal elements, i.e. [m01, m12, m20]
};

#include <Common/Base/Math/SymmetricMatrix/hkSymmetricMatrix3.inl>

#endif	//	HK_MATH_SYMMETRIC_MATRIX3_H

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
