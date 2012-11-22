/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2012 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */

#if !defined(HK_REAL_IS_DOUBLE)
#error This implementation is for 64-Bit double
#endif

#define HK_VECTOR4UTIL_exitMmx
HK_FORCE_INLINE void HK_CALL hkVector4Util::exitMmx()
{
#	ifndef HK_ARCH_X64
	_mm_empty();
#	endif
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

		const hkSingleReal r0xy = _mm_dp_pd( c0.xy, b.xy, 0x31 );
		const hkSingleReal r0zw = _mm_dp_pd( c0.zw, b.zw, 0x11 );
		const hkSingleReal r0   = _mm_add_pd(r0xy, r0zw);

		const hkSingleReal r1xy = _mm_dp_pd( c1.xy, b.xy, 0x32 );
		const hkSingleReal r1zw = _mm_dp_pd( c1.zw, b.zw, 0x12 );
		const hkSingleReal r1   = _mm_add_pd(r1xy, r1zw);

		vectorsOut[i].m_quad.xy = _mm_or_pd(r0, r1);

		const hkSingleReal r2xy = _mm_dp_pd( c2.xy, b.xy, 0x31 );
		const hkSingleReal r2zw = _mm_dp_pd( c2.zw, b.zw, 0x11 );

		vectorsOut[i].m_quad.zw = _mm_add_pd(r2xy, r2zw);
	}
}

#define HK_VECTOR4UTIL_dot3_3vs3
HK_FORCE_INLINE void HK_CALL hkVector4Util::dot3_3vs3(hkVector4Parameter a0, hkVector4Parameter b0, hkVector4Parameter a1, hkVector4Parameter b1, hkVector4Parameter a2, hkVector4Parameter b2, hkVector4& dotsOut)
{
	const hkSingleReal dp0xy = _mm_dp_pd(a0.m_quad.xy, b0.m_quad.xy, 0x31);
	const hkSingleReal dp0zw = _mm_dp_pd(a0.m_quad.zw, b0.m_quad.zw, 0x11);
	const hkSingleReal dp0   = _mm_add_pd(dp0xy, dp0zw);

	const hkSingleReal dp1xy = _mm_dp_pd(a1.m_quad.xy, b1.m_quad.xy, 0x32);
	const hkSingleReal dp1zw = _mm_dp_pd(a1.m_quad.zw, b1.m_quad.zw, 0x12);
	const hkSingleReal dp1   = _mm_add_pd(dp1xy, dp1zw);

	dotsOut.m_quad.xy = _mm_or_pd(dp0,dp1);

	const hkSingleReal dp2xy = _mm_dp_pd(a2.m_quad.xy, b2.m_quad.xy, 0x31);
	const hkSingleReal dp2zw = _mm_dp_pd(a2.m_quad.zw, b2.m_quad.zw, 0x11);

	dotsOut.m_quad.zw = _mm_add_pd(dp2xy, dp2zw);
}

#define HK_VECTOR4UTIL_dot3_4vs4
HK_FORCE_INLINE void HK_CALL hkVector4Util::dot3_4vs4(hkVector4Parameter a0, hkVector4Parameter b0, hkVector4Parameter a1, hkVector4Parameter b1, hkVector4Parameter a2, hkVector4Parameter b2, hkVector4Parameter a3, hkVector4Parameter b3, hkVector4& dotsOut)
{
	const hkSingleReal dp0xy = _mm_dp_pd(a0.m_quad.xy, b0.m_quad.xy, 0x31);
	const hkSingleReal dp0zw = _mm_dp_pd(a0.m_quad.zw, b0.m_quad.zw, 0x11);
	const hkSingleReal dp0   = _mm_add_pd(dp0xy, dp0zw);

	const hkSingleReal dp1xy = _mm_dp_pd(a1.m_quad.xy, b1.m_quad.xy, 0x32);
	const hkSingleReal dp1zw = _mm_dp_pd(a1.m_quad.zw, b1.m_quad.zw, 0x12);
	const hkSingleReal dp1   = _mm_add_pd(dp1xy, dp1zw);

	dotsOut.m_quad.xy = _mm_or_pd(dp0,dp1);

	const hkSingleReal dp2xy = _mm_dp_pd(a2.m_quad.xy, b2.m_quad.xy, 0x31);
	const hkSingleReal dp2zw = _mm_dp_pd(a2.m_quad.zw, b2.m_quad.zw, 0x11);
	const hkSingleReal dp2   = _mm_add_pd(dp2xy, dp2zw);

	const hkSingleReal dp3xy = _mm_dp_pd(a3.m_quad.xy, b3.m_quad.xy, 0x32);
	const hkSingleReal dp3zw = _mm_dp_pd(a3.m_quad.zw, b3.m_quad.zw, 0x12);
	const hkSingleReal dp3   = _mm_add_pd(dp3xy, dp3zw);

	dotsOut.m_quad.zw = _mm_or_pd(dp2,dp3);
}

#define HK_VECTOR4UTIL_dot4_4vs4
HK_FORCE_INLINE void HK_CALL hkVector4Util::dot4_4vs4( hkVector4Parameter a0, hkVector4Parameter b0, hkVector4Parameter a1, hkVector4Parameter b1, hkVector4Parameter a2, hkVector4Parameter b2, hkVector4Parameter a3, hkVector4Parameter b3, hkVector4& dotsOut)
{
	const hkSingleReal dp0xy = _mm_dp_pd(a0.m_quad.xy, b0.m_quad.xy, 0x31);
	const hkSingleReal dp0zw = _mm_dp_pd(a0.m_quad.zw, b0.m_quad.zw, 0x31);
	const hkSingleReal dp0   = _mm_add_pd(dp0xy, dp0zw);

	const hkSingleReal dp1xy = _mm_dp_pd(a1.m_quad.xy, b1.m_quad.xy, 0x32);
	const hkSingleReal dp1zw = _mm_dp_pd(a1.m_quad.zw, b1.m_quad.zw, 0x32);
	const hkSingleReal dp1   = _mm_add_pd(dp1xy, dp1zw);

	dotsOut.m_quad.xy = _mm_or_pd(dp0,dp1);

	const hkSingleReal dp2xy = _mm_dp_pd(a2.m_quad.xy, b2.m_quad.xy, 0x31);
	const hkSingleReal dp2zw = _mm_dp_pd(a2.m_quad.zw, b2.m_quad.zw, 0x31);
	const hkSingleReal dp2   = _mm_add_pd(dp2xy, dp2zw);

	const hkSingleReal dp3xy = _mm_dp_pd(a3.m_quad.xy, b3.m_quad.xy, 0x32);
	const hkSingleReal dp3zw = _mm_dp_pd(a3.m_quad.zw, b3.m_quad.zw, 0x32);
	const hkSingleReal dp3   = _mm_add_pd(dp3xy, dp3zw);

	dotsOut.m_quad.zw = _mm_or_pd(dp2,dp3);
}

#define HK_VECTOR4UTIL_dot3_1vs4
HK_FORCE_INLINE void HK_CALL hkVector4Util::dot3_1vs4( hkVector4Parameter v, hkVector4Parameter a0, hkVector4Parameter a1, hkVector4Parameter a2, hkVector4Parameter a3, hkVector4& dotsOut)
{
	const hkSingleReal dp0xy = _mm_dp_pd(v.m_quad.xy, a0.m_quad.xy, 0x31);
	const hkSingleReal dp0zw = _mm_dp_pd(v.m_quad.zw, a0.m_quad.zw, 0x11);
	const hkSingleReal dp0   = _mm_add_pd(dp0xy, dp0zw);

	const hkSingleReal dp1xy = _mm_dp_pd(v.m_quad.xy, a1.m_quad.xy, 0x32);
	const hkSingleReal dp1zw = _mm_dp_pd(v.m_quad.zw, a1.m_quad.zw, 0x12);
	const hkSingleReal dp1   = _mm_add_pd(dp1xy, dp1zw);

	dotsOut.m_quad.xy = _mm_or_pd(dp0,dp1);

	const hkSingleReal dp2xy = _mm_dp_pd(v.m_quad.xy, a2.m_quad.xy, 0x31);
	const hkSingleReal dp2zw = _mm_dp_pd(v.m_quad.zw, a2.m_quad.zw, 0x11);
	const hkSingleReal dp2   = _mm_add_pd(dp2xy, dp2zw);

	const hkSingleReal dp3xy = _mm_dp_pd(v.m_quad.xy, a3.m_quad.xy, 0x32);
	const hkSingleReal dp3zw = _mm_dp_pd(v.m_quad.zw, a3.m_quad.zw, 0x12);
	const hkSingleReal dp3   = _mm_add_pd(dp3xy, dp3zw);

	dotsOut.m_quad.zw = _mm_or_pd(dp2,dp3);
}

//
//	Sets this vector components: this(i) = vector.dot<4>(AI) for i=0..3

#define HK_VECTOR4UTIL_dot4_1vs4
HK_FORCE_INLINE void HK_CALL hkVector4Util::dot4_1vs4( hkVector4Parameter v, hkVector4Parameter a0, hkVector4Parameter a1, hkVector4Parameter a2, hkVector4Parameter a3, hkVector4& dotsOut)
{
	const hkSingleReal dp0xy = _mm_dp_pd(v.m_quad.xy, a0.m_quad.xy, 0x31);
	const hkSingleReal dp0zw = _mm_dp_pd(v.m_quad.zw, a0.m_quad.zw, 0x31);
	const hkSingleReal dp0   = _mm_add_pd(dp0xy, dp0zw);

	const hkSingleReal dp1xy = _mm_dp_pd(v.m_quad.xy, a1.m_quad.xy, 0x32);
	const hkSingleReal dp1zw = _mm_dp_pd(v.m_quad.zw, a1.m_quad.zw, 0x32);
	const hkSingleReal dp1   = _mm_add_pd(dp1xy, dp1zw);

	dotsOut.m_quad.xy = _mm_or_pd(dp0,dp1);

	const hkSingleReal dp2xy = _mm_dp_pd(v.m_quad.xy, a2.m_quad.xy, 0x31);
	const hkSingleReal dp2zw = _mm_dp_pd(v.m_quad.zw, a2.m_quad.zw, 0x31);
	const hkSingleReal dp2   = _mm_add_pd(dp2xy, dp2zw);

	const hkSingleReal dp3xy = _mm_dp_pd(v.m_quad.xy, a3.m_quad.xy, 0x32);
	const hkSingleReal dp3zw = _mm_dp_pd(v.m_quad.zw, a3.m_quad.zw, 0x32);
	const hkSingleReal dp3   = _mm_add_pd(dp3xy, dp3zw);

	dotsOut.m_quad.zw = _mm_or_pd(dp2,dp3);
}

//
//	Sets this vector components: this(i) = vector.dot<4>(AI) for i=0..2

#define HK_VECTOR4UTIL_dot4_1vs3
HK_FORCE_INLINE void HK_CALL hkVector4Util::dot4_1vs3( hkVector4Parameter v, hkVector4Parameter a0, hkVector4Parameter a1, hkVector4Parameter a2, hkVector4& dotsOut)
{
	const hkSingleReal dp0xy = _mm_dp_pd(v.m_quad.xy, a0.m_quad.xy, 0x31);
	const hkSingleReal dp0zw = _mm_dp_pd(v.m_quad.zw, a0.m_quad.zw, 0x31);
	const hkSingleReal dp0   = _mm_add_pd(dp0xy, dp0zw);

	const hkSingleReal dp1xy = _mm_dp_pd(v.m_quad.xy, a1.m_quad.xy, 0x32);
	const hkSingleReal dp1zw = _mm_dp_pd(v.m_quad.zw, a1.m_quad.zw, 0x32);
	const hkSingleReal dp1   = _mm_add_pd(dp1xy, dp1zw);

	dotsOut.m_quad.xy = _mm_or_pd(dp0,dp1);

	const hkSingleReal dp2xy = _mm_dp_pd(v.m_quad.xy, a2.m_quad.xy, 0x31);
	const hkSingleReal dp2zw = _mm_dp_pd(v.m_quad.zw, a2.m_quad.zw, 0x31);
	const hkSingleReal dp2   = _mm_add_pd(dp2xy, dp2zw);

	dotsOut.m_quad.zw = dp2;
}

#define HK_VECTOR4UTIL_dot3_2vs2
HK_FORCE_INLINE void HK_CALL hkVector4Util::dot3_2vs2( hkVector4Parameter a0, hkVector4Parameter a2, hkVector4Parameter b0, hkVector4Parameter b1, hkVector4& dotsOut)
{
	const hkSingleReal dp0xy = _mm_dp_pd(a0.m_quad.xy, b0.m_quad.xy, 0x31);
	const hkSingleReal dp0zw = _mm_dp_pd(a0.m_quad.zw, b0.m_quad.zw, 0x11);
	const hkSingleReal dp0   = _mm_add_pd(dp0xy, dp0zw);

	const hkSingleReal dp1xy = _mm_dp_pd(a0.m_quad.xy, b1.m_quad.xy, 0x32);
	const hkSingleReal dp1zw = _mm_dp_pd(a0.m_quad.zw, b1.m_quad.zw, 0x12);
	const hkSingleReal dp1   = _mm_add_pd(dp1xy, dp1zw);

	dotsOut.m_quad.xy = _mm_or_pd(dp0,dp1);

	const hkSingleReal dp2xy = _mm_dp_pd(a2.m_quad.xy, b0.m_quad.xy, 0x31);
	const hkSingleReal dp2zw = _mm_dp_pd(a2.m_quad.zw, b0.m_quad.zw, 0x11);
	const hkSingleReal dp2   = _mm_add_pd(dp2xy, dp2zw);

	const hkSingleReal dp3xy = _mm_dp_pd(a2.m_quad.xy, b1.m_quad.xy, 0x32);
	const hkSingleReal dp3zw = _mm_dp_pd(a2.m_quad.zw, b1.m_quad.zw, 0x12);
	const hkSingleReal dp3   = _mm_add_pd(dp3xy, dp3zw);

	dotsOut.m_quad.zw = _mm_or_pd(dp2,dp3);
}

#elif HK_SSE_VERSION >= 0x30

#define HK_VECTOR4UTIL_dot4_4vs4
HK_FORCE_INLINE void HK_CALL hkVector4Util::dot4_4vs4( hkVector4Parameter a0, hkVector4Parameter b0, hkVector4Parameter a1, hkVector4Parameter b1, hkVector4Parameter a2, hkVector4Parameter b2, hkVector4Parameter a3, hkVector4Parameter b3, hkVector4& dotsOut)
{
	const hkSingleReal m0xy = _mm_mul_pd(b0.m_quad.xy, a0.m_quad.xy);
	const hkSingleReal m1xy = _mm_mul_pd(b1.m_quad.xy, a1.m_quad.xy);
	const hkSingleReal m2xy = _mm_mul_pd(b2.m_quad.xy, a2.m_quad.xy);
	const hkSingleReal m3xy = _mm_mul_pd(b3.m_quad.xy, a3.m_quad.xy);
	const hkSingleReal m0zw = _mm_mul_pd(b0.m_quad.zw, a0.m_quad.zw);
	const hkSingleReal m1zw = _mm_mul_pd(b1.m_quad.zw, a1.m_quad.zw);
	const hkSingleReal m2zw = _mm_mul_pd(b2.m_quad.zw, a2.m_quad.zw);
	const hkSingleReal m3zw = _mm_mul_pd(b3.m_quad.zw, a3.m_quad.zw);

	const hkSingleReal hsum0 = _mm_hadd_pd(m0xy, m0zw);
	const hkSingleReal hsum1 = _mm_hadd_pd(m1xy, m1zw);
	const hkSingleReal hsum2 = _mm_hadd_pd(m2xy, m2zw);
	const hkSingleReal hsum3 = _mm_hadd_pd(m3xy, m3zw);

	const hkSingleReal dp0 = _mm_hadd_pd(hsum0, hsum0);
	const hkSingleReal dp1 = _mm_hadd_pd(hsum1, hsum1);
	const hkSingleReal dp2 = _mm_hadd_pd(hsum2, hsum2);
	const hkSingleReal dp3 = _mm_hadd_pd(hsum3, hsum3);

	dotsOut.m_quad.xy = _mm_move_sd(dp1,dp0);
	dotsOut.m_quad.zw = _mm_move_sd(dp3,dp2);
}

#define HK_VECTOR4UTIL_dot4_1vs4
HK_FORCE_INLINE void HK_CALL hkVector4Util::dot4_1vs4( hkVector4Parameter vectorIn, hkVector4Parameter a0, hkVector4Parameter a1, hkVector4Parameter a2, hkVector4Parameter a3, hkVector4& dotsOut)
{
	const hkSingleReal m0xy = _mm_mul_pd(vectorIn.m_quad.xy, a0.m_quad.xy);
	const hkSingleReal m1xy = _mm_mul_pd(vectorIn.m_quad.xy, a1.m_quad.xy);
	const hkSingleReal m2xy = _mm_mul_pd(vectorIn.m_quad.xy, a2.m_quad.xy);
	const hkSingleReal m3xy = _mm_mul_pd(vectorIn.m_quad.xy, a3.m_quad.xy);
	const hkSingleReal m0zw = _mm_mul_pd(vectorIn.m_quad.zw, a0.m_quad.zw);
	const hkSingleReal m1zw = _mm_mul_pd(vectorIn.m_quad.zw, a1.m_quad.zw);
	const hkSingleReal m2zw = _mm_mul_pd(vectorIn.m_quad.zw, a2.m_quad.zw);
	const hkSingleReal m3zw = _mm_mul_pd(vectorIn.m_quad.zw, a3.m_quad.zw);

	const hkSingleReal hsum0 = _mm_hadd_pd(m0xy, m0zw);
	const hkSingleReal hsum1 = _mm_hadd_pd(m1xy, m1zw);
	const hkSingleReal hsum2 = _mm_hadd_pd(m2xy, m2zw);
	const hkSingleReal hsum3 = _mm_hadd_pd(m3xy, m3zw);

	const hkSingleReal dp0 = _mm_hadd_pd(hsum0, hsum0);
	const hkSingleReal dp1 = _mm_hadd_pd(hsum1, hsum1);
	const hkSingleReal dp2 = _mm_hadd_pd(hsum2, hsum2);
	const hkSingleReal dp3 = _mm_hadd_pd(hsum3, hsum3);

	dotsOut.m_quad.xy = _mm_move_sd(dp1,dp0);
	dotsOut.m_quad.zw = _mm_move_sd(dp3,dp2);
}

#define HK_VECTOR4UTIL_dot4_1vs3
HK_FORCE_INLINE void HK_CALL hkVector4Util::dot4_1vs3( hkVector4Parameter vectorIn, hkVector4Parameter a0, hkVector4Parameter a1, hkVector4Parameter a2, hkVector4& dotsOut)
{
	const hkSingleReal m0xy = _mm_mul_pd(vectorIn.m_quad.xy, a0.m_quad.xy);
	const hkSingleReal m1xy = _mm_mul_pd(vectorIn.m_quad.xy, a1.m_quad.xy);
	const hkSingleReal m2xy = _mm_mul_pd(vectorIn.m_quad.xy, a2.m_quad.xy);

	const hkSingleReal m0zw = _mm_mul_pd(vectorIn.m_quad.zw, a0.m_quad.zw);
	const hkSingleReal m1zw = _mm_mul_pd(vectorIn.m_quad.zw, a1.m_quad.zw);
	const hkSingleReal m2zw = _mm_mul_pd(vectorIn.m_quad.zw, a2.m_quad.zw);

	const hkSingleReal hsum0 = _mm_hadd_pd(m0xy, m0zw);
	const hkSingleReal hsum1 = _mm_hadd_pd(m1xy, m1zw);
	const hkSingleReal hsum2 = _mm_hadd_pd(m2xy, m2zw);

	const hkSingleReal dp0 = _mm_hadd_pd(hsum0, hsum0);
	const hkSingleReal dp1 = _mm_hadd_pd(hsum1, hsum1);
	const hkSingleReal dp2 = _mm_hadd_pd(hsum2, hsum2);

	dotsOut.m_quad.xy = _mm_move_sd(dp1,dp0);
	dotsOut.m_quad.zw = dp2;
}

#endif // sse specials


#define HK_VECTOR4UTIL_convertQuaternionToRotation
void HK_CALL hkVector4Util::convertQuaternionToRotation( hkQuaternionParameter qi, hkRotation& rotationOut )
{
	HK_MATH_ASSERT(0x1ff88f0e, qi.isOk(), "hkQuaternion not normalized/invalid!");

	static HK_ALIGN16( const hkUint64 negateMask0[2] ) = { 0x0000000000000000, 0x8000000000000000 };
	static HK_ALIGN16( const hkUint64 negateMask1[2] ) = { 0x8000000000000000, 0x0000000000000000 };
	static HK_ALIGN16( const hkUint64 negateMask2[2] ) = { 0x8000000000000000, 0x8000000000000000 };

	const hkQuadReal xyzw = qi.m_vec.m_quad;
	hkQuadReal xyzw2;
	xyzw2.xy = _mm_add_pd(xyzw.xy,xyzw.xy);									// xyzw2 = x2, y2, z2, w2
	xyzw2.zw = _mm_add_pd(xyzw.zw,xyzw.zw);

	// temps
	__m128d z2z2 = _mm_shuffle_pd(xyzw2.zw,xyzw2.zw, _MM_SHUFFLE2(0,0));
	__m128d tmp1 = _mm_mul_sd(xyzw.xy,xyzw2.xy);							// tmp1 = xx2, y

	// calculate the 9 products

	hkQuadReal tmp2;
	{
		__m128d yx = _mm_shuffle_pd(xyzw.xy,xyzw.xy, _MM_SHUFFLE2(0,1));
		__m128d y2y2 = _mm_shuffle_pd(xyzw2.xy,xyzw2.xy, _MM_SHUFFLE2(1,1));
		tmp2.xy = _mm_mul_pd(yx,y2y2);
		tmp2.zw = _mm_mul_pd(xyzw.xy,z2z2);									// tmp2 = yy2, xy2, xz2, yz2
	}

	hkQuadReal tmp4;
	{
		__m128d ww = _mm_shuffle_pd(xyzw.zw,xyzw.zw, _MM_SHUFFLE2(1,1));
		__m128d y2x2 = _mm_shuffle_pd(xyzw2.xy,xyzw2.xy, _MM_SHUFFLE2(0,1));
		tmp4.xy = _mm_mul_pd(xyzw.zw,z2z2);
		tmp4.zw = _mm_mul_pd(ww,y2x2);										// tmp4 = zz2, wz2, wy2, wx2
	}

	// calculate the last two elements of the third row
	__m128d tmp6 = g_vectorConstants[HK_QUADREAL_1000].xy;					// tmp6 = 1, 0
	tmp6 = _mm_sub_sd( tmp6, tmp1 );										// tmp6 = -xx2+1, 0
	tmp6 = _mm_sub_sd( tmp6, tmp2.xy );										// tmp6 = -xx2-yy2+1, 0

	// calculate first row
	tmp2.xy = _mm_xor_pd( tmp2.xy, *(const __m128d*)&negateMask0 );
	tmp2.zw = _mm_xor_pd( tmp2.zw, *(const __m128d*)&negateMask2 );			// tmp2 = yy2, -xy2, -xz2, -yz2
	tmp4.xy = _mm_xor_pd( tmp4.xy, *(const __m128d*)&negateMask1 );
	tmp4.zw = _mm_xor_pd( tmp4.zw, *(const __m128d*)&negateMask2 );			// tmp4 = -zz2,   wz2, -wy2, -wx2
	tmp4.xy = _mm_add_sd (tmp4.xy, g_vectorConstants[HK_QUADREAL_1000].xy );// tmp4 = -zz2+1, wz2, -wy2, -wx2
	hkVector4 tmp3;
	{
		tmp3.m_quad.xy = _mm_sub_pd(tmp4.xy,tmp2.xy);
		tmp3.m_quad.zw = _mm_sub_pd(tmp4.zw,tmp2.zw);						// tmp3 = -yy2-zz2+1, xy2+wz2, xz2-wy2, yz2-wx2
	}
	rotationOut.setColumn<0>(tmp3);											// row0 = -yy2-zz2+1, xy2+wz2, xz2-wy2, yz2-wx2

	// calculate second row
	tmp2.xy = _mm_move_sd( tmp2.xy, tmp1 );									// tmp2 = xx2, -xy2, -xz2, -yz2
	tmp4.xy = _mm_xor_pd( tmp4.xy, *(const __m128d*)&negateMask0 );
	tmp4.zw = _mm_xor_pd( tmp4.zw, *(const __m128d*)&negateMask2 );			// tmp4 = -zz2+1, -wz2, wy2, wx2
	tmp4.xy = _mm_sub_pd(tmp4.xy,tmp2.xy);
	tmp4.zw = _mm_sub_pd(tmp4.zw,tmp2.zw);									// tmp4 = -xx2-zz2+1, xy2-wz2, xz2+wy2, yz2+wx2
	hkVector4 tmp5;
	{
		tmp5.m_quad.xy = _mm_shuffle_pd(tmp4.xy,tmp4.xy,_MM_SHUFFLE2(0,1));
		tmp5.m_quad.zw = _mm_shuffle_pd(tmp4.zw,tmp4.zw,_MM_SHUFFLE2(0,1));	// tmp5 = xy2-wz2, -xx2-zz2+1, yz2+wx2, xz2+wy2
	}
	rotationOut.setColumn<1>(tmp5);											// row1 = xy2-wz2, -xx2-zz2+1, yz2+wx2, xz2+wy2

	// calculate third row
	{
		tmp3.m_quad.xy = _mm_shuffle_pd(tmp5.m_quad.zw,tmp3.m_quad.zw,_MM_SHUFFLE2(1,1));
		tmp3.m_quad.zw = tmp6;												// tmp3 = xz2+wy2, yz2-wx2, -xx2-yy2+1, 0
	}
	rotationOut.setColumn<2>(tmp3);											// row2 = xz2+wy2, yz2-wx2, -xx2-yy2+1, 0
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
