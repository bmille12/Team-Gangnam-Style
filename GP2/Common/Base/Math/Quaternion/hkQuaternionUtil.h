/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2012 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */

#ifndef HK_MATH_QUATERNION_UTIL_H
#define HK_MATH_QUATERNION_UTIL_H

#include <Common/Base/Math/Vector/hkVector4Util.h>

/// Utility functions for hkQuaternion
namespace hkQuaternionUtil
{
	/// Sets qOut to be the shortest rotation which brings 'from' to 'to'.
	/// NOTE: The vectors 'from ' and 'to' must be normalized.
	HK_FORCE_INLINE void HK_CALL _computeShortestRotation(hkVector4Parameter from, hkVector4Parameter to, hkQuaternion& qOut);

	/// Sets qOut to be the shortest rotation which brings 'from' to 'to'.
	/// NOTE: The vectors 'from ' and 'to' must be normalized.
	/// This version is damped and the result interpolates from 'from' to to' as gain goes from 0 to 1.
	/// This is similar to scaling the angle of rotation according to the gain.
	
	HK_FORCE_INLINE void HK_CALL _computeShortestRotationDamped(hkVector4Parameter from, hkVector4Parameter to, hkSimdRealParameter gain, hkQuaternion& qOut);

	/// Computes a 4x4 matrix that allows you to compute any product of the form (this * q) by multiplying (opOut * q).
	/// This is also equal to Transpose(getInversePostMultiplyOperator(this)).
	/// Also, the first 3x3 block is equal to the first 3x3 block of Transpose(getPreMultiplyOperator(this))
	HK_FORCE_INLINE void HK_CALL _computePostMultiplyOperator(hkQuaternionParameter q, hkMatrix4& opOut);

	/// Computes a 4x4 matrix that allows you to compute any product of the form (q * this) by multiplying (opOut * q).
	/// This is also equal to Transpose(getInversePreMultiplyOperator(this)).
	/// Also, the first 3x3 block is equal to the first 3x3 block of Transpose(getPostMultiplyOperator(this))
	HK_FORCE_INLINE void HK_CALL _computePreMultiplyOperator(hkQuaternionParameter q, hkMatrix4& opOut);

	/// Computes a 4x4 matrix that allows you to compute any product of the form (Inverse(this) * q) by multiplying (opOut * q).
	/// This is also equal to Transpose(getPostMultiplyOperator(this)).
	HK_FORCE_INLINE void HK_CALL _computeInversePostMultiplyOperator(hkQuaternionParameter q, hkMatrix4& opOut);

	/// Computes a 4x4 matrix that allows you to compute any product of the form (q * Inverse(this)) by multiplying (opOut * q).
	/// This is also equal to Transpose(getPreMultiplyOperator(this)).
	HK_FORCE_INLINE void HK_CALL _computeInversePreMultiplyOperator(hkQuaternionParameter q, hkMatrix4& opOut);

	/// Computes qOut = Transpose(op) * qIn, where op is a 4x4 matrix operator and qIn is a quaternion interpreted as a 4-element vector.
	HK_FORCE_INLINE void HK_CALL _computeTransposeMul(const hkMatrix4& op, hkQuaternionParameter qIn, hkQuaternion& qOut);

	///	Computes the logarithm of a quaternion
	HK_FORCE_INLINE void HK_CALL _computeLog(hkQuaternionParameter q, hkVector4& vOut);

	///	Computes the the exponential (quaternion) of the given (axis-angle) vector
	HK_FORCE_INLINE void HK_CALL _computeExp(hkVector4Parameter vIn, hkQuaternion& qOut);
	HK_FORCE_INLINE void HK_CALL _computeExp_Approximate(hkVector4Parameter vIn, hkQuaternion& qOut);

	/// N-lerp
	HK_FORCE_INLINE void HK_CALL _computeNlerp(hkQuaternionParameter q0, hkQuaternionParameter q1, hkSimdRealParameter t, hkQuaternion& qOut);
};

#include <Common/Base/Math/Quaternion/hkQuaternionUtil.inl>

#endif	//	HK_MATH_QUATERNION_UTIL_H

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
