/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2012 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */

#if defined(HK_REAL_IS_DOUBLE) || (HK_SSE_VERSION < 0x50)
#error This implementation is for 32-Bit float with AVX SIMD instruction set
#endif


template <int M>
HK_FORCE_INLINE void hkMxMask<M>::horizontalAnd4( hkVector4Comparison& andOut ) const
{
	HK_MXVECTOR_MX_NOT_IMPLEMENTED;
}

template <>
HK_FORCE_INLINE void hkMxMask<1>::horizontalAnd4( hkVector4Comparison& andOut ) const
{
	const int mask = _mm256_movemask_ps(m_comp.c[0]) & 0xf;
	andOut.set( (mask == 0xf) ? hkVector4Comparison::MASK_X : hkVector4Comparison::MASK_NONE );
}

template <>
HK_FORCE_INLINE void hkMxMask<2>::horizontalAnd4( hkVector4Comparison& andOut ) const
{
	const int bitmask = _mm256_movemask_ps(m_comp.c[0]);

	int mask =	( ((bitmask & 0x0f) == 0x0f) ? hkVector4Comparison::MASK_X : hkVector4Comparison::MASK_NONE ) |
				( ((bitmask & 0xf0) == 0xf0) ? hkVector4Comparison::MASK_Y : hkVector4Comparison::MASK_NONE );

	andOut.set( (hkVector4Comparison::Mask)mask );
}

template <>
HK_FORCE_INLINE void hkMxMask<3>::horizontalAnd4( hkVector4Comparison& andOut ) const
{
	const int bitmask0 = _mm256_movemask_ps(m_comp.c[0]);
	const int bitmask1 = _mm256_movemask_ps(m_comp.c[1]);

	int mask =	( ((bitmask0 & 0x0f) == 0x0f) ? hkVector4Comparison::MASK_X : hkVector4Comparison::MASK_NONE ) |
				( ((bitmask0 & 0xf0) == 0xf0) ? hkVector4Comparison::MASK_Y : hkVector4Comparison::MASK_NONE ) |
				( ((bitmask1 & 0x0f) == 0x0f) ? hkVector4Comparison::MASK_Z : hkVector4Comparison::MASK_NONE );

	andOut.set( (hkVector4Comparison::Mask)mask );
}

template <>
HK_FORCE_INLINE void hkMxMask<4>::horizontalAnd4( hkVector4Comparison& andOut ) const
{
	const int bitmask0 = _mm256_movemask_ps(m_comp.c[0]);
	const int bitmask1 = _mm256_movemask_ps(m_comp.c[1]);

	int mask =	( ((bitmask0 & 0x0f) == 0x0f) ? hkVector4Comparison::MASK_X : hkVector4Comparison::MASK_NONE ) |
				( ((bitmask0 & 0xf0) == 0xf0) ? hkVector4Comparison::MASK_Y : hkVector4Comparison::MASK_NONE ) |
				( ((bitmask1 & 0x0f) == 0x0f) ? hkVector4Comparison::MASK_Z : hkVector4Comparison::MASK_NONE ) |
				( ((bitmask1 & 0xf0) == 0xf0) ? hkVector4Comparison::MASK_W : hkVector4Comparison::MASK_NONE ) ;

	andOut.set( (hkVector4Comparison::Mask)mask );
}




namespace hkMxMask_Implementation
{
template <int I> HK_FORCE_INLINE void setMH(__m256* m, const __m256& mask) { setMH<I-1>(m,mask); m[I-1] = mask; }
template <> HK_FORCE_INLINE void setMH<1>(__m256* m, const __m256& mask) { m[0] = mask; } 
}
template <int M>
template <hkVector4Comparison::Mask MASK>
HK_FORCE_INLINE void hkMxMask<M>::setAll()
{
	const __m256 mask = _mm256_broadcast_ps((const __m128*)&hkVector4Comparison_maskToComparison[MASK*4]);
	hkMxMask_Implementation::setMH<((M+1)>>1)>(m_comp.c, mask);
}

template <int M>
template <int I>
HK_FORCE_INLINE void hkMxMask<M>::set( hkVector4ComparisonParameter a )
{
	HK_MXVECTOR_MX_SUBINDEX_CHECK;
	const int subvector = I>>1;
	const int subvectorsubindex = ((I+1)>>1) - subvector;
	m_comp.c[subvector] = _mm256_insertf128_ps(m_comp.c[subvector], a.m_mask, subvectorsubindex);
}

template <int M>
template <int I>
HK_FORCE_INLINE const hkVector4Comparison& hkMxMask<M>::get() const
{
	HK_MXVECTOR_MX_SUBINDEX_CHECK;
	const int subvector = I>>1;
	const int subvectorsubindex = ((I+1)>>1) - subvector;
	hkVector4Comparison c;
	c.m_mask = _mm256_extractf128_ps(m_comp.c[subvector], subvectorsubindex);
	return c;
}


namespace hkMxMask_Implementation
{
template <int I> HK_FORCE_INLINE void setAndMH(__m256* m, const __m256* a, const __m256* b) { setAndMH<I-1>(m,a,b); m[I-1] = _mm256_and_ps(a[I-1], b[I-1]); }
template <> HK_FORCE_INLINE void setAndMH<1>(__m256* m, const __m256* a, const __m256* b) { m[0] = _mm256_and_ps(a[0], b[0]); }
}
template <int M>
HK_FORCE_INLINE void hkMxMask<M>::setAnd(hkMxMaskParameter a, hkMxMaskParameter b)
{
	hkMxMask_Implementation::setAndMH<((M+1)>>1)>(m_comp.c, a.m_comp.c, b.m_comp.c);
}

namespace hkMxMask_Implementation
{
template <int I> HK_FORCE_INLINE void setOrMH(__m256* m, const __m256* a, const __m256* b) { setOrMH<I-1>(m,a,b); m[I-1] = _mm256_or_ps(a[I-1], b[I-1]); }
template <> HK_FORCE_INLINE void setOrMH<1>(__m256* m, const __m256* a, const __m256* b) { m[0] = _mm256_or_ps(a[0], b[0]); }
}
template <int M>
HK_FORCE_INLINE void hkMxMask<M>::setOr(hkMxMaskParameter a, hkMxMaskParameter b)
{
	hkMxMask_Implementation::setOrMH<((M+1)>>1)>(m_comp.c, a.m_comp.c, b.m_comp.c);
}


namespace hkMxMask_Implementation
{
template <int I, int N> struct anyIsSetMH { HK_FORCE_INLINE static hkBool32 apply(const __m256* m) 
{ 
	return _mm256_movemask_ps(m[N]) | anyIsSetMH<I-2,N+1>::apply(m); 
} };
template <int N> struct anyIsSetMH<2,N> { HK_FORCE_INLINE static hkBool32 apply(const __m256* m) 
{ 
	return _mm256_movemask_ps(m[N]); 
} };
template <int N> struct anyIsSetMH<1,N> { HK_FORCE_INLINE static hkBool32 apply(const __m256* m) 
{ 
	return _mm256_movemask_ps(m[N]) & 0x0f; 
} };
}
template <int M>
HK_FORCE_INLINE hkBool32 hkMxMask<M>::anyIsSet() const
{
	return hkMxMask_Implementation::anyIsSetMH<M,0>::apply(m_comp.c);
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
