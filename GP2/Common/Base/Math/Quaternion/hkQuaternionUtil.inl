/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2012 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */

//
//	Computes a 4x4 matrix that allows you to compute any product of the form (this * q) by multiplying (opOut * q)
//	Let:
//		this	= (pV, pS)
//		q		= (qV, qS)
//	Then:
//		this * q	=	(pV * qS + pS * qV + hat(pV) * qV, pS * qS - dot(pV, qV)) = opOut * q
//		opOut		=	| (pS * Identity(3) + hat(pV))		pV	|
//						| -Transpose(pV)					pS	|
//	If:
//		this	= (x, y, z, w), pS = w, pV = (x, y, z)
//		opOut	=	|	 w		-z		 y		x	|
//					|	 z		 w		-x		y	|
//					|	-y		 x		 w		z	|
//					|	-x		-y		-z		w	|

HK_FORCE_INLINE void HK_CALL hkQuaternionUtil::_computePostMultiplyOperator(hkQuaternionParameter q, hkMatrix4& opOut)
{
	// Shuffle the quantities
	hkVector4 c0;	c0.setPermutation<hkVectorPermutation::WZYX>(q.m_vec);	// c0 = (w, z, y, x)
	hkVector4 c1;	c1.setPermutation<hkVectorPermutation::ZWXY>(q.m_vec);	// c1 = (z, w, x, y)
	hkVector4 c2;	c2.setPermutation<hkVectorPermutation::YXWZ>(q.m_vec);	// c2 = (y, x, w, z)

	hkVector4Comparison s0;	s0.set<hkVector4Comparison::MASK_ZW>();			// s0 = (1, 1, -1, -1)
	hkVector4Comparison s1;	s1.set<hkVector4Comparison::MASK_XW>();			// s1 = (-1, 1, 1, -1)
	hkVector4Comparison s2;	s2.set<hkVector4Comparison::MASK_YW>();			// s2 = (1, -1, 1, -1)

	// Set-up the output matrix
	c0.setFlipSign(c0, s0);
	c1.setFlipSign(c1, s1);
	c2.setFlipSign(c2, s2);
	opOut.setCols(c0, c1, c2, q.m_vec);
}

//
//	Computes a 4x4 matrix that allows you to compute any product of the form (q * this) by multiplying (opOut * q)
//	Let:
//		this	= (pV, pS)
//		q		= (qV, qS)
//	Then:
//		q * this	=	(pV * qS + pS * qV - hat(pV) * qV, pS * qS - dot(pV, qV)) = opOut * q
//		opOut		=	| (pS * Identity(3) - hat(pV))		pV	|
//						| -Transpose(pV)					pS	|
//	If:
//		this	= (x, y, z, w), pS = w, pV = (x, y, z)
//		opOut	=	|	 w		 z		-y		x	|
//					|	-z		 w		 x		y	|
//					|	 y		-x		 w		z	|
//					|	-x		-y		-z		w	|

HK_FORCE_INLINE void HK_CALL hkQuaternionUtil::_computePreMultiplyOperator(hkQuaternionParameter q, hkMatrix4& opOut)
{
	// Shuffle the quantities
	hkVector4 c0;	c0.setPermutation<hkVectorPermutation::WZYX>(q.m_vec);	// c0 = (w, z, y, x)
	hkVector4 c1;	c1.setPermutation<hkVectorPermutation::ZWXY>(q.m_vec);	// c1 = (z, w, x, y)
	hkVector4 c2;	c2.setPermutation<hkVectorPermutation::YXWZ>(q.m_vec);	// c2 = (y, x, w, z)

	hkVector4Comparison s0;	s0.set<hkVector4Comparison::MASK_YW>();			// s0 = (1, -1, 1, -1)
	hkVector4Comparison s1;	s1.set<hkVector4Comparison::MASK_ZW>();			// s1 = (1, 1, -1, -1)
	hkVector4Comparison s2;	s2.set<hkVector4Comparison::MASK_XW>();			// s2 = (-1, 1, 1, -1)

	// Set-up the output matrix
	c0.setFlipSign(c0, s0);
	c1.setFlipSign(c1, s1);
	c2.setFlipSign(c2, s2);
	opOut.setCols(c0, c1, c2, q.m_vec);
}

//
//	Computes a 4x4 matrix that allows you to compute any product of the form (Inverse(this) * q) by multiplying (opOut * q)
//	If:
//		this	= (x, y, z, w), pS = w, pV = (x, y, z)
//		opOut	=	|	 w		 z		-y		-x	|
//					|	-z		 w		 x		-y	|
//					|	 y		-x		 w		-z	|
//					|	 x		 y		 z		 w	|

HK_FORCE_INLINE void HK_CALL hkQuaternionUtil::_computeInversePostMultiplyOperator(hkQuaternionParameter q, hkMatrix4& opOut)
{
	// Shuffle the quantities
	hkVector4 c0;	c0.setPermutation<hkVectorPermutation::WZYX>(q.m_vec);	// c0 = (w, z, y, x)
	hkVector4 c1;	c1.setPermutation<hkVectorPermutation::ZWXY>(q.m_vec);	// c1 = (z, w, x, y)
	hkVector4 c2;	c2.setPermutation<hkVectorPermutation::YXWZ>(q.m_vec);	// c2 = (y, x, w, z)
	hkVector4 c3;
	
	hkVector4Comparison s0;	s0.set<hkVector4Comparison::MASK_Y>();			// s0 = (1, -1, 1, 1)
	hkVector4Comparison s1;	s1.set<hkVector4Comparison::MASK_Z>();			// s1 = (1, 1, -1, 1)
	hkVector4Comparison s2;	s2.set<hkVector4Comparison::MASK_X>();			// s2 = (-1, 1, 1, 1)
	hkVector4Comparison s3;	s3.set<hkVector4Comparison::MASK_XYZ>();		// s3 = (-1, -1, -1, 1)

	// Set-up the output matrix
	c0.setFlipSign(c0, s0);
	c1.setFlipSign(c1, s1);
	c2.setFlipSign(c2, s2);
	c3.setFlipSign(q.m_vec, s3);
	opOut.setCols(c0, c1, c2, c3);
}

//
//	Computes a 4x4 matrix that allows you to compute any product of the form (q * Inverse(this)) by multiplying (opOut * q)
//	If:
//		this	= (x, y, z, w), pS = w, pV = (x, y, z)
//		opOut	=	|	 w		-z		 y		-x	|
//					|	 z		 w		-x		-y	|
//					|	-y		 x		 w		-z	|
//					|	 x		 y		 z		 w	|

HK_FORCE_INLINE void HK_CALL hkQuaternionUtil::_computeInversePreMultiplyOperator(hkQuaternionParameter q, hkMatrix4& opOut)
{
	hkVector4 c0;	c0.setPermutation<hkVectorPermutation::WZYX>(q.m_vec);	// c0 = (w, z, y, x)
	hkVector4 c1;	c1.setPermutation<hkVectorPermutation::ZWXY>(q.m_vec);	// c1 = (z, w, x, y)
	hkVector4 c2;	c2.setPermutation<hkVectorPermutation::YXWZ>(q.m_vec);	// c2 = (y, x, w, z)
	hkVector4 c3;

	hkVector4Comparison s0;	s0.set<hkVector4Comparison::MASK_Z>();			// s0 = (1, 1, -1, 1)
	hkVector4Comparison s1;	s1.set<hkVector4Comparison::MASK_X>();			// s1 = (-1, 1, 1, 1)
	hkVector4Comparison s2;	s2.set<hkVector4Comparison::MASK_Y>();			// s2 = (1, -1, 1, 1)
	hkVector4Comparison s3;	s3.set<hkVector4Comparison::MASK_XYZ>();		// s3 = (-1, -1, -1, 1)

	// Set-up the output matrix
	c0.setFlipSign(c0, s0);
	c1.setFlipSign(c1, s1);
	c2.setFlipSign(c2, s2);
	c3.setFlipSign(q.m_vec, s3);
	opOut.setCols(c0, c1, c2, c3);
}

//
//	Sets this = Transpose(op) * q, where op is a 4x4 matrix operator and q is a quaternion interpreted as a 4-element vector.

HK_FORCE_INLINE void HK_CALL hkQuaternionUtil::_computeTransposeMul(const hkMatrix4& op, hkQuaternionParameter qIn, hkQuaternion& qOut)
{
	hkVector4Util::dot4_1vs4(qIn.m_vec, op.getColumn<0>(), op.getColumn<1>(), op.getColumn<2>(), op.getColumn<3>(), qOut.m_vec);
}

//
//	N-lerp

HK_FORCE_INLINE void HK_CALL hkQuaternionUtil::_computeNlerp(hkQuaternionParameter q0, hkQuaternionParameter q1, hkSimdRealParameter t, hkQuaternion& qOut)
{
	hkVector4 v;
	v.setInterpolate(q0.m_vec, q1.m_vec, t);
	v.normalize<4>();
	qOut.m_vec = v;
}

//
//	Computes the logarithm of a quaternion

HK_FORCE_INLINE void HK_CALL hkQuaternionUtil::_computeLog(hkQuaternionParameter q, hkVector4& vOut)
{
	// Get axis and cos(angle)
	const hkSimdReal cosA = q.getRealPart();
	vOut = q.getImag();

	// If the angle is not zero, return angle * axis,
	// otherwise return just axis, because axis = sin(angle) * direction
	// and for very small angles, the angle / sin(angle) -> 1.
	hkSimdReal absCosA; absCosA.setAbs(cosA);
	if ( absCosA.isLess(hkSimdReal_1) )
	{
		const hkReal angle = hkMath::acos(q.getReal()); // use indep cosA
		hkSimdReal a; a.setFromFloat(angle);
		const hkReal sinA = hkMath::sin(angle);
		hkSimdReal sA; sA.setFromFloat(sinA);

		hkSimdReal absSinA; absSinA.setAbs(sA);
		const hkVector4Comparison absSinAGreaterEqualEps = absSinA.greaterEqual(hkSimdReal_Eps);

		hkSimdReal d; d.setReciprocal(sA); d.mul(a);
		hkVector4 logV; logV.setMul(vOut, d);

		vOut.setSelect(absSinAGreaterEqualEps, logV, vOut);
	}
}

//
//	Set the quaternion as the exponential of the given (axis-angle) vector

HK_FORCE_INLINE void HK_CALL hkQuaternionUtil::_computeExp(hkVector4Parameter vIn, hkQuaternion& qOut)
{
	const hkSimdReal angle = vIn.length<3>();

	hkSimdReal sA; sA.setFromFloat(hkMath::sin(angle.getReal()));
	hkSimdReal w; w.setFromFloat(hkMath::cos(angle.getReal()));

	hkSimdReal absSinA; absSinA.setAbs(sA);
	const hkVector4Comparison absSinAGreaterEqualEps = absSinA.greaterEqual(hkSimdReal_Eps);

	hkSimdReal d; d.setReciprocal(angle); d.mul(sA);
	hkVector4 expV; expV.setMul(vIn, d);

	hkVector4 quatDir; quatDir.setSelect(absSinAGreaterEqualEps, expV, vIn);

	qOut.m_vec.setXYZ_W(quatDir, w);
}

HK_FORCE_INLINE void HK_CALL hkQuaternionUtil::_computeExp_Approximate(hkVector4Parameter vIn, hkQuaternion& qOut)
{
#if defined(HK_PLATFORM_WIN32) && (HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED)
	hkVector4 angle; angle.setAll(vIn.length<3>());

	hkVector4 sinCos; sinCos.m_quad = hkMath::quadSinCos(angle.m_quad);
	hkSimdReal sA = sinCos.getComponent<0>();

	hkSimdReal absSinA; absSinA.setAbs(sA);
	const hkVector4Comparison absSinAGreaterEqualEps = absSinA.greaterEqual(hkSimdReal::getConstant<HK_QUADREAL_EPS>());

	hkSimdReal d; d.setReciprocal<HK_ACC_12_BIT,HK_DIV_IGNORE>(angle.getComponent<0>()); d.mul(sA);
	hkVector4 expV; expV.setMul(vIn, d);

	hkVector4 quatDir; quatDir.setSelect(absSinAGreaterEqualEps, expV, vIn);

	qOut.m_vec.setXYZ_W(quatDir, sinCos.getComponent<1>());
#else
	hkQuaternionUtil::_computeExp(vIn, qOut);
#endif
}

HK_FORCE_INLINE void HK_CALL hkQuaternionUtil::_computeShortestRotation(hkVector4Parameter from, hkVector4Parameter to, hkQuaternion& qOut)
{
	HK_MATH_ASSERT(0xad87ba22, from.isNormalized<3>() && to.isNormalized<3>(), "hkQuaternion::setShortestRotation input vectors are not normalized.");

#if (HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED)

	const hkSimdReal dotProd = from.dot<3>( to ); // cos(theta)

	hkSimdReal nearlyOne; nearlyOne.setFromFloat(hkReal(1.0f - 1e-5f));

	if( dotProd.isGreater(nearlyOne) )
	{
		qOut.setIdentity();
	}
	else 
	{
		nearlyOne = -nearlyOne;

		if( dotProd.isLess(nearlyOne) )
		{
			qOut.setFlippedRotation( from );
		}
		else
		{
			// Using cos(theta) = 2*cos^2(theta/2) - 1
			const hkSimdReal oneHalf = hkSimdReal::getConstant<HK_QUADREAL_INV_2>();
			hkSimdReal c; c.setAddMul(oneHalf, oneHalf, dotProd); // .5 * ( 1 + dotProd) 
			const hkSimdReal sqrtInverseC = c.sqrtInverse();

			//const hkReal cosT2 = hkMath::sqrt( c );

			hkVector4 cross;
			cross.setCross( from, to ); // ~sin(theta)* axis -- this is not exact sin(theta) .. error around 1-e2 for angles close to 180deg

			//hkReal rescaleSin  = 0.5f / cosT2;
			hkSimdReal rescaleSin = oneHalf * sqrtInverseC;

			hkSimdReal somewhatNearlyNegOne; somewhatNearlyNegOne.setFromFloat(hkReal(-(1.0f - 1e-3f)));

			if (dotProd.isLess(somewhatNearlyNegOne))
			{
				// Extra correction needed for angles around 180deg
				//
				//const hkReal sinT2 = hkMath::sqrt( cosT2 * cosT2 - dotProd );
				const hkSimdReal sinT4 = c - dotProd;
				const hkSimdReal invApproxSinT = cross.lengthInverse<3>(); // this has errors around 1-e2 for angles around 180 deg.
				//const hkReal sinT = 2 * sinT2 * cosT2;
				//rescaleSin *= (sinT / approxSinT);

				//rescaleSin = sinT2 / approxSinT;
				rescaleSin = sinT4 * sinT4.sqrtInverse() * invApproxSinT;
			}

			// Using sin(theta) = 2*cos(theta/2)*sin(theta/2)
			cross.mul( rescaleSin );
			qOut.m_vec.setXYZ_W( cross, c * sqrtInverseC ); // = sqrt(c)
		}
	}

#else

	const hkReal dotProd = from.dot<3>( to ).getReal(); // cos(theta)
	const hkReal nearlyOne = hkReal(1.0f - 1e-5f);
	const hkReal somewhatNearlyNegativeOne = hkReal(-(1.0f - 1e-3f));
	if( dotProd > nearlyOne )
	{
		qOut.setIdentity();
	}
	else if( dotProd < -nearlyOne )
	{
		qOut.setFlippedRotation( from );
	}
	else
	{
		// Using cos(theta) = 2*cos^2(theta/2) - 1
		const hkReal c = (dotProd + hkReal(1)) * hkReal(0.5f);
		const hkReal cosT2 = hkMath::sqrt( c );

		hkVector4 cross;
		cross.setCross( from, to ); // ~sin(theta)* axis -- this is not exact sin(theta) .. error around 1-e2 for angles close to 180deg

		hkReal rescaleSin  = hkReal(0.5f) / cosT2;

		if (dotProd < somewhatNearlyNegativeOne)
		{
			// Extra correction needed for angles around 180deg
			//
			const hkReal sinT2 = hkMath::sqrt( cosT2 * cosT2 - dotProd );
			const hkReal approxSinT = cross.length<3>().getReal(); // this has errors around 1-e2 for angles around 180 deg.
			const hkReal sinT = hkReal(2) * sinT2 * cosT2;
			rescaleSin *= (sinT / approxSinT);
		}

		// Using sin(theta) = 2*cos(theta/2)*sin(theta/2)
		cross.mul( hkSimdReal::convert(rescaleSin) );
		cross(3) = cosT2;
		qOut.m_vec = cross;
	}

#endif

	HK_MATH_ASSERT(0xad78999d, qOut.isOk(), "Resulting quaternion is not normalized.");
}

HK_FORCE_INLINE void HK_CALL hkQuaternionUtil::_computeShortestRotationDamped(hkVector4Parameter from, hkVector4Parameter to, hkSimdRealParameter gain, hkQuaternion& qOut)
{
	const hkSimdReal one = hkSimdReal_1;
	const hkSimdReal half = hkSimdReal_Inv2;

	const hkSimdReal dotProd = from.dot<3>( to ); // cos(theta)
	const hkSimdReal dampedDot = one - gain + gain * dotProd;
	hkSimdReal nearlyOne; nearlyOne.setFromFloat(hkReal(1.0f - 1e-6f));

	const hkSimdReal c = (dampedDot + one) * half;
	if ( c.isLessEqualZero() | dotProd.isLess(-nearlyOne) )
	{
		qOut.setFlippedRotation( from );
	}
	else
	{
		if( dotProd.isGreater(nearlyOne) )
		{
			qOut.setIdentity();
		}
		else
		{
			const hkSimdReal cosT2 = c.sqrt<HK_ACC_23_BIT,HK_SQRT_IGNORE>();

			hkVector4 cross; cross.setCross( from, to ); // sin(theta)* axis

			// Using sin(theta) = 2*cos(theta/2)*sin(theta/2)
			hkSimdReal rescaleSin; rescaleSin.setDiv(gain * half, cosT2);
			cross.mul( rescaleSin );
			cross.setComponent<3>(cosT2);

			// renormalize for gain.
			cross.normalize<4>();
			qOut.m_vec = cross;
		}
	}

	HK_MATH_ASSERT(0xad78999e, qOut.isOk(), "Resulting quaternion is not normalized.");
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
