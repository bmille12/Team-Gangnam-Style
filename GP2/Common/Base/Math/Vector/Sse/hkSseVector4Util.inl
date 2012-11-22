/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2012 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */

#if defined(HK_REAL_IS_DOUBLE)
#error This implementation is for 32-Bit float
#endif

#define HK_VECTOR4UTIL_exitMmx
HK_FORCE_INLINE void HK_CALL hkVector4Util::exitMmx()
{
#	ifndef HK_ARCH_X64
	_mm_empty();
#	endif
}

#define HK_VECTOR4UTIL_atan2Approximation
HK_FORCE_INLINE void HK_CALL hkVector4Util::atan2Approximation(hkVector4Parameter y, hkVector4Parameter x, hkVector4& result)
{
	result.m_quad = hkMath::quadAtan2(y.m_quad, x.m_quad);
}

#define HK_VECTOR4UTIL_logApproximation
HK_FORCE_INLINE void HK_CALL hkVector4Util::logApproximation(hkVector4Parameter v, hkVector4& result)
{
	result.m_quad = hkMath::quadLog(v.m_quad);
}

#if HK_SSE_VERSION >= 0x41
// yep, we have a dot product instruction

#define HK_VECTOR4UTIL_rotateInversePoints
HK_FORCE_INLINE void	HK_CALL hkVector4Util::rotateInversePoints( const hkRotation& r, const hkVector4* vectorsIn, int numVectors, hkVector4* vectorsOut )
{
	HK_MATH_ASSERT( 0xf0200104, numVectors > 0, "At least one vector to rotate required");

	const hkQuadReal c0 = r.getColumn<0>().m_quad;
	const hkQuadReal c1 = r.getColumn<1>().m_quad;
	const hkQuadReal c2 = r.getColumn<2>().m_quad;
	for( int i = 0; i < numVectors; ++i )
	{
		const hkQuadReal b = vectorsIn[i].m_quad;
		const hkQuadReal r0 = _mm_dp_ps( c0, b, 0x71 );
		const hkQuadReal r1 = _mm_dp_ps( c1, b, 0x72 );
		const hkQuadReal r2 = _mm_dp_ps( c2, b, 0x74 );

		vectorsOut[i].m_quad = _mm_or_ps( _mm_or_ps(r0, r1), r2 );
	}
}

#define HK_VECTOR4UTIL_dot3_3vs3
HK_FORCE_INLINE void HK_CALL hkVector4Util::dot3_3vs3(hkVector4Parameter a0, hkVector4Parameter b0, hkVector4Parameter a1, hkVector4Parameter b1, hkVector4Parameter a2, hkVector4Parameter b2, hkVector4& dotsOut)
{
	const hkQuadReal dp0 = _mm_dp_ps(a0.m_quad, b0.m_quad, 0x71);
	const hkQuadReal dp1 = _mm_dp_ps(a1.m_quad, b1.m_quad, 0x72);
	const hkQuadReal dp2 = _mm_dp_ps(a2.m_quad, b2.m_quad, 0x74);
	dotsOut.m_quad = _mm_or_ps(_mm_or_ps(dp0,dp1), dp2);
}

#define HK_VECTOR4UTIL_dot3_4vs4
HK_FORCE_INLINE void HK_CALL hkVector4Util::dot3_4vs4(hkVector4Parameter a0, hkVector4Parameter b0, hkVector4Parameter a1, hkVector4Parameter b1, hkVector4Parameter a2, hkVector4Parameter b2, hkVector4Parameter a3, hkVector4Parameter b3, hkVector4& dotsOut)
{
	const hkQuadReal dp0 = _mm_dp_ps(a0.m_quad, b0.m_quad, 0x71);
	const hkQuadReal dp1 = _mm_dp_ps(a1.m_quad, b1.m_quad, 0x72);
	const hkQuadReal dp2 = _mm_dp_ps(a2.m_quad, b2.m_quad, 0x74);
	const hkQuadReal dp3 = _mm_dp_ps(a3.m_quad, b3.m_quad, 0x78);
	dotsOut.m_quad = _mm_or_ps(_mm_or_ps(dp0, dp1), _mm_or_ps(dp2, dp3));
}

#define HK_VECTOR4UTIL_dot4_4vs4
HK_FORCE_INLINE void HK_CALL hkVector4Util::dot4_4vs4( hkVector4Parameter a0, hkVector4Parameter b0, hkVector4Parameter a1, hkVector4Parameter b1, hkVector4Parameter a2, hkVector4Parameter b2, hkVector4Parameter a3, hkVector4Parameter b3, hkVector4& dotsOut)
{
	const hkQuadReal dp0 = _mm_dp_ps(a0.m_quad, b0.m_quad, 0xF1);
	const hkQuadReal dp1 = _mm_dp_ps(a1.m_quad, b1.m_quad, 0xF2);
	const hkQuadReal dp2 = _mm_dp_ps(a2.m_quad, b2.m_quad, 0xF4);
	const hkQuadReal dp3 = _mm_dp_ps(a3.m_quad, b3.m_quad, 0xF8);
	dotsOut.m_quad = _mm_or_ps(_mm_or_ps(dp0, dp1), _mm_or_ps(dp2, dp3));
}

#define HK_VECTOR4UTIL_dot3_1vs4
HK_FORCE_INLINE void HK_CALL hkVector4Util::dot3_1vs4( hkVector4Parameter v, hkVector4Parameter a0, hkVector4Parameter a1, hkVector4Parameter a2, hkVector4Parameter a3, hkVector4& dotsOut)
{
	const hkQuadReal dp0 = _mm_dp_ps(v.m_quad, a0.m_quad, 0x71);
	const hkQuadReal dp1 = _mm_dp_ps(v.m_quad, a1.m_quad, 0x72);
	const hkQuadReal dp2 = _mm_dp_ps(v.m_quad, a2.m_quad, 0x74);
	const hkQuadReal dp3 = _mm_dp_ps(v.m_quad, a3.m_quad, 0x78);
	dotsOut.m_quad = _mm_or_ps(_mm_or_ps(dp0, dp1), _mm_or_ps(dp2, dp3));
}

#define HK_VECTOR4UTIL_dot3_1vs3
HK_FORCE_INLINE void HK_CALL hkVector4Util::dot3_1vs3( hkVector4Parameter v, hkVector4Parameter a0, hkVector4Parameter a1, hkVector4Parameter a2, hkVector4& dotsOut)
{
	const hkQuadReal dp0 = _mm_dp_ps(v.m_quad, a0.m_quad, 0x71);
	const hkQuadReal dp1 = _mm_dp_ps(v.m_quad, a1.m_quad, 0x72);
	const hkQuadReal dp2 = _mm_dp_ps(v.m_quad, a2.m_quad, 0x74);
	dotsOut.m_quad = _mm_or_ps(_mm_or_ps(dp0, dp1), dp2);
}

//
//	Sets this vector components: this(i) = vector.dot<4>(AI) for i=0..3

#define HK_VECTOR4UTIL_dot4_1vs4
HK_FORCE_INLINE void HK_CALL hkVector4Util::dot4_1vs4( hkVector4Parameter vectorIn, hkVector4Parameter a0, hkVector4Parameter a1, hkVector4Parameter a2, hkVector4Parameter a3, hkVector4& dotsOut)
{
	const hkQuadReal dp0 = _mm_dp_ps(vectorIn.m_quad, a0.m_quad, 0xF1);
	const hkQuadReal dp1 = _mm_dp_ps(vectorIn.m_quad, a1.m_quad, 0xF2);
	const hkQuadReal dp2 = _mm_dp_ps(vectorIn.m_quad, a2.m_quad, 0xF4);
	const hkQuadReal dp3 = _mm_dp_ps(vectorIn.m_quad, a3.m_quad, 0xF8);

	dotsOut.m_quad = _mm_or_ps(_mm_or_ps(dp0, dp1), _mm_or_ps(dp2, dp3));
}

//
//	Sets this vector components: this(i) = vector.dot<4>(AI) for i=0..2

#define HK_VECTOR4UTIL_dot4_1vs3
HK_FORCE_INLINE void HK_CALL hkVector4Util::dot4_1vs3( hkVector4Parameter vectorIn, hkVector4Parameter a0, hkVector4Parameter a1, hkVector4Parameter a2, hkVector4& dotsOut)
{
	const hkQuadReal dp0 = _mm_dp_ps(vectorIn.m_quad, a0.m_quad, 0xF1);
	const hkQuadReal dp1 = _mm_dp_ps(vectorIn.m_quad, a1.m_quad, 0xF2);
	const hkQuadReal dp2 = _mm_dp_ps(vectorIn.m_quad, a2.m_quad, 0xF4);

	dotsOut.m_quad = _mm_or_ps(_mm_or_ps(dp0, dp1), dp2);
}

#define HK_VECTOR4UTIL_dot3_2vs2
HK_FORCE_INLINE void HK_CALL hkVector4Util::dot3_2vs2( hkVector4Parameter a0, hkVector4Parameter a2, hkVector4Parameter b0, hkVector4Parameter b1, hkVector4& dotsOut)
{
	const hkQuadReal dp00 = _mm_dp_ps(a0.m_quad, b0.m_quad, 0x71);
	const hkQuadReal dp01 = _mm_dp_ps(a0.m_quad, b1.m_quad, 0x72);
	const hkQuadReal dp20 = _mm_dp_ps(a2.m_quad, b0.m_quad, 0x74);
	const hkQuadReal dp21 = _mm_dp_ps(a2.m_quad, b1.m_quad, 0x78);
	dotsOut.m_quad = _mm_or_ps(_mm_or_ps(dp00, dp01), _mm_or_ps(dp20, dp21));
}

#elif HK_SSE_VERSION >= 0x30

#define HK_VECTOR4UTIL_dot4_4vs4
HK_FORCE_INLINE void HK_CALL hkVector4Util::dot4_4vs4( hkVector4Parameter a0, hkVector4Parameter b0, hkVector4Parameter a1, hkVector4Parameter b1, hkVector4Parameter a2, hkVector4Parameter b2, hkVector4Parameter a3, hkVector4Parameter b3, hkVector4& dotsOut)
{
	const hkQuadReal m0 = _mm_mul_ps(b0.m_quad, a0.m_quad);
	const hkQuadReal m1 = _mm_mul_ps(b1.m_quad, a1.m_quad);
	const hkQuadReal m2 = _mm_mul_ps(b2.m_quad, a2.m_quad);
	const hkQuadReal m3 = _mm_mul_ps(b3.m_quad, a3.m_quad);

	const hkQuadReal hsum0 = _mm_hadd_ps(m0, m0);
	const hkQuadReal hsum1 = _mm_hadd_ps(m1, m1);
	const hkQuadReal hsum2 = _mm_hadd_ps(m2, m2);
	const hkQuadReal hsum3 = _mm_hadd_ps(m3, m3);

	const hkQuadReal dp0 = _mm_hadd_ps(hsum0, hsum0);
	const hkQuadReal dp1 = _mm_hadd_ps(hsum1, hsum1);
	const hkQuadReal dp2 = _mm_hadd_ps(hsum2, hsum2);
	const hkQuadReal dp3 = _mm_hadd_ps(hsum3, hsum3);

	const hkQuadReal zero = _mm_setzero_ps();

	const hkQuadReal dp0z = _mm_shuffle_ps(dp0,zero,_MM_SHUFFLE(0,0,0,0));
	const hkQuadReal dp1z = _mm_shuffle_ps(dp1,zero,_MM_SHUFFLE(1,1,0,0));
	const hkQuadReal dp2z = _mm_shuffle_ps(dp2,zero,_MM_SHUFFLE(2,2,0,0));
	const hkQuadReal dp3z = _mm_shuffle_ps(dp3,zero,_MM_SHUFFLE(3,3,0,0));

	dotsOut.m_quad =                           _mm_shuffle_ps(dp0z,dp2z,_MM_SHUFFLE(3,0,3,0));
	dotsOut.m_quad = _mm_or_ps(dotsOut.m_quad, _mm_shuffle_ps(dp1z,dp3z,_MM_SHUFFLE(0,3,0,3)));
}

#define HK_VECTOR4UTIL_dot4_1vs4
HK_FORCE_INLINE void HK_CALL hkVector4Util::dot4_1vs4( hkVector4Parameter vectorIn, hkVector4Parameter a0, hkVector4Parameter a1, hkVector4Parameter a2, hkVector4Parameter a3, hkVector4& dotsOut)
{
	const hkQuadReal m0 = _mm_mul_ps(vectorIn.m_quad, a0.m_quad);
	const hkQuadReal m1 = _mm_mul_ps(vectorIn.m_quad, a1.m_quad);
	const hkQuadReal m2 = _mm_mul_ps(vectorIn.m_quad, a2.m_quad);
	const hkQuadReal m3 = _mm_mul_ps(vectorIn.m_quad, a3.m_quad);

	const hkQuadReal hsum0 = _mm_hadd_ps(m0, m0);
	const hkQuadReal hsum1 = _mm_hadd_ps(m1, m1);
	const hkQuadReal hsum2 = _mm_hadd_ps(m2, m2);
	const hkQuadReal hsum3 = _mm_hadd_ps(m3, m3);

	const hkQuadReal dp0 = _mm_hadd_ps(hsum0, hsum0);
	const hkQuadReal dp1 = _mm_hadd_ps(hsum1, hsum1);
	const hkQuadReal dp2 = _mm_hadd_ps(hsum2, hsum2);
	const hkQuadReal dp3 = _mm_hadd_ps(hsum3, hsum3);

	const hkQuadReal zero = _mm_setzero_ps();

	const hkQuadReal dp0z = _mm_shuffle_ps(dp0,zero,_MM_SHUFFLE(0,0,0,0));
	const hkQuadReal dp1z = _mm_shuffle_ps(dp1,zero,_MM_SHUFFLE(1,1,0,0));
	const hkQuadReal dp2z = _mm_shuffle_ps(dp2,zero,_MM_SHUFFLE(2,2,0,0));
	const hkQuadReal dp3z = _mm_shuffle_ps(dp3,zero,_MM_SHUFFLE(3,3,0,0));

	dotsOut.m_quad =                           _mm_shuffle_ps(dp0z,dp2z,_MM_SHUFFLE(3,0,3,0));
	dotsOut.m_quad = _mm_or_ps(dotsOut.m_quad, _mm_shuffle_ps(dp1z,dp3z,_MM_SHUFFLE(0,3,0,3)));
}

#define HK_VECTOR4UTIL_dot4_1vs3
HK_FORCE_INLINE void HK_CALL hkVector4Util::dot4_1vs3( hkVector4Parameter vectorIn, hkVector4Parameter a0, hkVector4Parameter a1, hkVector4Parameter a2, hkVector4& dotsOut)
{
	const hkQuadReal m0 = _mm_mul_ps(vectorIn.m_quad, a0.m_quad);
	const hkQuadReal m1 = _mm_mul_ps(vectorIn.m_quad, a1.m_quad);
	const hkQuadReal m2 = _mm_mul_ps(vectorIn.m_quad, a2.m_quad);

	const hkQuadReal hsum0 = _mm_hadd_ps(m0, m0);
	const hkQuadReal hsum1 = _mm_hadd_ps(m1, m1);
	const hkQuadReal hsum2 = _mm_hadd_ps(m2, m2);

	const hkQuadReal dp0 = _mm_hadd_ps(hsum0, hsum0);
	const hkQuadReal dp1 = _mm_hadd_ps(hsum1, hsum1);
	const hkQuadReal dp2 = _mm_hadd_ps(hsum2, hsum2);

	const hkQuadReal zero = _mm_setzero_ps();

	const hkQuadReal dp0z = _mm_shuffle_ps(dp0,zero,_MM_SHUFFLE(0,0,0,0));
	const hkQuadReal dp1z = _mm_shuffle_ps(dp1,zero,_MM_SHUFFLE(1,1,0,0));
	const hkQuadReal dp2z = _mm_shuffle_ps(dp2,zero,_MM_SHUFFLE(2,2,0,0));

	dotsOut.m_quad =                           _mm_shuffle_ps(dp0z,dp2z,_MM_SHUFFLE(3,0,3,0));
	dotsOut.m_quad = _mm_or_ps(dotsOut.m_quad, _mm_shuffle_ps(dp1z,zero,_MM_SHUFFLE(0,3,0,3)));
}

#endif // sse specials

//
//	Returns the i-th component in the concatenated 8-element vector [ax, ay, az aw, bx, by, bz, bw], where ax has index 0.
//	The index i is computed based on the first 3 bits of given mask value., i.e. MASK_NONE	= 0, MASK_X = 1, MASK_Y = 2, MASK_XY = 3, etc.

HK_FORCE_INLINE const hkSimdReal hkVector4Util::getComponentAtMaskIndex(hkVector4ComparisonParameter cmp, hkVector4Parameter a, hkVector4Parameter b)
{
	//	mx		my		mz		|	
	//	0		0		0		|	ax
	//	0		0		1		|	ay
	//	0		1		0		|	az
	//	0		1		1		|	aw
	//	1		0		0		|	bx
	//	1		0		1		|	by
	//	1		1		0		|	bz
	//	1		1		1		|	bw

	// Select among the two vectors
	const	hkQuadReal		vMask	= cmp.m_mask;															// [mx, my, mz, 0]
	const	hkQuadReal		mx		= _mm_shuffle_ps(vMask, vMask, 0x00);	// (x, x, x, x)					// [mx, mx, mx, mx]
			hkQuadReal		ab		= _mm_or_ps( _mm_and_ps(mx, b.m_quad), _mm_andnot_ps(mx, a.m_quad) );	// [abx, aby, abz, abw]
	{
		const hkQuadReal	my		= _mm_shuffle_ps(vMask, vMask, 0x55);	// (y, y, y, y)					// [my, my, my, my]
		const hkQuadReal	abxy	= _mm_shuffle_ps(ab, ab, 0x44);			// (x, y, x, y)					// [abx, aby, abx, aby]
		const hkQuadReal	abzw	= _mm_shuffle_ps(ab, ab, 0xEE);			// (z, w, z, w)					// [abz, abw, abz, abw]
							ab		= _mm_or_ps( _mm_and_ps(my, abzw), _mm_andnot_ps(my, abxy) );			// [abxz, abyw, abxz, abyw]
	}
	{
		const hkQuadReal	abxz	= _mm_shuffle_ps(ab, ab, 0x00);			// (x, x, x, x)					// [abxz, abxz, abxz, abxz]
		const hkQuadReal	abyw	= _mm_shuffle_ps(ab, ab, 0x55);			// (y, y, y, y)					// [abyw, abyw, abyw, abyw]			 
		const hkQuadReal	mz		= _mm_shuffle_ps(vMask, vMask, 0xAA);	// (z, z, z, z)					// [mz, mz, mz, mz]
							ab		= _mm_or_ps( _mm_and_ps(mz, abyw), _mm_andnot_ps(mz, abxz) );
	}

	hkSimdReal ret;	ret.m_real = ab;
	return ret;
}


#define HK_VECTOR4UTIL_convertQuaternionToRotation
void HK_CALL hkVector4Util::convertQuaternionToRotation( hkQuaternionParameter qi, hkRotation& rotationOut )
{
	HK_MATH_ASSERT(0x1ff88f0e, qi.isOk(), "hkQuaternion not normalized/invalid!");

	static HK_ALIGN16( const hkUint32 negateMask0[4] ) = { 0x00000000, 0x80000000, 0x80000000, 0x80000000 };
	static HK_ALIGN16( const hkUint32 negateMask1[4] ) = { 0x80000000, 0x00000000, 0x80000000, 0x80000000 };

	const hkQuadReal xyzw = qi.m_vec.m_quad;
	const hkQuadReal xyzw2 = _mm_add_ps(xyzw,xyzw);							// xyzw2 = x2, y2, z2, w2

	// calculate the 9 products

	hkQuadReal tmp2 = _mm_shuffle_ps(xyzw, xyzw, _MM_SHUFFLE(1,0,0,1));		// tmp2 = y, x, x, y
	hkQuadReal tmp3 = _mm_shuffle_ps(xyzw2, xyzw2, _MM_SHUFFLE(2,2,1,1) );	// tmp3 = y2, y2, z2, z2
	tmp2 = _mm_mul_ps(tmp2,tmp3);											// tmp2 = yy2, xy2, xz2, yz2

	hkQuadReal tmp4 = _mm_shuffle_ps(xyzw, xyzw, _MM_SHUFFLE(3,3,3,2) );	// tmp4 = z, w, w, w
	hkQuadReal tmp5 = _mm_shuffle_ps(xyzw2, xyzw2, _MM_SHUFFLE(0,1,2,2) );	// tmp5 = z2, z2, y2, x2
	tmp4 = _mm_mul_ps(tmp4,tmp5);											// tmp4 = zz2, wz2, wy2, wx2

	hkQuadReal tmp1 = _mm_mul_ss( xyzw, xyzw2 );							// tmp1 = xx2, y, z, w

	// calculate the last two elements of the third row
	hkQuadReal tmp6 = g_vectorConstants[HK_QUADREAL_1000];					// tmp6 = 1, 0, 0, 0
	tmp6 = _mm_sub_ss( tmp6, tmp1 );										// tmp6 = -xx2+1, 0, 0, 0
	tmp6 = _mm_sub_ss( tmp6, tmp2 );										// tmp6 = -xx2-yy2+1, 0, 0, 0

	// calculate first row
	tmp2 = _mm_xor_ps ( tmp2, *(const hkQuadReal*)&negateMask0 );			// tmp2 = yy2, -xy2, -xz2, -yz2
	tmp4 = _mm_xor_ps ( tmp4, *(const hkQuadReal*)&negateMask1 );			// tmp4 = -zz2, wz2, -wy2, -wx2
	tmp4 = _mm_add_ss ( tmp4, g_vectorConstants[HK_QUADREAL_1000]); 		// tmp4 = -zz2+1, wz2, -wy2, -wx2
	tmp3 = _mm_sub_ps(tmp4,tmp2);											// tmp3 = -yy2-zz2+1, xy2+wz2, xz2-wy2, yz2-wx2
	rotationOut.setColumn<0>((const hkVector4&)tmp3);						// row0 = -yy2-zz2+1, xy2+wz2, xz2-wy2, yz2-wx2

	// calculate second row
	tmp2 = _mm_move_ss( tmp2, tmp1 );										// tmp2 = xx2, -xy2, -xz2, -yz2
	tmp4 = _mm_xor_ps ( tmp4, *(const hkQuadReal*)&negateMask0 );			// tmp4 = -zz2+1, -wz2, wy2, wx2
	tmp4 = _mm_sub_ps(tmp4,tmp2);											// tmp4 = -xx2-zz2+1, xy2-wz2, xz2+wy2, yz2+wx2
	tmp4 = _mm_shuffle_ps(tmp4, tmp4, _MM_SHUFFLE(2,3,0,1) );
	rotationOut.setColumn<1>((const hkVector4&)tmp4);						// row1 = xy2-wz2, -xx2-zz2+1, yz2+wx2, xz2+wy2

	// calculate third row
	tmp3 = _mm_movehl_ps( tmp3, tmp4 );										// tmp3 = yz2+wx2, xz2+wy2, xz2-wy2, yz2-wx2
	tmp3 = _mm_shuffle_ps(tmp3,tmp6, _MM_SHUFFLE(2,0,3,1) );				// tmp3 = xz2+wy2, yz2-wx2, -xx2-yy2+1, 0
	rotationOut.setColumn<2>((const hkVector4&)tmp3);						// row2 = xz2+wy2, yz2-wx2, -xx2-yy2+1, 0
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
