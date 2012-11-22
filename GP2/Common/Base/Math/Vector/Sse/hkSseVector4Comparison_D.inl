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

static HK_ALIGN16 (const hkInt64 hkVector4Comparison_maskToComparison[4*2]) = 
{
	 0,  0,
	~0,  0,
	 0, ~0,
	~0, ~0,
};

HK_FORCE_INLINE int calcMask(int xy, int zw)
{
	return xy | ( zw << 2 );
}

HK_FORCE_INLINE /*static*/ const hkVector4Comparison HK_CALL hkVector4Comparison::convert(const hkVector4Mask& x)
{
	hkVector4Comparison c;
	c.m_mask = x;
	return c;
}

HK_FORCE_INLINE void hkVector4Comparison::setAnd( hkVector4ComparisonParameter a, hkVector4ComparisonParameter b )
{ 
	m_mask.xy = _mm_and_pd( a.m_mask.xy, b.m_mask.xy ); 
	m_mask.zw = _mm_and_pd( a.m_mask.zw, b.m_mask.zw ); 
}

HK_FORCE_INLINE void hkVector4Comparison::setAndNot( hkVector4ComparisonParameter a, hkVector4ComparisonParameter b )
{
	m_mask.xy = _mm_andnot_pd(b.m_mask.xy, a.m_mask.xy);
	m_mask.zw = _mm_andnot_pd(b.m_mask.zw, a.m_mask.zw);
}

HK_FORCE_INLINE void hkVector4Comparison::setNot( hkVector4ComparisonParameter a )
{ 
	const __m128i zeros = _mm_setzero_si128();
	m_mask.xy = _mm_castsi128_pd(_mm_cmpeq_epi32(zeros, _mm_castpd_si128(a.m_mask.xy)));
	m_mask.zw = _mm_castsi128_pd(_mm_cmpeq_epi32(zeros, _mm_castpd_si128(a.m_mask.zw)));
}

HK_FORCE_INLINE void hkVector4Comparison::setOr( hkVector4ComparisonParameter a, hkVector4ComparisonParameter b ) 
{ 
	m_mask.xy = _mm_or_pd( a.m_mask.xy, b.m_mask.xy ); 
	m_mask.zw = _mm_or_pd( a.m_mask.zw, b.m_mask.zw ); 
}

HK_FORCE_INLINE void hkVector4Comparison::setSelect( hkVector4ComparisonParameter comp, hkVector4ComparisonParameter trueValue, hkVector4ComparisonParameter falseValue ) 
{
#if HK_SSE_VERSION >= 0x41
	m_mask.xy = _mm_blendv_pd(falseValue.m_mask.xy, trueValue.m_mask.xy, comp.m_mask.xy);
	m_mask.zw = _mm_blendv_pd(falseValue.m_mask.zw, trueValue.m_mask.zw, comp.m_mask.zw);
#else
	m_mask.xy = _mm_or_pd( _mm_and_pd(comp.m_mask.xy, trueValue.m_mask.xy), _mm_andnot_pd(comp.m_mask.xy, falseValue.m_mask.xy) );
	m_mask.zw = _mm_or_pd( _mm_and_pd(comp.m_mask.zw, trueValue.m_mask.zw), _mm_andnot_pd(comp.m_mask.zw, falseValue.m_mask.zw) );
#endif
}

HK_FORCE_INLINE void hkVector4Comparison::set( Mask m ) 
{ 
	HK_COMPILE_TIME_ASSERT( hkVector4Comparison::INDEX_X == 0);
	HK_COMPILE_TIME_ASSERT( hkVector4Comparison::MASK_X == 1);
	HK_COMPILE_TIME_ASSERT( hkVector4Comparison::MASK_Y == 2);
	HK_COMPILE_TIME_ASSERT( hkVector4Comparison::MASK_XYZW == 15);

	HK_MATH_ASSERT(0x557dac2a, (m&0xf)==m, "illegal mask value handed in");

	m_mask.xy = _mm_load_pd( (const hkDouble64*)&(hkVector4Comparison_maskToComparison[(m&0x3)*2]) );
	m_mask.zw = _mm_load_pd( (const hkDouble64*)&(hkVector4Comparison_maskToComparison[(m>>2)*2])  );
}

template <hkVector4Comparison::Mask M>
HK_FORCE_INLINE void hkVector4Comparison::set() 
{ 
	HK_VECTORCOMPARISON_MASK_CHECK;
	if (M == MASK_NONE)
	{
		m_mask.xy = _mm_setzero_pd();
		m_mask.zw = _mm_setzero_pd();
	}
	else
	{
#if HK_SSE_VERSION >= 0x41
		const hkSingleReal zeros = _mm_setzero_pd();
		const hkSingleReal ones = _mm_castsi128_pd(_mm_cmpeq_epi32(_mm_setzero_si128(),_mm_setzero_si128()));
		if (M == MASK_XYZW)
		{
			m_mask.xy = ones;
			m_mask.zw = ones;
		}
		else
		{
			if (M & 0x3)      m_mask.xy = _mm_blend_pd(zeros,ones, M & 0x3);      else m_mask.xy = zeros;
			if ((M>>2) & 0x3) m_mask.zw = _mm_blend_pd(zeros,ones, (M>>2) & 0x3); else m_mask.zw = zeros;
		}
#else
		m_mask.xy = _mm_load_pd( (const hkDouble64*)&(hkVector4Comparison_maskToComparison[(M&0x3)*2]) );
		m_mask.zw = _mm_load_pd( (const hkDouble64*)&(hkVector4Comparison_maskToComparison[(M>>2)*2])  );
#endif
	}
}

template <hkVector4Comparison::Mask M>
HK_FORCE_INLINE hkBool32 hkVector4Comparison::allAreSet() const 
{ 
	HK_VECTORCOMPARISON_MASK_CHECK;
	if (M == MASK_NONE)
	{
		return true;
	}
	else
	{
#if HK_SSE_VERSION >= 0x41
		if (M == MASK_XYZW)
		{
			return _mm_test_all_ones(_mm_castpd_si128(m_mask.xy)) & _mm_test_all_ones(_mm_castpd_si128(m_mask.zw));
		}
		else
		{
			__m128i comp_xy;
			__m128i comp_zw;
			const hkSingleReal zeros = _mm_setzero_pd();
			const hkSingleReal ones = _mm_castsi128_pd(_mm_cmpeq_epi32(_mm_setzero_si128(),_mm_setzero_si128()));
			if (M & 0x3)      comp_xy = _mm_castpd_si128(_mm_blend_pd(zeros,ones, M & 0x3));      else comp_xy = _mm_castpd_si128(zeros);
			if ((M>>2) & 0x3) comp_zw = _mm_castpd_si128(_mm_blend_pd(zeros,ones, (M>>2) & 0x3)); else comp_zw = _mm_castpd_si128(zeros);
			__m128i m_xy = _mm_and_si128(_mm_castpd_si128(m_mask.xy), comp_xy);
			__m128i m_zw = _mm_and_si128(_mm_castpd_si128(m_mask.zw), comp_zw);
			return _mm_testc_si128(m_xy, comp_xy) & _mm_testc_si128(m_zw, comp_zw);
		}
#else
		int yx = _mm_movemask_pd(m_mask.xy);
		int wz = _mm_movemask_pd(m_mask.zw);
		int xyzw = calcMask(yx, wz);
		return (xyzw & M) == M; 
#endif
	}
}

template <hkVector4Comparison::Mask M>
HK_FORCE_INLINE hkBool32 hkVector4Comparison::anyIsSet() const 
{ 
	HK_VECTORCOMPARISON_MASK_CHECK;
	if (M == MASK_NONE)
	{
		return false;
	}
	else if (M == MASK_XYZW)
	{
		int yx = _mm_movemask_pd(m_mask.xy); 
		int wz = _mm_movemask_pd(m_mask.zw); 
		return yx | wz;
	}
	else
	{
		int yx = _mm_movemask_pd(m_mask.xy); 
		int wz = _mm_movemask_pd(m_mask.zw); 
		int xyzw = calcMask(yx, wz);
		return xyzw & M;
	}
}

HK_FORCE_INLINE hkBool32 hkVector4Comparison::allAreSet( Mask m ) const 
{ 
	HK_MATH_ASSERT(0x71b4006b, (m&0xf)==m, "illegal mask value handed in");
#if HK_SSE_VERSION >= 0x41
	__m128i comp_xy = _mm_load_si128( (const __m128i*)&(hkVector4Comparison_maskToComparison[(m&0x3)*2]) );
	__m128i comp_zw = _mm_load_si128( (const __m128i*)&(hkVector4Comparison_maskToComparison[(m>>2)*2])  );
	__m128i m_xy = _mm_and_si128(_mm_castpd_si128(m_mask.xy), comp_xy);
	__m128i m_zw = _mm_and_si128(_mm_castpd_si128(m_mask.zw), comp_zw);
	return _mm_testc_si128(m_xy, comp_xy) & _mm_testc_si128(m_zw, comp_zw);
#else
	int yx = _mm_movemask_pd(m_mask.xy);
	int wz = _mm_movemask_pd(m_mask.zw);
	int xyzw = calcMask(yx, wz);
	return (xyzw & m) == m; 
#endif
}

HK_FORCE_INLINE hkBool32 hkVector4Comparison::anyIsSet( Mask m ) const 
{ 
	HK_MATH_ASSERT(0x68b44aee, (m&0xf)==m, "illegal mask value handed in");
	int yx = _mm_movemask_pd(m_mask.xy); 
	int wz = _mm_movemask_pd(m_mask.zw); 
	int xyzw = calcMask(yx, wz);
	return xyzw & m;
}

HK_FORCE_INLINE hkBool32 hkVector4Comparison::allAreSet() const 
{ 
#if HK_SSE_VERSION >= 0x41
	return _mm_test_all_ones(_mm_castpd_si128(m_mask.xy)) & _mm_test_all_ones(_mm_castpd_si128(m_mask.zw));
#else
	int yx = _mm_movemask_pd(m_mask.xy); 
	int wz = _mm_movemask_pd(m_mask.zw); 
	int xyzw = calcMask(yx, wz);
	return (xyzw == MASK_XYZW);
#endif
}

HK_FORCE_INLINE hkBool32 hkVector4Comparison::anyIsSet() const 
{ 
	int yx = _mm_movemask_pd(m_mask.xy); 
	int wz = _mm_movemask_pd(m_mask.zw); 
	return yx | wz;
}

HK_FORCE_INLINE hkVector4Comparison::Mask hkVector4Comparison::getMask() const 
{ 
	int yx = _mm_movemask_pd(m_mask.xy); 
	int wz = _mm_movemask_pd(m_mask.zw); 
	int xyzw = calcMask(yx, wz);
	return (hkVector4Comparison::Mask)xyzw;
}

HK_FORCE_INLINE hkVector4Comparison::Mask hkVector4Comparison::getMask(Mask m) const 
{ 
	HK_MATH_ASSERT(0x3d2fea61, (m&0xf)==m, "illegal mask value handed in");
	int yx = _mm_movemask_pd(m_mask.xy); 
	int wz = _mm_movemask_pd(m_mask.zw); 
	int xyzw = calcMask(yx, wz);
	return (hkVector4Comparison::Mask)( xyzw & m );
}

template <hkVector4Comparison::Mask M>
HK_FORCE_INLINE hkVector4Comparison::Mask hkVector4Comparison::getMask() const 
{ 
	HK_VECTORCOMPARISON_MASK_CHECK;
	int yx = _mm_movemask_pd(m_mask.xy); 
	int wz = _mm_movemask_pd(m_mask.zw); 
	int xyzw = calcMask(yx, wz);
	return (hkVector4Comparison::Mask)( xyzw & M );
}

HK_FORCE_INLINE /*static*/ hkUint32 HK_CALL hkVector4Comparison::getCombinedMask(hkVector4ComparisonParameter ca, hkVector4ComparisonParameter cb, hkVector4ComparisonParameter cc )
{
	return ca.getMask() | (cb.getMask()<<4) | (cc.getMask()<<8);
}

//
//	Horizontal or

template <>
HK_FORCE_INLINE const hkVector4Comparison hkVector4Comparison::horizontalOr<1>() const
{
	hkVector4Comparison ret;
	ret.m_mask.xy = _mm_unpacklo_pd(m_mask.xy, m_mask.xy);
	ret.m_mask.zw = ret.m_mask.xy;
	return ret;
}

template <>
HK_FORCE_INLINE const hkVector4Comparison hkVector4Comparison::horizontalOr<2>() const
{
	hkSingleReal xx = _mm_unpacklo_pd(m_mask.xy, m_mask.xy);
	hkSingleReal yy = _mm_unpackhi_pd(m_mask.xy, m_mask.xy);
	
	hkVector4Comparison ret;
	ret.m_mask.xy	= _mm_or_pd(xx, yy);
	ret.m_mask.zw	= ret.m_mask.xy;
	return ret;
}

template <>
HK_FORCE_INLINE const hkVector4Comparison hkVector4Comparison::horizontalOr<3>() const
{
	hkSingleReal xz_yw	= _mm_or_pd(m_mask.xy, m_mask.zw);			// [x | z, y | w]
	hkSingleReal xz_xz	= _mm_unpacklo_pd(xz_yw, xz_yw);			// [x | z, x | z]
	hkSingleReal yy		= _mm_unpackhi_pd(m_mask.xy, m_mask.xy);	// [y, y]

	hkVector4Comparison ret;
	ret.m_mask.xy		= _mm_or_pd(xz_xz, yy);
	ret.m_mask.zw		= ret.m_mask.xy;
	return ret;
}

template <>
HK_FORCE_INLINE const hkVector4Comparison hkVector4Comparison::horizontalOr<4>() const
{
	hkSingleReal xz_yw	= _mm_or_pd(m_mask.xy, m_mask.zw);					// [x | z, y | w]
	hkSingleReal yw_xz	= _mm_shuffle_pd(xz_yw, xz_yw,_MM_SHUFFLE2(0,1));	// [y | w, x | z]

	hkVector4Comparison ret;
	ret.m_mask.xy		= _mm_or_pd(xz_yw, yw_xz);
	ret.m_mask.zw		= ret.m_mask.xy;
	return ret;
}

template <int N>
HK_FORCE_INLINE const hkVector4Comparison hkVector4Comparison::horizontalOr() const
{
	HK_VECTORCOMPARISON_SUBINDEX_CHECK;
	hkVector4Comparison cmp;
	cmp.set<hkVector4Comparison::MASK_NONE>();
	return cmp;
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
