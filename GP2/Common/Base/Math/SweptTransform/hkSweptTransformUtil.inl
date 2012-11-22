/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2012 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */




HK_FORCE_INLINE void HK_CALL hkSweptTransformUtil::_lerp2( const hkSweptTransform& sweptTrans, hkReal t, hkQuaternion& quatOut )
{
	_lerp2(sweptTrans, hkSimdReal::fromFloat(t), quatOut);
}

HK_FORCE_INLINE void HK_CALL hkSweptTransformUtil::_lerp2( const hkSweptTransform& sweptTrans, hkSimdRealParameter st, hkQuaternion& quatOut )
{
	hkVector4 rot0 = sweptTrans.m_rotation0.m_vec;
	hkVector4 rot1 = sweptTrans.m_rotation1.m_vec;
	const hkSimdReal half = hkSimdReal_Inv2;
	const hkSimdReal two = hkSimdReal_2;
	const hkSimdReal one = hkSimdReal_1;
	{
		quatOut.m_vec.setAdd( rot0, rot1 );
		const hkSimdReal len2 = quatOut.m_vec.lengthSquared<4>();
		hkSimdReal threequarter; threequarter.setFromFloat(0.75f);
		hkSimdReal oneeighth; oneeighth.setFromFloat(0.125f);
		hkSimdReal r = threequarter - oneeighth * len2;
		r = r * ((one + half) - half * len2 * r * r);
		quatOut.m_vec.mul(r);
	}

	if ( st < half )
	{
		quatOut.m_vec.setInterpolate( rot0, quatOut.m_vec, st * two);
	}
	else
	{
		quatOut.m_vec.setInterpolate( quatOut.m_vec, rot1, (two * st) - one);
	}
	quatOut.normalize();
}


HK_FORCE_INLINE void HK_CALL hkSweptTransformUtil::_lerp2( const hkSweptTransform& sweptTrans, hkReal t, hkTransform& transformOut )
{
	_lerp2(sweptTrans, hkSimdReal::fromFloat(t), transformOut);
}

HK_FORCE_INLINE void HK_CALL hkSweptTransformUtil::_lerp2( const hkSweptTransform& sweptTrans, hkSimdRealParameter t, hkTransform& transformOut )
{
	hkQuaternion qt;
	_lerp2( sweptTrans, t, qt );

	transformOut.setRotation( qt );
	transformOut.getTranslation().setInterpolate( sweptTrans.m_centerOfMass0, sweptTrans.m_centerOfMass1, t);

	hkVector4 centerShift;
	centerShift._setRotatedDir( transformOut.getRotation(), sweptTrans.m_centerOfMassLocal);
	transformOut.getTranslation().sub( centerShift );
}


HK_FORCE_INLINE void HK_CALL hkSweptTransformUtil::calcTransAtT0( const hkSweptTransform& sweptTrans, hkTransform& transformOut )
{
	const hkQuaternion& qt = sweptTrans.m_rotation0;

	transformOut.setRotation( qt );

	hkVector4 centerShift;
	centerShift._setRotatedDir( transformOut.getRotation(), sweptTrans.m_centerOfMassLocal);
	transformOut.getTranslation().setSub( sweptTrans.m_centerOfMass0, centerShift );
}


HK_FORCE_INLINE void HK_CALL hkSweptTransformUtil::calcTransAtT1( const hkSweptTransform& sweptTrans, hkTransform& transformOut )
{
	const hkQuaternion& qt = sweptTrans.m_rotation1;

	transformOut.setRotation( qt );

	hkVector4 centerShift;
	centerShift._setRotatedDir( transformOut.getRotation(), sweptTrans.m_centerOfMassLocal);
	transformOut.getTranslation().setSub( sweptTrans.m_centerOfMass1, centerShift );
}

HK_FORCE_INLINE void HK_CALL hkSweptTransformUtil::_clipVelocities( const hkMotionState& motionState, hkVector4& linearVelocity, hkVector4& angularVelocity )
{
	const hkSimdReal linVelSq = linearVelocity.lengthSquared<3>();
	const hkSimdReal angVelSq = angularVelocity.lengthSquared<3>();
	hkSimdReal maxLinear; maxLinear = hkSimdReal::fromFloat(hkReal(motionState.m_maxLinearVelocity));
	const hkSimdReal maxLinear2 = maxLinear * maxLinear;
	hkSimdReal maxAngularRadians; maxAngularRadians = hkSimdReal::fromFloat(hkReal(motionState.m_maxAngularVelocity)); 
	const hkSimdReal maxAngularRadians2 = maxAngularRadians * maxAngularRadians;

	if ( (linVelSq > maxLinear2) || (linVelSq!=linVelSq) )
	{
		const hkSimdReal f = maxLinear * linVelSq.sqrtInverse();
		linearVelocity.mul( f );
		if ( linVelSq!=linVelSq )
		{
			linearVelocity = hkVector4::getConstant<HK_QUADREAL_1000>();
			HK_ASSERT2(0xf0124242, false, "NaN velocity detected, something is seriously wrong (bad inertia tensors?)");
		}
	}
	if ((angVelSq > maxAngularRadians2) || ( angVelSq != angVelSq ))
	{
		const hkSimdReal f = maxAngularRadians * angVelSq.sqrtInverse();
		angularVelocity.mul( f );
		if ( angVelSq!=angVelSq )
		{
			angularVelocity = hkVector4::getConstant<HK_QUADREAL_1000>();
			HK_ASSERT2(0xf0143243, false, "NaN velocity detected, something is seriously wrong (bad inertia tensors?)");
		}
	}
}


HK_FORCE_INLINE void HK_CALL hkSweptTransformUtil::_stepMotionState( const hkStepInfo& info,
											 hkVector4& linearVelocity, hkVector4& angularVelocity,
											 hkMotionState& motionState )
{
	hkSimdReal	deltaTime; deltaTime.setFromFloat( info.m_deltaTime.val() * motionState.m_timeFactor );
#ifdef HK_DEBUG
	{
		if ( motionState.getSweptTransform().getInvDeltaTime() != hkReal(0))
		{
			hkReal motionEndTime = motionState.getSweptTransform().getBaseTime() + hkReal(1) / motionState.getSweptTransform().getInvDeltaTime();
			HK_ASSERT(0xf0f0083, hkMath::equal(info.m_startTime, motionEndTime ) ) ;
		}
	}
#endif
	// check for nans in velocities
	// fix nans to (1,0,0)
	{
		hkVector4 absLin; absLin.setAbs( linearVelocity );
		hkVector4 absAng; absAng.setAbs( angularVelocity );

		hkVector4 maxVel; maxVel.m_quad = _stepMotionStateMaxVelf;

		hkVector4Comparison isNormal = absLin.less( maxVel );
		isNormal.setAnd( isNormal, absAng.less( maxVel ) );
		isNormal.setAnd( isNormal, linearVelocity.equal( linearVelocity ) );
		isNormal.setAnd( isNormal, angularVelocity.equal( angularVelocity ) );

		if ( !isNormal.allAreSet<hkVector4Comparison::MASK_XYZ>() )
		{
			// velocity to a 'random' non zero velocity
			const hkVector4 id0 = hkVector4::getConstant<HK_QUADREAL_1000>();
			linearVelocity  = id0;
			angularVelocity = id0;
		}
	}

	{
		hkSimdReal startT; startT.setFromFloat(info.m_startTime.val());
		motionState.getSweptTransform().m_centerOfMass0.setXYZ_W( motionState.getSweptTransform().m_centerOfMass1, startT );
	}

	{
		hkSimdReal stateMaxLin; stateMaxLin.setFromFloat(motionState.m_maxLinearVelocity);
		const hkSimdReal linVelSq = linearVelocity.lengthSquared<3>();
		if ( linVelSq > stateMaxLin * stateMaxLin )
		{
			//HK_WARN_ONCE(0xf0327683, "Object exceeding maximum velocity, velocity clipped" );
			const hkSimdReal f = stateMaxLin * linVelSq.sqrtInverse<HK_ACC_23_BIT,HK_SQRT_IGNORE>();
			linearVelocity.mul( f );
		}
	}

	motionState.getSweptTransform().m_centerOfMass1.addMul(deltaTime, linearVelocity);
	motionState.getSweptTransform().m_centerOfMass1(3) = info.m_invDeltaTime.val();

	hkQuaternion newRotation = motionState.getSweptTransform().m_rotation1;
	motionState.getSweptTransform().m_rotation0 = newRotation;

	//
	//	Calculate a new rotation, the fabs angle and angle squared
	//
	{
		hkQuaternion rotationRadians;	rotationRadians.m_vec.setMul( deltaTime * hkSimdReal_Inv2 , angularVelocity );
		hkSimdReal numHalfRotations2 = rotationRadians.m_vec.lengthSquared<3>() * hkSimdReal::fromFloat(hkReal(4) / (HK_REAL_PI*HK_REAL_PI));

		
		const hkSimdReal a = hkSimdReal::fromFloat(0.822948f);
		const hkSimdReal b = hkSimdReal::fromFloat(0.130529f);
		const hkSimdReal c = hkSimdReal::fromFloat(0.044408f);

		hkSimdReal maxAngleHalfRotations; maxAngleHalfRotations.setMin( hkSimdReal::fromFloat(0.9f), ( deltaTime * hkSimdReal::fromFloat(hkReal(motionState.m_maxAngularVelocity)) ) / hkSimdReal::getConstant<HK_QUADREAL_PI>() );

		// clipping angular velocity to be between [0, PI*0.9/dt]
		// info: was "<", is "<=" -- works ok for zero dt now.
		if ( numHalfRotations2 <= maxAngleHalfRotations * maxAngleHalfRotations )
		{
			const hkSimdReal numHalfRotations4 = numHalfRotations2 * numHalfRotations2;
			const hkSimdReal w = hkSimdReal::getConstant<HK_QUADREAL_1>() - a * numHalfRotations2 - b * numHalfRotations4 - c * numHalfRotations2 * numHalfRotations4;
			rotationRadians.m_vec.setW(w);
		}
		else
		{
			const hkSimdReal factor = maxAngleHalfRotations * numHalfRotations2.sqrtInverse();
			angularVelocity.mul( factor );
			rotationRadians.m_vec.mul( factor );

			numHalfRotations2  = maxAngleHalfRotations * maxAngleHalfRotations;
			const hkSimdReal numHalfRotations4  = numHalfRotations2 * numHalfRotations2;
			const hkSimdReal w = hkSimdReal::getConstant<HK_QUADREAL_1>() - a * numHalfRotations2 - b * numHalfRotations4 - c * numHalfRotations2 * numHalfRotations4;
			rotationRadians.m_vec.setW(w);
		}

		newRotation.setMul( rotationRadians, newRotation );
		newRotation.normalize();

		motionState.m_deltaAngle.setAdd( rotationRadians.m_vec, rotationRadians.m_vec );
		const hkSimdReal angle = numHalfRotations2.sqrt() * hkSimdReal::getConstant<HK_QUADREAL_PI>();
		motionState.m_deltaAngle.setW(angle);
	}
	motionState.getSweptTransform().m_rotation1 = newRotation;
	calcTransAtT1( motionState.getSweptTransform(), motionState.getTransform());
}







HK_FORCE_INLINE void HK_CALL hkSweptTransformUtil::deactivate( hkMotionState& ms )
{
	hkSweptTransform& sweptTransform = ms.getSweptTransform();
	ms.m_deltaAngle.setZero();

	sweptTransform.m_rotation0 = sweptTransform.m_rotation1;
	sweptTransform.m_centerOfMass0 = sweptTransform.m_centerOfMass1;
	sweptTransform.m_centerOfMass1.zeroComponent<3>();
}



//HK_FORCE_INLINE void HK_CALL hkSweptTransformUtil::calcTimInfo( const hkMotionState& ms0, const hkMotionState& ms1, hkVector4& timOut)
//{
//	HK_ASSERT2(0xad44d321, st0.getInvDeltaTime() == st1.getInvDeltaTime(), "Internal error: hkSweptTransform's must correspond to the same deltaTime in order to use void HK_CALL hkSweptTransformUtil::calcTimInfo( const hkMotionState& ms0, const hkMotionState& ms1, hkVector4& timOut)");
//
//	const hkSweptTransform& st0 = ms0.getSweptTransform();
//	const hkSweptTransform& st1 = ms1.getSweptTransform();
//
//	hkVector4 diff0; diff0.setSub( st0.m_centerOfMass0, st0.m_centerOfMass1 );
//	hkVector4 diff1; diff1.setSub( st1.m_centerOfMass1, st1.m_centerOfMass0 );
//
//	timOut.setAdd( diff0, diff1 );
//
//	timOut(3) = ms0.m_deltaAngle(3) * ms0.m_objectRadius + ms1.m_deltaAngle(3) * ms1.m_objectRadius;
//
//}

HK_FORCE_INLINE void HK_CALL hkSweptTransformUtil::calcTimInfo( const hkMotionState& ms0, const hkMotionState& ms1, hkReal deltaTime, hkVector4& timOut)
{
	const hkSweptTransform& st0 = ms0.getSweptTransform();
	const hkSweptTransform& st1 = ms1.getSweptTransform();

	hkVector4 diff0; diff0.setSub( st0.m_centerOfMass0, st0.m_centerOfMass1 );
	hkVector4 diff1; diff1.setSub( st1.m_centerOfMass1, st1.m_centerOfMass0 );

	hkSimdReal dt; dt.setFromFloat(deltaTime);
	const hkSimdReal f0 = dt * st0.getInvDeltaTimeSr();
	const hkSimdReal f1 = dt * st1.getInvDeltaTimeSr();

	HK_ASSERT2(0xad56daaa, f0.getReal() <= hkReal(1.01f) && f1.getReal() <= hkReal(1.01f), "Internal error: input for TIM calculation may be corrupted.");

	timOut.setMul( f0, diff0 );
	timOut.addMul( f1, diff1 );
	timOut.setW(f0 * ms0.m_deltaAngle.getComponent<3>() * hkSimdReal::fromFloat(ms0.m_objectRadius) +
				f1 * ms1.m_deltaAngle.getComponent<3>() * hkSimdReal::fromFloat(ms1.m_objectRadius) );

	// we don't project angular velocity just to keep it simple ( no cross products)
}

	// Calculates angular distance (angVelocity * dt) travelled by the bodies. 
HK_FORCE_INLINE void HK_CALL hkSweptTransformUtil::calcAngularTimInfo( const hkMotionState& ms0, const hkMotionState& ms1, hkReal deltaTime, hkVector4* HK_RESTRICT deltaAngleOut0, hkVector4* HK_RESTRICT deltaAngleOut1)
{
	const hkSweptTransform& st0 = ms0.getSweptTransform();
	const hkSweptTransform& st1 = ms1.getSweptTransform();
	hkSimdReal dt; dt.setFromFloat(deltaTime);
	const hkSimdReal f0 = dt * st0.getInvDeltaTimeSr();
	const hkSimdReal f1 = dt * st1.getInvDeltaTimeSr();

	hkVector4 ang0; ang0.setMul( f0, ms0.m_deltaAngle );
	hkVector4 ang1; ang1.setMul( f1, ms1.m_deltaAngle );

	deltaAngleOut0[0] = ang0;
	deltaAngleOut1[0] = ang1;
}

HK_FORCE_INLINE void HK_CALL hkSweptTransformUtil::calcCenterOfMassAt( const hkMotionState& ms, hkSimdRealParameter t, hkVector4& centerOut )
{
	const hkSimdReal iv = ms.getSweptTransform().getInterpolationValue(t);
	centerOut.setInterpolate( ms.getSweptTransform().m_centerOfMass0, ms.getSweptTransform().m_centerOfMass1, iv );
}

HK_FORCE_INLINE void HK_CALL hkSweptTransformUtil::calcCenterOfMassAt( const hkMotionState& ms, hkTime t, hkVector4& centerOut )
{
	calcCenterOfMassAt(ms, hkSimdReal::fromFloat(t), centerOut);
}

HK_FORCE_INLINE void HK_CALL hkSweptTransformUtil::getVelocity( const hkMotionState& ms, hkVector4& linearVelOut, hkVector4& angularVelOut )
{
	linearVelOut.setSub (ms.getSweptTransform().m_centerOfMass1, ms.getSweptTransform().m_centerOfMass0);
	const hkSimdReal idt = ms.getSweptTransform().getInvDeltaTimeSr();
	linearVelOut.mul( idt );
	angularVelOut.setMul( idt, ms.m_deltaAngle );
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
