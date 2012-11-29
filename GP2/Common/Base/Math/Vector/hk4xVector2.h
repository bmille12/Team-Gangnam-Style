/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2012 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */
#ifndef HK_4X_VECTOR2_H
#define HK_4X_VECTOR2_H

//#include <Common/Base/Math/Vector/Mx/hkMxVector.h>
#include <Common/Base/Math/Vector/hkVector4Util.h>

class hk4xVector2
{
public:
	template<hkVectorPermutation::Permutation i>
	HK_FORCE_INLINE void setPermutation( const hk4xVector2& v );

	/// this = v[i]
	template<int i>
	HK_FORCE_INLINE void selectVector( const hk4xVector2& v );

	HK_FORCE_INLINE static void dotPerpendicular( const hk4xVector2& a, const hk4xVector2& b, hkVector4& dotOut );

	HK_FORCE_INLINE static void dot( const hk4xVector2& a, const hk4xVector2& b, hkVector4& dotOut );

	HK_FORCE_INLINE void setAddMul( const hk4xVector2& a, const hk4xVector2& b, hkVector4Parameter t );

	HK_FORCE_INLINE void setMul( const hk4xVector2& a, hkVector4Parameter t );

	HK_FORCE_INLINE void set( hkSimdRealParameter x, hkSimdRealParameter y);

	//HK_FORCE_INLINE void set( const hk4xVector4& v );

	HK_FORCE_INLINE void set( hkVector4Parameter uAxis, hkVector4Parameter vAxis, hkVector4Parameter offset,
		hkVector4Parameter v0, hkVector4Parameter v1, hkVector4Parameter v2, hkVector4Parameter v3 );

	HK_FORCE_INLINE void set( hkVector4Parameter uAxis, hkVector4Parameter vAxis, 
		hkVector4Parameter v0, hkVector4Parameter v1, hkVector4Parameter v2, hkVector4Parameter v3 );

	HK_FORCE_INLINE void transform(hkVector4Parameter colX, hkVector4Parameter colY, hkVector4Parameter translate, hkVector4 &x)
	{
		x.setAddMul(translate, colX, m_x);
		x.addMul(colY, m_y);
	}

	template<hkVector4Comparison::Mask M>
	HK_FORCE_INLINE void setSelect( const hk4xVector2& v0, const hk4xVector2& v1 );

	template<hkMathAccuracyMode A, hkMathNegSqrtMode S>
	HK_FORCE_INLINE void setNormalize( const hk4xVector2& v0 );

	// convert to 4 vectors
	//HK_FORCE_INLINE void convertToMxVector( hk4xVector4& v ) const;

	HK_FORCE_INLINE static void getFromArray(const hk4xVector2* HK_RESTRICT vectors, hkUint32 i, hkSimdReal &xOut, hkSimdReal &yOut);

	HK_FORCE_INLINE static hk4xVector2 getFromArray2D(const hk4xVector2* HK_RESTRICT vectors, hkUint32 i);

	HK_FORCE_INLINE void setSelect(hkVector4ComparisonParameter cmp, const hk4xVector2& a0, const hk4xVector2& b0);

#define DEFINE_FUNCTION2(TYPE)	void TYPE( const hk4xVector2& a0, const hk4xVector2& b0)	{		m_x.TYPE( a0.m_x, b0.m_x );		m_y.TYPE( a0.m_y, b0.m_y );	}

//	DEFINE_FUNCTION2(setAdd);
	DEFINE_FUNCTION2(setSub);
	DEFINE_FUNCTION2(setDiv);

	hkVector4 m_x;
	hkVector4 m_y;
};

#include <Common/Base/Math/Vector/hk4xVector2.inl>

#endif // HK_4X_VECTOR2_H

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
