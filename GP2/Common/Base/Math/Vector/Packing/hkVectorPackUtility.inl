/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2012 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */

template< typename T, int NUM_BITS_X, int NUM_BITS_Y >
HK_FORCE_INLINE void hkVectorPackUtility::decode3( const T& vertex, hkVector4* HK_RESTRICT vectorOut )
{
	enum
	{
		TOTAL_BITS	=	sizeof(T) << 3,
		NUM_BITS_Z	=	TOTAL_BITS - (NUM_BITS_X + NUM_BITS_Y),
		MASK_X		=	(1<<NUM_BITS_X)-1,
		MASK_Y		=	(1<<NUM_BITS_Y)-1,
		MASK_Z		=	(1<<NUM_BITS_Z)-1
	};

#if defined(HK_PLATFORM_WIN32) && (HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED)
		static const HK_ALIGN16( hkUint32 maskX[4] ) = { MASK_X, 0, 0, 0 };
		static const HK_ALIGN16( hkUint32 maskY[4] ) = { 0, MASK_Y, 0, 0 };
		static const HK_ALIGN16( hkUint32 maskZ[4] ) = { 0, 0, MASK_Z, 0 };

		if(sizeof(T) == 4)
		{
			hkIntVector a; a.load<1>( (hkUint32*)(&vertex) );

			hkIntVector z; z.setShiftRight32<31&(NUM_BITS_X + NUM_BITS_Y)>( a );z.setBroadcast<0>(z); z.setAnd( z, (const hkIntVector&)maskZ );
			hkIntVector y; y.setShiftRight32<NUM_BITS_X>( a );					y.setBroadcast<0>(y); y.setAnd( y, (const hkIntVector&)maskY );
			hkIntVector x; x.setAnd( a, (const hkIntVector&)maskX );

			hkIntVector	iv; iv.setOr( x, z ); iv.setOr( iv, y );
			iv.convertS32ToF32(*vectorOut);
		}
		else if(sizeof(T) == 8)
		{
			hkIntVector a; a.load<2>( (hkUint32*)(&vertex) );

			hkIntVector y; y.setShiftRight128<NUM_BITS_X/8>( a );
			y.setShiftRight32<NUM_BITS_X&0x7>( y );
			y.setAnd( y, (const hkIntVector&)maskY );

			hkIntVector z; z.setShiftRight32<31&(NUM_BITS_X + NUM_BITS_Y - 32)>( a );
			z.setBroadcast<1>(z);
			z.setAnd( z, (const hkIntVector&)maskZ );

			hkIntVector x; x.setAnd( a, (const hkIntVector&)maskX );

			hkIntVector	iv; iv.setOr( x, z ); iv.setOr( iv, y );
			iv.convertS32ToF32(*vectorOut);
		}
		else
		{
			hkIntVector	iv; iv.set((int) (vertex & MASK_X), (int) ((vertex >> NUM_BITS_X) & MASK_Y), (int) ((vertex >> (NUM_BITS_X + NUM_BITS_Y) ) & MASK_Z), 0);
			iv.convertS32ToF32(*vectorOut);
		}
	#else
		hkIntVector	iv; iv.set((int) (vertex & MASK_X), (int) ((vertex >> NUM_BITS_X) & MASK_Y), (int) ((vertex >> (NUM_BITS_X + NUM_BITS_Y) ) & MASK_Z), 0);
		iv.convertS32ToF32(*vectorOut);
	#endif
}



template< typename T, int NUM_BITS_X, int NUM_BITS_Y >
HK_FORCE_INLINE void hkVectorPackUtility::decodeFractions3( const T& vertex, hkVector4* HK_RESTRICT vectorOut )
{
	enum
	{
		TOTAL_BITS	=	sizeof(T) << 3,
		NUM_BITS_Z	=	TOTAL_BITS - (NUM_BITS_X + NUM_BITS_Y),
		MASK_X		=	(1<<NUM_BITS_X)-1,
		MASK_Y		=	(1<<NUM_BITS_Y)-1,
		MASK_Z		=	(1<<NUM_BITS_Z)-1
	};

	hkVector4	rv;
	hkVectorPackUtility::decode3<T,NUM_BITS_X,NUM_BITS_Y> ( vertex, &rv );

	HK_ALIGN16( hkReal ) invMask[4] = { 1.0f/MASK_X, 1.0f/hkReal(MASK_Y), 1.0f/hkReal(MASK_Z), 0.0f };

	hkVector4	fractions; fractions.setMul( rv, (const hkVector4&)invMask );

	vectorOut->setMul( rv, fractions );
}

/// Encode a vertex.
template< typename T, int NUM_BITS_X, int NUM_BITS_Y >
HK_FORCE_INLINE void	hkVectorPackUtility::encode3(const hkAabb& aabb, hkVector4Parameter vertex, T* HK_RESTRICT output)
{
	enum
	{
		TOTAL_BITS	=	sizeof(T) << 3,
		NUM_BITS_Z	=	TOTAL_BITS - (NUM_BITS_X + NUM_BITS_Y),
		MASK_X		=	(1<<NUM_BITS_X)-1,
		MASK_Y		=	(1<<NUM_BITS_Y)-1,
		MASK_Z		=	(1<<NUM_BITS_Z)-1
	};

	hkVector4   clampled; clampled.setClamped(vertex, aabb.m_min, aabb.m_max);
	hkVector4	delta; delta.setSub(aabb.m_max, aabb.m_min);
	hkVector4	invDelta; invDelta.setReciprocal<HK_ACC_23_BIT,HK_DIV_IGNORE>(delta);
	HK_ASSERT( 0xf0345456, delta.greaterZero().allAreSet<hkVector4Comparison::MASK_XYZ>());	// check for valid aabb
	hkVector4	offset; offset.setSub(vertex, aabb.m_min);
	hkVector4	fractions; fractions.setMul(offset, invDelta);

	HK_ALIGN16( hkReal ) invMask[4] = { 1.0f/MASK_X, 1.0f/hkReal(MASK_Y), 1.0f/hkReal(MASK_Z), 0.0f };
	fractions.setAddMul( hkVector4::getConstant<HK_QUADREAL_INV_2>(), fractions, (const hkVector4&)invMask );

	hkIntVector i; i.setConvertF32toS32( fractions );

	const T		x = i.getU32(0);
	const T		y = i.getU32(1);
	const T		z = i.getU32(2);
	const T		p = (z << (NUM_BITS_X + NUM_BITS_Y)) | (y << NUM_BITS_Y) | x;
	output[0] = p;
}

#if defined HK_COMPILER_MSVC
//  warning C4723: potential divide by 0
#	pragma warning(push)
#	pragma warning(disable: 4723)
#endif


// If the bit set in n is also set in m, this function will return ts. Otherwise it will return 0.
// Note: the 'if' statement is a compile-time 'if' (not a runtime 'if').
HK_FORCE_INLINE hkUint32 filteredStoreToStream_calcOffset( hkUint32 n, const unsigned int ts, hkVector4Comparison::Mask m )
{
	if ( n > ts )
	{
		return (m&n) / (n/ts);
	}
	else
	{
		return (m&n) * (ts/n);
	}
}

HK_FORCE_INLINE hkReal*	hkVectorPackUtility::filteredStoreToStream( hkVector4Parameter vector, hkVector4Comparison::Mask m, hkReal* dest )
{
	const unsigned int ts = sizeof(hkInt32);
	{ const hkUint32 n=hkVector4Comparison::MASK_X;   vector.					store<1>(dest);	dest = hkAddByteOffset( dest, filteredStoreToStream_calcOffset( n, ts, m ) ); }
	{ const hkUint32 n=hkVector4Comparison::MASK_Y;   vector.getComponent<1>().	store<1>(dest); dest = hkAddByteOffset( dest, filteredStoreToStream_calcOffset( n, ts, m ) ); }
	{ const hkUint32 n=hkVector4Comparison::MASK_Z;   vector.getComponent<2>().	store<1>(dest); dest = hkAddByteOffset( dest, filteredStoreToStream_calcOffset( n, ts, m ) ); }
	{ const hkUint32 n=hkVector4Comparison::MASK_W;   vector.getComponent<3>().	store<1>(dest); dest = hkAddByteOffset( dest, filteredStoreToStream_calcOffset( n, ts, m ) ); }
	return dest;
}

#if defined HK_COMPILER_MSVC
//  warning C4723: potential divide by 0
#	pragma warning(pop)
#endif

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
