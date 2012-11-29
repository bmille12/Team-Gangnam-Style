/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2012 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */


HK_FORCE_INLINE hkTime hkSweptTransform::getBaseTime() const
{
	return hkTime(m_centerOfMass0(3));
}

HK_FORCE_INLINE hkSimdReal hkSweptTransform::getBaseTimeSr() const
{
	return m_centerOfMass0.getComponent<3>();
}

HK_FORCE_INLINE hkReal hkSweptTransform::getInvDeltaTime() const 
{
	return m_centerOfMass1(3);
}

HK_FORCE_INLINE hkSimdReal hkSweptTransform::getInvDeltaTimeSr() const 
{
	return m_centerOfMass1.getComponent<3>();
}

#define HK_SWEPT_TRANSFORM_ASSERT_STRING "Disable this assert if it gets fired from your user code. It is only used to check internal consistency."

HK_FORCE_INLINE hkReal hkSweptTransform::getInterpolationValue( hkTime t ) const
{
	HK_MATH_ASSERT(0xf0ff0080, getInvDeltaTime() == hkReal(0) || getInvDeltaTime() > hkReal(0.01f), "hkSweptTransforms time period (which corresponds to simulation-step deltaTime) is neither set to infinity (as for fixed/not-integrated motion) nor is less than 100 seconds.");
	hkReal dt = (t - getBaseTime()) * getInvDeltaTime();
	//HK_MATH_ASSERT(0xf0ff0080, hkMath::isFinite(dt), HK_SWEPT_TRANSFORM_ASSERT_STRING);
	HK_MATH_ASSERT(0xf0ff0080, dt >= hkReal(0),  HK_SWEPT_TRANSFORM_ASSERT_STRING);
	HK_MATH_ASSERT(0xf0ff0080, dt <= hkReal(1.01f), HK_SWEPT_TRANSFORM_ASSERT_STRING);
	return dt;
}

HK_FORCE_INLINE hkSimdReal hkSweptTransform::getInterpolationValue( hkSimdRealParameter t ) const
{
	HK_MATH_ASSERT(0xf0ff0080, getInvDeltaTime() == hkReal(0) || getInvDeltaTime() > hkReal(0.01f), "hkSweptTransforms time period (which corresponds to simulation-step deltaTime) is neither set to infinity (as for fixed/not-integrated motion) nor is less than 100 seconds.");
	hkSimdReal dt = (t - getBaseTimeSr()) * getInvDeltaTimeSr();
	//HK_MATH_ASSERT(0xf0ff0080, hkMath::isFinite(dt), HK_SWEPT_TRANSFORM_ASSERT_STRING);
	HK_MATH_ASSERT(0xf0ff0080, dt.isGreaterEqualZero(),  HK_SWEPT_TRANSFORM_ASSERT_STRING);
	HK_MATH_ASSERT(0xf0ff0080, dt.getReal() <= hkReal(1.01f), HK_SWEPT_TRANSFORM_ASSERT_STRING);
	return dt;
}

HK_FORCE_INLINE hkReal hkSweptTransform::getInterpolationValueHiAccuracy( hkTime t, hkReal deltaTimeAddon ) const
{
	HK_MATH_ASSERT(0xad67fa3a, getInvDeltaTime() == hkReal(0) || getInvDeltaTime() > hkReal(0.01f), "hkSweptTransforms time period (which corresponds to simulation-step deltaTime) is neither set to infinity (as for fixed/not-integrated motion) nor is less than 100 seconds.");
	hkReal dt = t - getBaseTime();
	dt += deltaTimeAddon;
	hkReal ddt = dt * getInvDeltaTime();
	//HK_MATH_ASSERT(0xad67fa3a, hkMath::isFinite(ddt), HK_SWEPT_TRANSFORM_ASSERT_STRING);
	HK_MATH_ASSERT(0xad67fa3a, ddt >= hkReal(0),  HK_SWEPT_TRANSFORM_ASSERT_STRING);
	HK_MATH_ASSERT(0xad67fa3a, ddt <= hkReal(1.01f), HK_SWEPT_TRANSFORM_ASSERT_STRING);
	return ddt;
}

HK_FORCE_INLINE hkSimdReal hkSweptTransform::getInterpolationValueHiAccuracy( hkSimdRealParameter t, hkSimdRealParameter deltaTimeAddon ) const
{
	HK_MATH_ASSERT(0xad67fa3a, getInvDeltaTime() == hkReal(0) || getInvDeltaTime() > hkReal(0.01f), "hkSweptTransforms time period (which corresponds to simulation-step deltaTime) is neither set to infinity (as for fixed/not-integrated motion) nor is less than 100 seconds.");
	const hkSimdReal dt = t - getBaseTimeSr() + deltaTimeAddon;
	const hkSimdReal ddt = dt * getInvDeltaTimeSr();
	//HK_MATH_ASSERT(0xad67fa3a, hkMath::isFinite(ddt), HK_SWEPT_TRANSFORM_ASSERT_STRING);
	HK_MATH_ASSERT(0xad67fa3a, ddt.isGreaterEqualZero(),  HK_SWEPT_TRANSFORM_ASSERT_STRING);
	HK_MATH_ASSERT(0xad67fa3a, ddt.getReal() <= hkReal(1.01f), HK_SWEPT_TRANSFORM_ASSERT_STRING);
	return ddt;
}

HK_FORCE_INLINE void hkSweptTransform::_approxTransformAt( hkTime t, hkTransform& transformOut ) const 
{
	_approxTransformAt(hkSimdReal::fromFloat(t),transformOut);
}

HK_FORCE_INLINE void hkSweptTransform::_approxTransformAt( hkSimdRealParameter t, hkTransform& transformOut ) const 
{
	const hkSimdReal dt = getInterpolationValue(t);

	hkQuaternion q;
	q.m_vec.setInterpolate( m_rotation0.m_vec, m_rotation1.m_vec, dt );
	q.normalize();

	transformOut.setRotation( q );
	transformOut.getTranslation().setInterpolate( m_centerOfMass0, m_centerOfMass1, dt);

	hkVector4 centerShift;
	centerShift._setRotatedDir( transformOut.getRotation(), m_centerOfMassLocal);
	transformOut.getTranslation().sub( centerShift );
}

#undef HK_SWEPT_TRANSFORM_ASSERT_STRING

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
