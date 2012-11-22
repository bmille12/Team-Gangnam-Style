/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2012 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */

 
inline hkpConvexShape::hkpConvexShape(ShapeType type, hkReal radius) : hkpSphereRepShape( type )
{
	HK_ASSERT2(0x20b67de1, radius >= 0, "hkpConvexShape should not have a negative radius");
	m_radius = radius;
}

inline const hkReal& hkpConvexShape::getRadius() const
{
	return m_radius;
}

inline void hkpConvexShape::setRadiusUnchecked(hkReal radius)
{
	HK_ASSERT2(0x20b67de1, radius >= 0, "hkpConvexShape should not have a negative radius");
	m_radius = radius;
}

inline void hkpConvexShape::setRadius(hkReal radius)
{
	HK_WARN_ON_DEBUG_IF(getType()==hkcdShapeType::CONVEX_VERTICES,0xCF7E3E10,"Changing convex radius of that shape after creation can generate inconsistent collisions");
	setRadiusUnchecked(radius);
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
