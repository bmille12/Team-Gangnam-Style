/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2012 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */

#ifndef HK_MATH_MATRIX3_UTIL_H
#define HK_MATH_MATRIX3_UTIL_H

#include <Common/Base/Math/Vector/hkVector4Util.h>

/// Utility functions for hkMatrix3
namespace hkMatrix3Util
{
	/// Sets reflectionOut to be the reflection of matrix D through matrix P, i.e., reflectionOut = Transpose(P) * A * P.
	/// Matrix D is assumed diagonal.
	HK_FORCE_INLINE void HK_CALL _computeReflectedDiagonal(const hkMatrix3& P, hkVector4Parameter D, hkMatrix3& reflectionOut);
	void HK_CALL computeReflectedDiagonal(const hkMatrix3& P, hkVector4Parameter D, hkMatrix3& reflectionOut);

	/// Sets matrixOut = Q * D * Inverse(Q), where D is interpreted as a diagonal matrix.
	HK_FORCE_INLINE void HK_CALL _changeBasis(hkQuaternionParameter Q, hkVector4Parameter D, hkMatrix3& matrixOut);
	void HK_CALL changeBasis(hkQuaternionParameter Q, hkVector4Parameter D, hkMatrix3& matrixOut);

	/// Sets matrixOut = R * D * Transpose(R), where D is interpreted as a diagonal matrix.
	HK_FORCE_INLINE void HK_CALL _changeBasis(const hkRotation& R, hkVector4Parameter D, hkMatrix3& matrixOut);
	void HK_CALL changeBasis(const hkRotation& R, hkVector4Parameter D, hkMatrix3& matrixOut);

	/// Sets inertiaOut = Identity * dot(v, v) - OuterProduct(v, v). The matrix is used to displace inertia tensors
	/// by v from the center of mass.
	HK_FORCE_INLINE void HK_CALL _computeInertiaDisplacement(hkVector4Parameter v, hkMatrix3& inertiaOut);
	void HK_CALL computeInertiaDisplacement(hkVector4Parameter v, hkMatrix3& inertiaOut);

	/// Returns the diagonal of the given matrix.
	HK_FORCE_INLINE	void HK_CALL _getDiagonal(const hkMatrix3& matrixIn, hkVector4& diagonalOut);
	void HK_CALL getDiagonal(const hkMatrix3& matrixIn, hkVector4& diagonalOut);

	/// Returns the off-diagonal elements of the given matrix. The matrix is assumed to be symmetric. The returned vector is
	/// [m01, m02, m12]
	HK_FORCE_INLINE void HK_CALL _getSymmetricOffDiagonal(const hkMatrix3& matrixIn, hkVector4& offDiagonalOut);
	void HK_CALL getSymmetricOffDiagonal(const hkMatrix3& matrixIn, hkVector4& offDiagonalOut);

	/// Returns the maximum element
	HK_FORCE_INLINE const hkSimdReal HK_CALL _computeMaxAbsElement(const hkMatrix3& matrixIn);
	const hkSimdReal HK_CALL computeMaxAbsElement(const hkMatrix3& matrixIn);

	/// Computes the coefficients of the characteristic polynomial
	HK_FORCE_INLINE void HK_CALL _computeCharacteristicPolynomial(const hkMatrix3& matrixIn, hkVector4& coeffsOut);
	void HK_CALL computeCharacteristicPolynomial(const hkMatrix3& matrixIn, hkVector4& coeffsOut);

	/// Computes the eigenvalues analytically, using Cardano's formula. The matrix is assumed to be symmetric
	HK_FORCE_INLINE void HK_CALL _computeSymmetricEigenValues(const hkMatrix3& matrixIn, hkVector4& eigenVals);
	void HK_CALL computeSymmetricEigenValues(const hkMatrix3& matrixIn, hkVector4& eigenVals);

	/// Diagonalizes a symmetric matrix using a hybrid approach. It first computes the eigenvalues with Cardano's cubic equation
	/// roots and if the error is too big, it defaults to diagonalizeSymmetricApproximation().
	hkResult HK_CALL diagonalizeSymmetricHybrid(const hkMatrix3& matrixIn, hkRotation& eigenVec, hkVector4& eigenVal, int maxIter = 10);

	/// Computes matrixOut = matrixIn * hat(v), whereh hat(v) is the skew-symmetric matrix of v
	HK_FORCE_INLINE void HK_CALL _computeCrossProduct(const hkMatrix3& matrixIn, hkVector4Parameter v, hkMatrix3& matrixOut);

	/// Sets aTb = Transpose(a) * b.
	HK_FORCE_INLINE void HK_CALL _computeTransposeMul(const hkMatrix3& a, const hkMatrix3& b, hkMatrix3& aTb);

	/// Sets this matrix as the cross product between the given vector and each of the given matrix's columns.
	/// Specifically, it sets mtxOut.getColumn(i) = cross(v, src.getColumn(i))
	HK_FORCE_INLINE void HK_CALL _computeCrossMul(hkVector4Parameter v, const hkMatrix3& src, hkMatrix3& mtxOut);

	/// Checks if matrixIn is equal to the identity matrix within an epsilon.
	HK_FORCE_INLINE hkBool32 HK_CALL _isApproximatelyIdentity(const hkMatrix3& matrixIn, hkSimdRealParameter eps);

	/// Sets the specified diagonal values, zeroes the non-diagonal values.
	HK_FORCE_INLINE void HK_CALL _setDiagonal(hkReal m00, hkReal m11, hkReal m22, hkMatrix3& matrixOut);
	HK_FORCE_INLINE void HK_CALL _setDiagonal(hkSimdRealParameter m00, hkSimdRealParameter m11, hkSimdRealParameter m22, hkMatrix3& matrixOut);
	HK_FORCE_INLINE void HK_CALL _setDiagonal(hkVector4Parameter vDiagonal, hkMatrix3& matrixOut);
	HK_FORCE_INLINE void HK_CALL _setDiagonal(hkSimdRealParameter diag, hkMatrix3& matrixOut);

	/// Sets the specified diagonal values, leaves the non-diagonal values alone.
	HK_FORCE_INLINE void HK_CALL _setDiagonalOnly(hkVector4Parameter vDiagonal, hkMatrix3& matrixOut);
	HK_FORCE_INLINE void HK_CALL _setDiagonalOnly(hkSimdRealParameter diag, hkMatrix3& matrixOut);

	/// Computes mtxOut to be the product of a and the inverse of b. (mtxOut = a * b^-1)
	HK_FORCE_INLINE void HK_CALL _computeMulInverse(const hkMatrix3& a, const hkRotation& b, hkMatrix3& mtxOut);
}

#include <Common/Base/Math/Matrix/hkMatrix3Util.inl>

#endif	//	HK_MATH_MATRIX3_UTIL_H

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
