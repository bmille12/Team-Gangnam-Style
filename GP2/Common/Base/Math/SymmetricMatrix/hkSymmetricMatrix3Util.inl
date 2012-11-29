/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2012 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */

//
//	Sets mtxOut = outer product of the given vector, i.e [xx, yy, zz], [xy, yz, zx]

HK_FORCE_INLINE void HK_CALL hkSymmetricMatrix3Util::_computeOuterProduct(hkVector4Parameter vXYZ, hkSymmetricMatrix3& mtxOut)
{
	hkVector4 vYZX;	vYZX.setPermutation<hkVectorPermutation::YZXW>(vXYZ);
	mtxOut.m_diag.setMul(vXYZ, vXYZ);		// [xx, yy, zz]
	mtxOut.m_offDiag.setMul(vXYZ, vYZX);	// [xy, yz, zx]
}

//
//	Projects the tensor on the given direction, i.e. computes Transpose(d) * mtx * d

HK_FORCE_INLINE const hkSimdReal HK_CALL hkSymmetricMatrix3Util::_computeProjection(const hkSymmetricMatrix3& mtx, hkVector4Parameter vD)
{
	hkSymmetricMatrix3 d;			// d =	[d_xx, d_yy, d_zz]
	_computeOuterProduct(vD, d);	//		[d_xy, d_yz, d_zx]
	
	hkVector4 diag;		diag.setMul(d.m_diag, d.m_diag);				// [m_xx * d_xx, m_yy * d_yy, m_zz * d_zz]
	hkVector4 offDiag;	offDiag.setMul(d.m_offDiag, d.m_offDiag);		// [m_xy * d_xy, m_yz * d_yz, m_zx * d_zx]

	diag.addMul(hkSimdReal_2, offDiag);	// [m_xx * d_xx + 2 * m_xy * d_xy, m_yy * d_yy + 2 * m_yz * d_yz, m_zz * d_zz + 2 * m_zx * d_zx]
	return diag.horizontalAdd<3>();		// [m_xx * d_xx + 2 * m_xy * d_xy + m_yy * d_yy + 2 * m_yz * d_yz + m_zz * d_zz + 2 * m_zx * d_zx]
}

//
//	Sets mtxOut = Transpose(R) * Diag(a) * R, where Diag(a) is a diagonal matrix with diagonal elements a.

HK_FORCE_INLINE void HK_CALL hkSymmetricMatrix3Util::_computeInverseBasisTransform(const hkMatrix3& R, hkVector4Parameter vD, hkSymmetricMatrix3& mtxOut)
{
	// Get columns
	const hkVector4 c0 = R.getColumn<0>();
	const hkVector4 c1 = R.getColumn<1>();
	const hkVector4 c2 = R.getColumn<2>();

	hkVector4 dc0;	dc0.setMul(c0, vD);
	hkVector4 dc1;	dc1.setMul(c1, vD);
	hkVector4 dc2;	dc2.setMul(c2, vD);

	hkVector4Util::dot3_3vs3(dc0, c0, dc1, c1, dc2, c2, mtxOut.m_diag);
	hkVector4Util::dot3_3vs3(dc0, c1, dc1, c2, dc2, c0, mtxOut.m_offDiag);
}

//
//	Sets inverseOut = Inverse(t).

HK_FORCE_INLINE void HK_CALL hkSymmetricMatrix3Util::_computeInverse(const hkSymmetricMatrix3& t, hkSymmetricMatrix3& inverseOut)
{
	const hkVector4 d_xyz	= t.m_diag;													// [m00, m11, m22]
	const hkVector4 od_xyz	= t.m_offDiag;												// [m01, m12, m20]

	hkVector4 d_yzx;		d_yzx.setPermutation<hkVectorPermutation::YZXW>(d_xyz);		// [m11, m22, m00]
	hkVector4 d_zxy;		d_zxy.setPermutation<hkVectorPermutation::ZXYW>(d_xyz);		// [m22, m00, m11]
	hkVector4 diag;			diag.setMul(d_yzx, d_zxy);									// [m11 * m22, m22 * m00, m00 * m11]
	hkVector4 det;			det.setMul(d_xyz, diag);									// [m00 * m11 * m22, *]

	hkVector4 od_yzx;		od_yzx.setPermutation<hkVectorPermutation::YZXW>(od_xyz);	// [m12, m20, m01]
	hkVector4 od_zxy;		od_zxy.setPermutation<hkVectorPermutation::ZXYW>(od_xyz);	// [m20, m01, m12]
	hkVector4 od_yzx_Sq;	od_yzx_Sq.setMul(od_yzx, od_yzx);							// [m12 * m12, m20 * m20, m01 * m01]
	diag.sub(od_yzx_Sq);																// [m11 * m22 - m12 * m12, m22 * m00 - m20 * m20, m00 * m11 - m01 * m01]

	hkVector4 offDiag;		offDiag.setMul(od_yzx, od_zxy);								// [m12 * m20, m20 * m01, m01 * m12]
	hkVector4 offDiagProd;	offDiagProd.setMul(od_xyz, offDiag);						// [m01 * m12 * m20, *]
	det.addMul(hkSimdReal_2, offDiagProd);												// [m00 * m11 * m22 + 2 * m01 * m12 * m20, *]
	offDiag.subMul(d_zxy, od_xyz);														// [m12 * m20 - m22 * m01, m20 * m01 - m00 * m12, m01 * m12 - m11 * m20]

	// Compute and invert determinant
	const hkSimdReal determinant	= det.getComponent<0>() - d_xyz.dot<3>(od_yzx_Sq);	// m00 * m11 * m22 + 2 * m01 * m12 * m20 - m00 * m12 * m12 - m11 * m20 * m20 - m22 * m01 * m01
	hkSimdReal invDet;				invDet.setSelect(determinant.greater(hkSimdReal_Eps), determinant.reciprocal(), hkSimdReal_0);

	// Store inverse tensor
	inverseOut.m_diag.setMul(diag, invDet);
	inverseOut.m_offDiag.setMul(offDiag, invDet);
}

//
//	Computes vOut = t * vIn

HK_FORCE_INLINE void HK_CALL hkSymmetricMatrix3Util::_multiply(const hkSymmetricMatrix3& t, hkVector4Parameter vIn, hkVector4& vOut)
{
	hkVector4 tmp0;		tmp0.setMul(t.m_diag, vIn);								// [m00 * x, m11 * y, m22 * z]
	hkVector4 tmp1;		tmp1.setMul(t.m_offDiag, vIn);							// [m01 * x, m12 * y, m20 * z]
	hkVector4 tmp2;		tmp2.setPermutation<hkVectorPermutation::YZXW>(vIn);	// [y, z, x, w]
	tmp1.setPermutation<hkVectorPermutation::ZXYW>(tmp1);						// [m20 * z, m01 * x, m12 * y]

	tmp0.addMul(t.m_offDiag, tmp2);												// [m00 * x + m01 * y, m11 * y + m12 * z, m22 * z + m20 * x]
	vOut.setAdd(tmp0, tmp1);													// [m00 * x + m01 * y + m20 * z, m01 * x + m11 * y + m12 * z, m20 * x + m12 * y + m22 * z]
}

//
//	Solves the linear system: mtx * v = b, for v.

HK_FORCE_INLINE void HK_CALL hkSymmetricMatrix3Util::_solve(const hkSymmetricMatrix3& mtx, hkVector4Parameter b, hkVector4& solutionOut)
{
	hkSymmetricMatrix3 inv;
	hkSymmetricMatrix3Util::_computeInverse(mtx, inv);
	hkSymmetricMatrix3Util::_multiply(inv, b, solutionOut);
}

//
//	Solves the linear system: mtx * v = b for v, where |v| < vMax per component.
//	The problem is a LCP and the returned solution is an approximation computed via Jacobi iterations.
//	The number of iterations is given as a template parameter

template <int NIter>
HK_FORCE_INLINE void HK_CALL hkSymmetricMatrix3Util::_solve(const hkSymmetricMatrix3& mtx, hkVector4Parameter b, hkVector4Parameter vMax, hkVector4& solutionOut)
{
	// Compute the unbounded solution
	hkVector4 x;	hkSymmetricMatrix3Util::_solve(mtx, b, x);
	hkVector4 vMin;	vMin.setNeg<3>(vMax);

	// Applying the limit makes the problem a LCP. Run a few Jacobi iterations
	// to compute an approximate solution that obeys the limits.
	hkVector4 invD;	invD.setReciprocal<HK_ACC_23_BIT, HK_DIV_SET_ZERO>(mtx.m_diag);
	for (int iter = NIter - 1; iter >= 0; iter--)
	{
		// Compute new solution
		hkVector4 tmp1;		tmp1.setMul(mtx.m_offDiag, x);						// [m01 * x, m12 * y, m20 * z]
		hkVector4 tmp2;		tmp2.setPermutation<hkVectorPermutation::YZXW>(x);	// [y, z, x, w]
		tmp1.setPermutation<hkVectorPermutation::ZXYW>(tmp1);					// [m20 * z, m01 * x, m12 * y]
		x.setAddMul(tmp1, mtx.m_offDiag, tmp2);									// [m01 * y + m20 * z, m01 * x + m12 * z, m20 * x + m12 * y]
		x.setSub(b, x);
		x.mul(invD);

		// Clamp to interval
		x.setMin(x, vMax);
		x.setMax(x, vMin);
	}

	solutionOut = x;
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
