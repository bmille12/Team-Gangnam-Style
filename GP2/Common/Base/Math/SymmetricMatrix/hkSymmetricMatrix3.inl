/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2012 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */

//
//	Multiplies the matrix with the given scalar

HK_FORCE_INLINE void hkSymmetricMatrix3::setMul(hkSimdRealParameter scale, const hkSymmetricMatrix3& tensor)
{
	m_diag.setMul(tensor.m_diag, scale);
	m_offDiag.setMul(tensor.m_offDiag, scale);
}

//
//	Sets this *= scale

HK_FORCE_INLINE void hkSymmetricMatrix3::mul(hkSimdRealParameter scale)
{
	m_diag.mul(scale);
	m_offDiag.mul(scale);
}

//
//	Sets this = ta + tb

HK_FORCE_INLINE void hkSymmetricMatrix3::setAdd(const hkSymmetricMatrix3& ta, const hkSymmetricMatrix3& tb)
{
	m_diag.setAdd(ta.m_diag, tb.m_diag);
	m_offDiag.setAdd(ta.m_offDiag, tb.m_offDiag);
}

//
//	Sets this += Diag(a), where Diag(a) is a diagonal matrix with diagonal elements a.

HK_FORCE_INLINE void hkSymmetricMatrix3::addDiag(hkVector4Parameter diagA)
{
	m_diag.add(diagA);
}

HK_FORCE_INLINE void hkSymmetricMatrix3::addDiag(hkSimdRealParameter diagA)
{
	hkVector4 vDiag;
	vDiag.setAll(diagA);
	addDiag(vDiag);
}

//
//	Sets this tensor from the given matrix m. The matrix is assumed to be symmetric

HK_FORCE_INLINE void hkSymmetricMatrix3::setFromSymmetricMatrix(const hkMatrix3& m)
{
	m_diag.set(m.getElement<0,0>(), m.getElement<1,1>(), m.getElement<2,2>(), hkSimdReal_0);
	m_offDiag.set(m.getElement<0,1>(), m.getElement<1,2>(), m.getElement<0,2>(), hkSimdReal_0);
}

//
//	Gets the 3x3 matrix equivalent to this tensor

HK_FORCE_INLINE void hkSymmetricMatrix3::getMatrix(hkMatrix3& m)
{
	const hkSimdReal m01 = m_offDiag.getComponent<0>();
	const hkSimdReal m12 = m_offDiag.getComponent<1>();
	const hkSimdReal m02 = m_offDiag.getComponent<2>();

	hkVector4 c0;		c0.set(m_diag.getComponent<0>(), m01, m02, m01);
	hkVector4 c1;		c1.set(m01, m_diag.getComponent<1>(), m12, m12);
	hkVector4 c2;		c2.set(m02, m12, m_diag.getComponent<2>(), m02);
	m.setCols(c0, c1, c2);
}

//
//	Sets this = Diagonal(v)

HK_FORCE_INLINE void hkSymmetricMatrix3::setDiagonal(hkVector4Parameter v)
{
	m_offDiag.setZero();
	m_diag = v;
}

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
