/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2012 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */

//
//	Sets resultOut to be the product of t1 by the inverse of t2. (resultOut = t1 * Inverse(t2))

HK_FORCE_INLINE void HK_CALL hkTransformUtil::_computeMulInverse(const hkTransform& t1, const hkTransform& t2, hkTransform& resultOut)
{
	hkRotation rot;		hkMatrix3Util::_computeMulInverse(t1.getRotation(), t2.getRotation(), rot);
	hkVector4 pos;		pos._setRotatedDir(rot, t2.getTranslation());
						pos.setSub(t1.getTranslation(), pos);
	resultOut.set(rot, pos);
}

//
//	Converts a (hkQuaternion, hkVector4) pair to a hkTransform

HK_FORCE_INLINE void HK_CALL hkTransformUtil::_convert(hkQuaternionParameter fromRot, hkVector4Parameter fromPos, hkTransform& to)
{
	hkRotation rot;
	hkVector4Util::convertQuaternionToRotation(fromRot, rot);
	to.set(rot, fromPos);
}

//
//	Converts a hkQTransform to a hkTransform

HK_FORCE_INLINE void HK_CALL hkTransformUtil::_convert(const hkQTransform& from, hkTransform& to)
{
	hkTransformUtil::_convert(from.m_rotation, from.m_translation, to);
}

//
//	Sets this transform to be the product of t1 and t2. (this = t1 * t2)

HK_FORCE_INLINE void hkTransformUtil::_mulTransformTransform( const hkTransform& aTb, const hkTransform& bTc, hkTransform* HK_RESTRICT tOut )
{
	HK_ASSERT(0x4763da71,  tOut != &aTb );

	hkVector4Util::rotatePoints( aTb.getRotation(), &bTc.getRotation().getColumn<0>(), 4, &tOut->getRotation().getColumn<0>() );
	tOut->getTranslation().add( aTb.getTranslation());
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
