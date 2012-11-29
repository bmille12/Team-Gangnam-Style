/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2012 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */

//
//	Sets this matrix to be the reflection of matrix D through matrix P, i.e. this = Transpose(P) * A * P
//	Matrix D is assumed diagonal.

HK_FORCE_INLINE void HK_CALL hkMatrix3Util::_computeReflectedDiagonal(const hkMatrix3& P, hkVector4Parameter D, hkMatrix3& reflectionOut)
{
	// Resulting matrix is symmetric and element(i, j) = dot3(P(*, i) * P(*, j), D)
	// Get columns of p
	const hkVector4 p0 = P.getColumn<0>();
	const hkVector4 p1 = P.getColumn<1>();
	const hkVector4 p2 = P.getColumn<2>();

	// Compute dots
	hkVector4 p00;	p00.setMul(p0, p0);
	hkVector4 p01;	p01.setMul(p0, p1);
	hkVector4 p02;	p02.setMul(p0, p2);
	hkVector4 p11;	p11.setMul(p1, p1);
	hkVector4 p12;	p12.setMul(p1, p2);
	hkVector4 p22;	p22.setMul(p2, p2);

	hkVector4 dotsA;	hkVector4Util::dot3_1vs4(D, p00, p01, p02, p11, dotsA);	// (m00, m01, m02, m11)
	hkVector4 dotsB;	hkVector4Util::dot3_1vs4(D, p02, p12, p22, p11, dotsB);	// (m02, m12, m22, m11)

	const hkVector4 mCol0 = dotsA;
	const hkVector4 mCol2 = dotsB;
	hkVector4 mCol1;

#if (HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED)
	// dotsA = (m01, m11, m02, m00)
	dotsA.setPermutation<hkVectorPermutation::YWZX>(dotsA);

	// dotsB = (m12, m12, m12, m12)
	dotsB.broadcast<1>();

	hkVector4Comparison cmp;
	cmp.set(hkVector4Comparison::MASK_XY);
	mCol1.setSelect(cmp, dotsA, dotsB);
#else
	mCol1.set(dotsA(1), dotsA(3), dotsB(1), dotsB(1));
#endif

	reflectionOut.setCols(mCol0, mCol1, mCol2);
}

//
//	Sets this = Q * D * Inverse(Q), where D is interpreted as a diagonal matrix.

HK_FORCE_INLINE void HK_CALL hkMatrix3Util::_changeBasis(hkQuaternionParameter Q, hkVector4Parameter D, hkMatrix3& matrixOut)
{
	hkRotation R;
	hkVector4Util::convertQuaternionToRotation(Q, R);

	hkMatrix3Util::_changeBasis(R, D, matrixOut);
}

//
//	Changes the basis of the the given diagonal matrix by the rotation matrix t. (this = R * D * Transpose(R))

HK_FORCE_INLINE void HK_CALL hkMatrix3Util::_changeBasis(const hkRotation& R, hkVector4Parameter D, hkMatrix3& matrixOut)
{
	// Get cols
	hkVector4 l0	= R.getColumn<0>();		// [m00, m10, m20]
	hkVector4 l1	= R.getColumn<1>();		// [m01, m11, m21]
	hkVector4 l2	= R.getColumn<2>();		// [m02, m12, m22]

	hkVector4 diag;
	{
		hkVector4 t0;	t0.setMul(l0, l0);	t0.mul(D.getComponent<0>());	// [d0 * m00 * m00, d0 * m10 * m10, d0 * m20 * m20]
		hkVector4 t1;	t1.setMul(l1, l1);	t1.mul(D.getComponent<1>());	// [d1 * m01 * m01, d1 * m11 * m11, d1 * m21 * m21]
		hkVector4 t2;	t2.setMul(l2, l2);	t2.mul(D.getComponent<2>());	// [d2 * m02 * m02, d2 * m12 * m12, d2 * m22 * m22]

		// In the end, diag is:
		//	[d0 * m00 * m00 + d1 * m01 * m01 + d2 * m02 * m02,
		//		d0 * m10 * m10 + d1 * m11 * m11 + d2 * m12 * m12,
		//			d0 * m20 * m20 + d1 * m21 * m21 + d2 * m22 * m22] = [a00, a11, a22]
		t0.add(t1);
		diag.setAdd(t0, t2);	
	}

	hkVector4 offDiag;
	{
		HK_TRANSPOSE3(l0, l1, l2);				// Transpose to get the lines
		hkVector4 l01;	l01.setMul(l0, l1);		// [m00 * m10, m01 * m11, m02 * m12]
		hkVector4 l02;	l02.setMul(l0, l2);		// [m00 * m20, m01 * m21, m02 * m22]
		hkVector4 l12;	l12.setMul(l1, l2);		// [m10 * m20, m11 * m21, m12 * m22]

		// In the end, offDiag is:
		//	[d0 * m00 * m10 + d1 * m01 * m11 + d2 * m02 * m12, 
		//		d0 * m00 * m20 + d1 * m01 * m21 + d2 * m02 * m22,
		//			d0 * m10 * m20 + d1 * m11 * m21 + d2 * m12 * m22] = [a01, a02, a12]
		hkVector4Util::dot3_1vs3(D, l01, l02, l12, offDiag);
	}

	// Store results
	{
		hkVector4Comparison maskX;	maskX.set<hkVector4Comparison::MASK_X>();
		hkVector4Comparison maskZ;	maskZ.set<hkVector4Comparison::MASK_Z>();
		hkVector4Comparison maskXZ;	maskXZ.setOr(maskX, maskZ);
		hkVector4 tmp0;	tmp0.setPermutation<hkVectorPermutation::WXYZ>(offDiag);	// [*, a01, a02, a12]
		hkVector4 tmp1;	tmp1.setPermutation<hkVectorPermutation::YZWX>(offDiag);	// [a02, a12, *, a01]

		hkVector4 mCol0, mCol1, mCol2;
		mCol0.setSelect(maskX, diag, tmp0);		// [a00, a01, a02]
		mCol1.setSelect(maskXZ, offDiag, diag);	// [a01, a11, a12]
		mCol2.setSelect(maskZ, diag, tmp1);		// [a02, a12, a22]
		matrixOut.setCols(mCol0, mCol1, mCol2);
	}
}

//
//	Sets this = Identity * dot(v, v) - OuterProduct(v, v). The matrix is used to displace inertia tensors
//	by v from the center of mass.

HK_FORCE_INLINE void HK_CALL hkMatrix3Util::_computeInertiaDisplacement(hkVector4Parameter v, hkMatrix3& inertiaOut)
{
	// Compute dots
	hkVector4 vvx;	vvx.setZero();	vvx.setComponent<0>(v.dot<3>(v));	// [xx + yy + zz, 0, 0]
	hkVector4 vvy;	vvy.setPermutation<hkVectorPermutation::YXZW>(vvx);	// [0, xx + yy + zz, 0]
	hkVector4 vvz;	vvz.setPermutation<hkVectorPermutation::YZXW>(vvx);	// [0, 0, xx + yy + zz]

	// Subtract 
	hkVector4 mCol0, mCol1, mCol2;
	mCol0.setSubMul(vvx, v, v.getComponent<0>());	// [yy + zz, -xy, -xz]
	mCol1.setSubMul(vvy, v, v.getComponent<1>());	// [-xy, xx + zz, -yz]
	mCol2.setSubMul(vvz, v, v.getComponent<2>());	// [-xz, -yz, xx + yy]
	inertiaOut.setCols(mCol0, mCol1, mCol2);
}

//
//	Returns the diagonal of the given matrix.

HK_FORCE_INLINE	void HK_CALL hkMatrix3Util::_getDiagonal(const hkMatrix3& matrixIn, hkVector4& diagonalOut)
{
	diagonalOut = matrixIn.getColumn<0>();
	diagonalOut.setSelect<hkVector4Comparison::MASK_Y>(matrixIn.getColumn<1>(), diagonalOut);
	diagonalOut.setSelect<hkVector4Comparison::MASK_Z>(matrixIn.getColumn<2>(), diagonalOut);
}

//
//	Returns the off-diagonal elements of this matrix. The matrix is assumed to be symmetric. The returned vector is
//	[m01, m02, m12]

HK_FORCE_INLINE void HK_CALL hkMatrix3Util::_getSymmetricOffDiagonal(const hkMatrix3& matrixIn, hkVector4& offDiagonalOut)
{
	offDiagonalOut.set(matrixIn.getElement<0,1>(), matrixIn.getElement<0,2>(), matrixIn.getElement<1,2>(), hkSimdReal_0);
}

//
//	Returns the maximum element

HK_FORCE_INLINE const hkSimdReal HK_CALL hkMatrix3Util::_computeMaxAbsElement(const hkMatrix3& matrixIn)
{
	hkVector4 vMax;
	vMax.setMax(matrixIn.getColumn<0>(), matrixIn.getColumn<1>());
	vMax.setMax(matrixIn.getColumn<2>(), vMax);
	vMax.setAbs(vMax);
	return vMax.horizontalMax<3>();
}

//
//	Computes the coefficients of the characteristic polynomial
//	Let matrix be:
//		[m00, m01, m02]
//		[m01, m11, m12]
//		[m02, m12, m22]
//
//	Coefficients of the characteristic polynomial are:
//		a3 = 1
//		a2 = -(m00 + m11 + m22)
//		a1 = (m00 * m11 + m11 * m22 + m22 * m00) - (Sqr(m01) + Sqr(m02) + Sqr(m12))
//		a0 = (m00 * Sqr(m12) + m11 * Sqr(m02) + m22 * Sqr(m01)) - m00 * m11 * m22 - 2 * m01 * m02 * m12

HK_FORCE_INLINE void HK_CALL hkMatrix3Util::_computeCharacteristicPolynomial(const hkMatrix3& matrixIn, hkVector4& coeffsOut)
{
	hkVector4 d;	hkMatrix3Util::_getDiagonal(matrixIn, d);				// [m00, m11, m22]
	hkVector4 od;	hkMatrix3Util::_getSymmetricOffDiagonal(matrixIn, od);	// [m01, m02, m12]

	// Compute polynomial coefficients
	hkVector4 rd;		rd.setPermutation<hkVectorPermutation::YZXW>(d);	// [m11, m22, m00]
	hkVector4 rod;		rod.setPermutation<hkVectorPermutation::ZYXZ>(od);	// [m12, m02, m01]
	rod.mul(rod);															// [Sqr(m12), Sqr(m02), Sqr(m01)]

	// dots = [	Sqr(m01) + Sqr(m02) + Sqr(m12), 
	//			m00 * m11 + m11 * m22 + m22 * m00,
	//			m00 * Sqr(m12) + m11 * Sqr(m02) + m22 * Sqr(m01) ]
	hkVector4 dots;
	hkVector4Util::dot3_3vs3(od, od, d, rd, d, rod, dots);
	const hkSimdReal c1 = dots.getComponent<1>() - dots.getComponent<0>();	// (m00 * m11 + m11 * m22 + m22 * m00) - (Sqr(m01) + Sqr(m02) + Sqr(m12))

	// Transpose, to compute horizontal add / mul			// d	= [m00, m00, m01]
	rd = d;													// rd	= [m11, m11, m02]
	HK_TRANSPOSE3(d, rd, od);								// od	= [m22, m22, m12]
	hkVector4 ha;	ha.setAdd(d, rd);	ha.add(od);			// [m00 + m11 + m22]
	hkVector4 hm;	hm.setMul(d, rd);	hm.mul(od);			// [*, m00 * m11 * m22, m01 * m02 * m12]

	const hkSimdReal c2 = ha.getComponent<0>();				// (m00 + m11 + m22)
	const hkSimdReal c0 = dots.getComponent<2>() - hm.getComponent<1>() - hkSimdReal_2 * hm.getComponent<2>();

	coeffsOut.set(hkSimdReal_1, -c2, c1, c0);
}

//
//	Computes the eigenvalues analytically, using Cardano's formula. The matrix is assumed to be symmetric.
//	Code adapted from http://www.mpi-hd.mpg.de/personalhomes/globes/3x3/

HK_FORCE_INLINE void HK_CALL hkMatrix3Util::_computeSymmetricEigenValues(const hkMatrix3& matrixIn, hkVector4& eigenVals)
{
	// Compute characteristic polynomial
	hkVector4 vCoeffs;		hkMatrix3Util::_computeCharacteristicPolynomial(matrixIn, vCoeffs);
	hkVector4 vCoeffsSqr;	vCoeffsSqr.setMul(vCoeffs, vCoeffs);	// [*, Sqr(a2), Sqr(a1), Sqr(a0)]

	const hkSimdReal a2		= vCoeffs.getComponent<1>();			// -m
	const hkSimdReal a1		= vCoeffs.getComponent<2>();			// c1
	const hkSimdReal a0		= vCoeffs.getComponent<3>();			// c0
	const hkSimdReal Sqr_a2	= vCoeffsSqr.getComponent<1>();			// Sqr(a2)
	const hkSimdReal Sqr_a1 = vCoeffsSqr.getComponent<2>();			// Sqr(a1)

	const hkSimdReal c_27		= hkSimdReal::fromInt32(27);		// 27
	const hkSimdReal c_27_2		= c_27 * hkSimdReal_Inv2;			// (27 / 2)
	const hkSimdReal c_27_4		= c_27 * hkSimdReal_Inv4;			// (27 / 4)
	const hkSimdReal c_3_2		= hkSimdReal_3 * hkSimdReal_Inv2;	// (3 / 2)

	hkSimdReal p			= Sqr_a2 - hkSimdReal_3 * a1;
	hkSimdReal q			= a2 * (c_3_2 * a1 - p) - c_27_2 * a0;
	hkSimdReal abs_p;		abs_p.setAbs(p);
	const hkSimdReal sqrt_p	= abs_p.sqrt();
	hkSimdReal phi			= c_27 * (hkSimdReal_Inv4 * Sqr_a1 * (p - a1) + a0 * (q + c_27_4 * a0));
	hkSimdReal abs_phi;		abs_phi.setAbs(phi);
	phi = hkSimdReal_Inv3 * hkSimdReal::fromFloat(hkMath::atan2(abs_phi.sqrt().getReal(), q.getReal()));

	hkVector4 vSinCos;
	
//#if defined(HK_PLATFORM_WIN32) && (HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED)
//	vSinCos.m_quad = hkMath::quadSinCos(phi.m_real);
//#else
	vSinCos.set(hkMath::sin(phi.getReal()), hkMath::cos(phi.getReal()), 0.0f);
//#endif
	vSinCos.mul(sqrt_p);

	const hkSimdReal rsqrt_3	= hkSimdReal::fromFloat(0.57735026918962576450914878050058f);	// 1 / sqrt(3)
	const hkSimdReal c			= vSinCos.getComponent<1>();
	const hkSimdReal s			= rsqrt_3 * vSinCos.getComponent<0>();
	const hkSimdReal w			= -hkSimdReal_Inv3 * (a2 + c);

	eigenVals.set(w + c, w - s, w + s, hkSimdReal_0);

	// Run a Newton-Raphson iteration to improve the precision
	{
		hkVector4 fx, dfx;
		hkVector4 va0, va1, va2;	va0.setAll(a0);	va1.setAll(a1);	va2.setAll(a2);

		fx.setAdd(va2, eigenVals);						// fx = (x + a2)
		fx.setAddMul(va1, fx, eigenVals);				// fx = (x^2 + a2 * x + a1)
		fx.setAddMul(va0, fx, eigenVals);				// fx = (x^3 + a2 * x^2 + a1 * x + a0)

		va2.mul(hkSimdReal_2);
		dfx.setAddMul(va2, eigenVals, hkSimdReal_3);	// dfx = (3 * x + 2 * a2)
		dfx.setAddMul(va1, dfx, eigenVals);				// dfx = (3 * x^2 + 2 * a2 * x + a1)

		hkVector4 inc;	inc.setDiv(fx, dfx);
		eigenVals.sub(inc);
	}
}

//
//	Computes matrixOut = matrixIn * hat(v), whereh hat(v) is the skew-symmetric matrix of v

HK_FORCE_INLINE void HK_CALL hkMatrix3Util::_computeCrossProduct(const hkMatrix3& matrixIn, hkVector4Parameter v, hkMatrix3& matrixOut)
{
	const hkSimdReal x = v.getComponent<0>();
	const hkSimdReal y = v.getComponent<1>();
	const hkSimdReal z = v.getComponent<2>();

	// Get source columns
	const hkVector4 sc0 = matrixIn.getColumn<0>();
	const hkVector4 sc1 = matrixIn.getColumn<1>();
	const hkVector4 sc2 = matrixIn.getColumn<2>();

	// Compute destination columns
	hkVector4 dc0;	dc0.setMul(sc1, z);	dc0.subMul(sc2, y);
	hkVector4 dc1;	dc1.setMul(sc2, x);	dc1.subMul(sc0, z);
	hkVector4 dc2;	dc2.setMul(sc0, y);	dc2.subMul(sc1, x);
	matrixOut.setCols(dc0, dc1, dc2);
}

//
//	Sets this = Transpose(a) * b.

HK_FORCE_INLINE void hkMatrix3Util::_computeTransposeMul(const hkMatrix3& a, const hkMatrix3& b, hkMatrix3& aTb)
{
	hkVector4 c0, c1, c2;
	hkVector4Util::dot3_3vs3(a.getColumn<0>(), b.getColumn<0>(), a.getColumn<1>(), b.getColumn<0>(), a.getColumn<2>(), b.getColumn<0>(), c0);
	hkVector4Util::dot3_3vs3(a.getColumn<0>(), b.getColumn<1>(), a.getColumn<1>(), b.getColumn<1>(), a.getColumn<2>(), b.getColumn<1>(), c1);
	hkVector4Util::dot3_3vs3(a.getColumn<0>(), b.getColumn<2>(), a.getColumn<1>(), b.getColumn<2>(), a.getColumn<2>(), b.getColumn<2>(), c2);

	aTb.setCols(c0, c1, c2);
}

//
//	Sets this matrix as the cross product between the given vector and each of the given matrix's columns.
//	Specifically, it sets mtxOut.getColumn(i) = cross(v, src.getColumn(i))

HK_FORCE_INLINE void HK_CALL hkMatrix3Util::_computeCrossMul(hkVector4Parameter v, const hkMatrix3& src, hkMatrix3& mtxOut)
{
	// Get source columns
	const hkVector4 sc0 = src.getColumn<0>();
	const hkVector4 sc1 = src.getColumn<1>();
	const hkVector4 sc2 = src.getColumn<2>();

	// Target columns
	hkVector4 tc0, tc1, tc2;

#if (HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED)
	// First part
	{
		hkVector4 shufV;	// (vy, vz, vx, *)
		shufV.setPermutation<hkVectorPermutation::YZXW>(v);

		hkVector4 shufC0;	// (c0z, c0x, c0y)
		shufC0.setPermutation<hkVectorPermutation::ZXYW>(sc0);

		hkVector4 shufC1;	// (c1z, c1x, c1y)
		shufC1.setPermutation<hkVectorPermutation::ZXYW>(sc1);

		hkVector4 shufC2;	// (c2z, c2x, c2y)
		shufC2.setPermutation<hkVectorPermutation::ZXYW>(sc2);

		tc0.setMul(shufV, shufC0);	// (vy * c0z, vz * c0x, vx * c0y)
		tc1.setMul(shufV, shufC1);	// (vy * c1z, vz * c1x, vx * c1y)
		tc2.setMul(shufV, shufC2);	// (vy * c2z, vz * c2x, vx * c2y)
	}

	// Second part
	{
		hkVector4 shufV;	// (vz, vx, vy, *)
		shufV.setPermutation<hkVectorPermutation::ZXYW>(v);

		hkVector4 shufC0;	// (c0y, c0z, c0x)
		shufC0.setPermutation<hkVectorPermutation::YZXW>(sc0);

		hkVector4 shufC1;	// (c1y, c1z, c1x)
		shufC1.setPermutation<hkVectorPermutation::YZXW>(sc1);

		hkVector4 shufC2;	// (c2y, c2z, c2x)
		shufC2.setPermutation<hkVectorPermutation::YZXW>(sc2);

		tc0.subMul(shufV, shufC0);	// (vy * c0z - vz * c0y, vz * c0x - vx * c0z, vx * c0y - vy * c0x)
		tc1.subMul(shufV, shufC1);	// (vy * c1z - vz * c1y, vz * c1x - vx * c1z, vx * c1y - vy * c1x)
		tc2.subMul(shufV, shufC2);	// (vy * c2z - vz * c2y, vz * c2x - vx * c2z, vx * c2y - vy * c2x)
	}
#else
	tc0.set( v(1) * sc0(2) - v(2) * sc0(1), v(2) * sc0(0) - v(0) * sc0(2), v(0) * sc0(1) - v(1) * sc0(0) );
	tc1.set( v(1) * sc1(2) - v(2) * sc1(1), v(2) * sc1(0) - v(0) * sc1(2), v(0) * sc1(1) - v(1) * sc1(0) );
	tc2.set( v(1) * sc2(2) - v(2) * sc2(1), v(2) * sc2(0) - v(0) * sc2(2), v(0) * sc2(1) - v(1) * sc2(0) );
#endif

	// Store
	mtxOut.setCols(tc0, tc1, tc2);
}

//
//	Checks if matrixIn is equal to the identity matrix within an epsilon.

HK_FORCE_INLINE hkBool32 HK_CALL hkMatrix3Util::_isApproximatelyIdentity(const hkMatrix3& matrixIn, hkSimdRealParameter eps)
{
	// non_euclidean, manhattan based
	hkVector4 t0,t1,t2;
	t0.setSub(matrixIn.getColumn<0>(), hkVector4::getConstant<HK_QUADREAL_1000>());
	t0.setAbs( t0 );
	t1.setSub(matrixIn.getColumn<1>(), hkVector4::getConstant<HK_QUADREAL_0100>());
	t1.setAbs( t1 );
	t2.setSub(matrixIn.getColumn<2>(), hkVector4::getConstant<HK_QUADREAL_0010>());
	t2.setAbs( t2 );

	hkVector4 epsilon_v;
	epsilon_v.setAll(eps);

	hkVector4Comparison cmp0 = t0.less( epsilon_v );
	hkVector4Comparison cmp1 = t1.less( epsilon_v );
	hkVector4Comparison cmp2 = t2.less( epsilon_v );

	hkVector4Comparison all; 
	all.setAnd(cmp0,cmp1);
	all.setAnd(cmp2,all);

	return all.allAreSet(hkVector4Comparison::MASK_XYZ);
}

HK_FORCE_INLINE void HK_CALL hkMatrix3Util::_setDiagonal( hkReal m00, hkReal m11, hkReal m22, hkMatrix3& mtxOut )
{
	mtxOut.setZero();
	mtxOut.setElement<0, 0>(hkSimdReal::fromFloat(m00));
	mtxOut.setElement<1, 1>(hkSimdReal::fromFloat(m11));
	mtxOut.setElement<2, 2>(hkSimdReal::fromFloat(m22));
}

HK_FORCE_INLINE void HK_CALL hkMatrix3Util::_setDiagonal( hkSimdRealParameter m00, hkSimdRealParameter m11, hkSimdRealParameter m22, hkMatrix3& mtxOut )
{
#if (HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED)
	hkVector4 c0;	c0.setMul(hkVector4::getConstant<HK_QUADREAL_1000>(), m00);
	hkVector4 c1;	c1.setMul(hkVector4::getConstant<HK_QUADREAL_0100>(), m11);
	hkVector4 c2;	c2.setMul(hkVector4::getConstant<HK_QUADREAL_0010>(), m22);
	mtxOut.setCols(c0, c1, c2);
#else
	mtxOut.setZero();
	mtxOut.setElement<0, 0>(m00);
	mtxOut.setElement<1, 1>(m11);
	mtxOut.setElement<2, 2>(m22);
#endif
}

HK_FORCE_INLINE void HK_CALL hkMatrix3Util::_setDiagonal( hkVector4Parameter vDiagonal, hkMatrix3& mtxOut )
{
#if (HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED)
	hkVector4 c0;	c0.setMul(hkVector4::getConstant<HK_QUADREAL_1000>(), vDiagonal);
	hkVector4 c1;	c1.setMul(hkVector4::getConstant<HK_QUADREAL_0100>(), vDiagonal);
	hkVector4 c2;	c2.setMul(hkVector4::getConstant<HK_QUADREAL_0010>(), vDiagonal);
	mtxOut.setCols(c0, c1, c2);
#else
	mtxOut.setZero();
	mtxOut.setElement<0, 0>(vDiagonal.getComponent<0>());
	mtxOut.setElement<1, 1>(vDiagonal.getComponent<1>());
	mtxOut.setElement<2, 2>(vDiagonal.getComponent<2>());
#endif
}

HK_FORCE_INLINE void HK_CALL hkMatrix3Util::_setDiagonal(hkSimdRealParameter diag, hkMatrix3& mtxOut)
{
#if (HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED)
	hkVector4 c0;	c0.setMul(hkVector4::getConstant<HK_QUADREAL_1000>(), diag);
	hkVector4 c1;	c1.setMul(hkVector4::getConstant<HK_QUADREAL_0100>(), diag);
	hkVector4 c2;	c2.setMul(hkVector4::getConstant<HK_QUADREAL_0010>(), diag);
	mtxOut.setCols(c0, c1, c2);
#else
	mtxOut.setZero();
	mtxOut.setElement<0, 0>(diag);
	mtxOut.setElement<1, 1>(diag);
	mtxOut.setElement<2, 2>(diag);
#endif
}

HK_FORCE_INLINE void HK_CALL hkMatrix3Util::_setDiagonalOnly( hkVector4Parameter vDiagonal, hkMatrix3& mtxOut )
{
#if (HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED)
	hkVector4 c0;	c0.setSelect<hkVector4Comparison::MASK_X>(vDiagonal, mtxOut.getColumn<0>());
	hkVector4 c1;	c1.setSelect<hkVector4Comparison::MASK_Y>(vDiagonal, mtxOut.getColumn<1>());
	hkVector4 c2;	c2.setSelect<hkVector4Comparison::MASK_Z>(vDiagonal, mtxOut.getColumn<2>());
	mtxOut.setCols(c0, c1, c2);
#else
	mtxOut.setElement<0, 0>(vDiagonal.getComponent<0>());
	mtxOut.setElement<1, 1>(vDiagonal.getComponent<1>());
	mtxOut.setElement<2, 2>(vDiagonal.getComponent<2>());
#endif
}

HK_FORCE_INLINE void HK_CALL hkMatrix3Util::_setDiagonalOnly(hkSimdRealParameter diag, hkMatrix3& mtxOut)
{
#if (HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED)
	hkVector4 diagV; diagV.setAll(diag);
	hkVector4 c0;	c0.setSelect<hkVector4Comparison::MASK_X>(diagV, mtxOut.getColumn<0>());
	hkVector4 c1;	c1.setSelect<hkVector4Comparison::MASK_Y>(diagV, mtxOut.getColumn<1>());
	hkVector4 c2;	c2.setSelect<hkVector4Comparison::MASK_Z>(diagV, mtxOut.getColumn<2>());
	mtxOut.setCols(c0, c1, c2);
#else
	mtxOut.setElement<0, 0>(diag);
	mtxOut.setElement<1, 1>(diag);
	mtxOut.setElement<2, 2>(diag);
#endif
}

//
//	Computes mtxOut to be the product of a and the inverse of b. (mtxOut = a * b^-1)

HK_FORCE_INLINE void HK_CALL hkMatrix3Util::_computeMulInverse(const hkMatrix3& mtxA, const hkRotation& mtxB, hkMatrix3& mtxOut)
{
	hkRotation invB;	invB._setTranspose(mtxB);
	hkMatrix3 ret;		hkVector4Util::rotatePoints(mtxA, &invB.getColumn<0>(), 3, &ret.getColumn<0>());
	mtxOut = ret;
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
