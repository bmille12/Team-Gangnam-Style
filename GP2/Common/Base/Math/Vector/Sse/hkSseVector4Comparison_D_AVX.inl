/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2012 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */

#if !defined(HK_REAL_IS_DOUBLE) || HK_SSE_VERSION < 0x50
#error This implementation is for 64-Bit double with AVX SIMD instruction set
#endif

static HK_ALIGN32 (const hkInt64 hkVector4Comparison_maskToComparison[16*4]) = 
{
	0,  0, 0, 0,
	~0,  0, 0, 0,
	0, ~0, 0, 0,
	~0, ~0, 0, 0,
	0,  0,~0, 0,
	~0,  0,~0, 0,
	0, ~0,~0, 0,
	~0, ~0,~0, 0,
	0,  0, 0,~0,
	~0,  0, 0,~0,
	0, ~0, 0,~0,
	~0, ~0, 0,~0,
	0,  0,~0,~0,
	~0,  0,~0,~0,
	0, ~0,~0,~0,
	~0, ~0,~0,~0,
};


HK_FORCE_INLINE /*static*/ const hkVector4Comparison HK_CALL hkVector4Comparison::convert(const hkVector4Mask& x)
{
	hkVector4Comparison c;
	c.m_mask = x;
	return c;
}

HK_FORCE_INLINE void hkVector4Comparison::setAnd( hkVector4ComparisonParameter a, hkVector4ComparisonParameter b )
{ 
	m_mask = _mm256_and_pd( a.m_mask, b.m_mask ); 
}

HK_FORCE_INLINE void hkVector4Comparison::setAndNot( hkVector4ComparisonParameter a, hkVector4ComparisonParameter b )
{
	m_mask = _mm256_andnot_pd(b.m_mask, a.m_mask);
}

HK_FORCE_INLINE void hkVector4Comparison::setNot( hkVector4ComparisonParameter a )
{ 
	m_mask = _mm256_andnot_pd(a.m_mask, *(const __m256d*)&(hkVector4Comparison_maskToComparison[15*4]));
}

HK_FORCE_INLINE void hkVector4Comparison::setOr( hkVector4ComparisonParameter a, hkVector4ComparisonParameter b ) 
{ 
	m_mask = _mm256_or_pd( a.m_mask, b.m_mask ); 
}

HK_FORCE_INLINE void hkVector4Comparison::setSelect( hkVector4ComparisonParameter comp, hkVector4ComparisonParameter trueValue, hkVector4ComparisonParameter falseValue ) 
{
	m_mask = _mm256_blendv_pd(falseValue.m_mask, trueValue.m_mask, comp.m_mask);
}

HK_FORCE_INLINE void hkVector4Comparison::set( Mask m ) 
{ 
	HK_COMPILE_TIME_ASSERT( hkVector4Comparison::INDEX_X == 0);
	HK_COMPILE_TIME_ASSERT( hkVector4Comparison::MASK_X == 1);
	HK_COMPILE_TIME_ASSERT( hkVector4Comparison::MASK_Y == 2);
	HK_COMPILE_TIME_ASSERT( hkVector4Comparison::MASK_XYZW == 15);

	HK_MATH_ASSERT(0x557dac2a, (m&0xf)==m, "illegal mask value handed in");

	m_mask = _mm256_load_pd( (const hkDouble64*)&(hkVector4Comparison_maskToComparison[m*4]) );
}

template <hkVector4Comparison::Mask M>
HK_FORCE_INLINE void hkVector4Comparison::set() 
{ 
	HK_VECTORCOMPARISON_MASK_CHECK;
	if (M == MASK_NONE)
	{
		m_mask = _mm256_setzero_pd();
	}
	else
	{
		const __m256d zeros = _mm256_setzero_pd();
		const __m256d ones = _mm256_load_pd( (const hkDouble64*)&(hkVector4Comparison_maskToComparison[15*4]) );
		if (M == MASK_XYZW)
		{
			m_mask = ones;
		}
		else
		{
			m_mask = _mm256_blend_pd(zeros,ones,M);
		}
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
		__m256d comp = _mm256_load_pd( (const hkDouble64*)&(hkVector4Comparison_maskToComparison[M*4]) );
		__m256d mask = _mm256_and_pd(m_mask, comp);
		return _mm256_testc_pd(mask, comp);
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
		return _mm256_movemask_pd(m_mask); 
	}
	else
	{
		return _mm256_movemask_pd(m_mask) & M;
	}
}

HK_FORCE_INLINE hkBool32 hkVector4Comparison::allAreSet( Mask m ) const 
{ 
	HK_MATH_ASSERT(0x71b4006b, (m&0xf)==m, "illegal mask value handed in");
	__m256d comp = _mm256_load_pd( (const hkDouble64*)&(hkVector4Comparison_maskToComparison[m*4]) );
	__m256d mask = _mm256_and_pd(m_mask, comp);
	return _mm256_testc_pd(mask, comp);
}

HK_FORCE_INLINE hkBool32 hkVector4Comparison::anyIsSet( Mask m ) const 
{ 
	HK_MATH_ASSERT(0x68b44aee, (m&0xf)==m, "illegal mask value handed in");
	return _mm256_movemask_pd(m_mask) & m;
}

HK_FORCE_INLINE hkBool32 hkVector4Comparison::allAreSet() const 
{ 
	return _mm256_testc_pd(m_mask, *(const __m256d*)&(hkVector4Comparison_maskToComparison[15*4]));
}

HK_FORCE_INLINE hkBool32 hkVector4Comparison::anyIsSet() const 
{ 
	return _mm256_movemask_pd(m_mask); 
}

HK_FORCE_INLINE hkVector4Comparison::Mask hkVector4Comparison::getMask() const 
{ 
	return (hkVector4Comparison::Mask)_mm256_movemask_pd(m_mask);
}

HK_FORCE_INLINE hkVector4Comparison::Mask hkVector4Comparison::getMask(Mask m) const 
{ 
	HK_MATH_ASSERT(0x3d2fea61, (m&0xf)==m, "illegal mask value handed in");
	return (hkVector4Comparison::Mask)( _mm256_movemask_pd(m_mask) & m );
}

template <hkVector4Comparison::Mask M>
HK_FORCE_INLINE hkVector4Comparison::Mask hkVector4Comparison::getMask() const 
{ 
	HK_VECTORCOMPARISON_MASK_CHECK;
	return (hkVector4Comparison::Mask)( _mm256_movemask_pd(m_mask) & M );
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
	const hkQuadReal xxzz = _mm256_movedup_pd(m_mask);

	return hkVector4Comparison::convert(_mm256_permute2f128_pd(xxzz, xxzz, _MM256_PERMUTE2(_MM256_A_LOW, _MM256_A_LOW)));
}

template <>
HK_FORCE_INLINE const hkVector4Comparison hkVector4Comparison::horizontalOr<2>() const
{
	const hkQuadReal yx		= _mm256_permute_pd(m_mask, 0x1);	// [y, x, ...]
	const hkQuadReal xy_xy	= _mm256_or_pd(m_mask, yx);			// [x | y, x | y, ...]

	return hkVector4Comparison::convert(_mm256_permute2f128_pd(xy_xy, xy_xy, _MM256_PERMUTE2(_MM256_A_LOW, _MM256_A_LOW)));
}

template <>
HK_FORCE_INLINE const hkVector4Comparison hkVector4Comparison::horizontalOr<3>() const
{
	const hkQuadReal yxzz	= _mm256_permute_pd(m_mask, 0x1);	// [y, x, z, z]
	const hkQuadReal zz		= _mm256_extractf128_pd(yxzz, 1);	// [z, z, z, z]
	const hkQuadReal xy_xy	= _mm256_or_pd(m_mask, yxzz);		// [x | y, y | x, ...]
	const hkQuadReal xyz	= _mm256_or_pd(xy_xy, zz);			// [x | y | z, x | y | z, ...]

	return hkVector4Comparison::convert(_mm256_permute2f128_pd(xyz, xyz, _MM256_PERMUTE2(_MM256_A_LOW, _MM256_A_LOW)));
}

template <>
HK_FORCE_INLINE const hkVector4Comparison hkVector4Comparison::horizontalOr<4>() const
{
	const hkQuadReal yxwz	= _mm256_permute_pd(m_mask, 0x5);	// [y, x, w, z]
	const hkQuadReal xy_zw	= _mm256_or_pd(m_mask, yxwz);		// [x | y, y | x, z | w, w | z]
	const hkQuadReal zw_xy	= _mm256_permute2f128_pd(xy_zw, xy_zw, _MM256_PERMUTE2(_MM256_A_LOW, _MM256_A_HIGH)); // [z | w, w | z, x | y, y | x]
	const hkQuadReal xyzw	= _mm256_or_pd(xy_zw, zw_xy ); 

	return hkVector4Comparison::convert(xyzw);
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
