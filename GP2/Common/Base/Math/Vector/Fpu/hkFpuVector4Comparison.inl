/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2012 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */

HK_FORCE_INLINE /*static*/ const hkVector4Comparison HK_CALL hkVector4Comparison::convert(const hkVector4Mask& x)
{
	hkVector4Comparison c;
	c.m_mask = x;
	return c;
}

HK_FORCE_INLINE void hkVector4Comparison::setAnd( hkVector4ComparisonParameter a, hkVector4ComparisonParameter b )
{ 
	m_mask = a.m_mask & b.m_mask; 
}

HK_FORCE_INLINE void hkVector4Comparison::setAndNot( hkVector4ComparisonParameter a, hkVector4ComparisonParameter b )
{
	m_mask = a.m_mask & (~b.m_mask);
}

HK_FORCE_INLINE void hkVector4Comparison::setXor( hkVector4ComparisonParameter a, hkVector4ComparisonParameter b )
{
	m_mask = a.m_mask ^ b.m_mask;
}

HK_FORCE_INLINE void hkVector4Comparison::setNot( hkVector4ComparisonParameter a )
{ 
	m_mask = (~a.m_mask) & MASK_XYZW;
}

HK_FORCE_INLINE void hkVector4Comparison::setOr( hkVector4ComparisonParameter a, hkVector4ComparisonParameter b ) 
{ 
	m_mask = a.m_mask | b.m_mask;
}

HK_FORCE_INLINE void hkVector4Comparison::setSelect( hkVector4ComparisonParameter comp, hkVector4ComparisonParameter trueValue, hkVector4ComparisonParameter falseValue ) 
{
	m_mask = (comp.m_mask & trueValue.m_mask) | (~comp.m_mask & falseValue.m_mask);
}

HK_FORCE_INLINE void hkVector4Comparison::set( Mask m ) 
{ 
	HK_MATH_ASSERT(0x610ba416, (m&0xf)==m, "illegal mask value handed in");
	m_mask = m;
}

template <hkVector4Comparison::Mask M>
HK_FORCE_INLINE void hkVector4Comparison::set()
{ 
	HK_VECTORCOMPARISON_MASK_CHECK;
	m_mask = M;
}

HK_FORCE_INLINE hkBool32 hkVector4Comparison::allAreSet( Mask m ) const 
{ 
	HK_MATH_ASSERT(0x217e76eb, (m&0xf)==m, "illegal mask value handed in");
	return (m_mask & m) == m;
}

template <hkVector4Comparison::Mask M>
HK_FORCE_INLINE hkBool32 hkVector4Comparison::allAreSet() const 
{ 
	HK_VECTORCOMPARISON_MASK_CHECK;
	if (M == MASK_NONE)
	{
		return true;
	}
	else if (M == MASK_XYZW)
	{
		return m_mask == MASK_XYZW;
	}
	else
	{
		return (m_mask & M) == M;
	}
}

HK_FORCE_INLINE hkBool32 hkVector4Comparison::anyIsSet( Mask m ) const 
{ 
	HK_MATH_ASSERT(0x21e05e49, (m&0xf)==m, "illegal mask value handed in");
	return m_mask & m;
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
		return m_mask;
	}
	else
	{
		return m_mask & M;
	}
}

HK_FORCE_INLINE hkBool32 hkVector4Comparison::allAreSet() const 
{ 
	return m_mask == MASK_XYZW;
}

HK_FORCE_INLINE hkBool32 hkVector4Comparison::anyIsSet() const 
{ 
	return m_mask;
}

HK_FORCE_INLINE hkVector4Comparison::Mask hkVector4Comparison::getMask() const 
{ 
	return (hkVector4Comparison::Mask)(m_mask);
}

HK_FORCE_INLINE hkVector4Comparison::Mask hkVector4Comparison::getMask(Mask m) const 
{ 
	HK_MATH_ASSERT(0x6b7c1f6a, (m&0xf)==m, "illegal mask value handed in");
	return (hkVector4Comparison::Mask)(m_mask & m);
}

template <hkVector4Comparison::Mask M>
HK_FORCE_INLINE hkVector4Comparison::Mask hkVector4Comparison::getMask() const 
{ 
	HK_VECTORCOMPARISON_MASK_CHECK;
	return (hkVector4Comparison::Mask)(m_mask & M);
}

HK_FORCE_INLINE /*static*/ hkUint32 HK_CALL hkVector4Comparison::getCombinedMask(hkVector4ComparisonParameter ca, hkVector4ComparisonParameter cb, hkVector4ComparisonParameter cc )
{
	return ca.m_mask | (cb.m_mask<<4 ) | (cc.m_mask<<8);
}

//
//	Horizontal or

template <>
HK_FORCE_INLINE const hkVector4Comparison hkVector4Comparison::horizontalOr<1>() const
{
	return hkVector4Comparison::convert((hkVector4Mask)((m_mask & MASK_X) ? MASK_XYZW : MASK_NONE));
}

template <>
HK_FORCE_INLINE const hkVector4Comparison hkVector4Comparison::horizontalOr<2>() const
{
	return hkVector4Comparison::convert((hkVector4Mask)((m_mask & MASK_XY) ? MASK_XYZW : MASK_NONE));
}

template <>
HK_FORCE_INLINE const hkVector4Comparison hkVector4Comparison::horizontalOr<3>() const
{
	return hkVector4Comparison::convert((hkVector4Mask)((m_mask & MASK_XYZ) ? MASK_XYZW : MASK_NONE));
}

template <>
HK_FORCE_INLINE const hkVector4Comparison hkVector4Comparison::horizontalOr<4>() const
{
	return hkVector4Comparison::convert((hkVector4Mask)(m_mask ? MASK_XYZW : MASK_NONE));
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
