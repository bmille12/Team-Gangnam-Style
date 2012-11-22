/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2012 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */

#ifndef HK_DYNAMICS2_DEFORMABLE_FIXED_CONSTRAINT_H
#define HK_DYNAMICS2_DEFORMABLE_FIXED_CONSTRAINT_H

#include <Physics/Dynamics/Constraint/hkpConstraintData.h>
#include <Physics/ConstraintSolver/Constraint/Atom/hkpConstraintAtom.h>


/// A fixed constraint that allows deformation.
class hkpDeformableFixedConstraintData : public hkpConstraintData
{
	public:
		
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

		/// Constructor
		hkpDeformableFixedConstraintData();

		/// Sets the construction information with body space information.
		/// \param pivotA The constraint pivot, specified in bodyA's space.
		/// \param pivotB The constraint pivot, specified in bodyB's space.
		void setInBodySpace(const hkTransform& pivotA, const hkTransform& pivotB);

		/// Sets the construction information with world space information. Will use the
		/// given transforms to work out the two local pivots.
		/// \param bodyA The first rigid body transform
		/// \param bodyB The second rigid body transform
		/// \param pivot The constraint pivot, specified in world space.
		void setInWorldSpace(const hkTransform& bodyATransform, const hkTransform& bodyBTransform, const hkTransform& pivotA, const hkTransform& pivotB);

		/// Sets the maximum distance
		HK_FORCE_INLINE void setLinearLimit(hkReal maxDistance);

		/// Sets the linear offset
		HK_FORCE_INLINE void setLinearOffset(hkVector4Parameter vOffset);

		/// Sets the linear strength tensors
		HK_FORCE_INLINE void setLinearStrength(const hkSymmetricMatrix3& linearYieldStrength, const hkSymmetricMatrix3& linearUltimateStrength);

		/// Sets the maximum allowed angle
		HK_FORCE_INLINE void setAngularLimit(hkReal maxAngle);

		/// Sets the angular offset
		HK_FORCE_INLINE void setAngularOffset(hkQuaternionParameter qOffset);

		/// Sets the angular strength tensors
		HK_FORCE_INLINE void setAngularStrength(const hkSymmetricMatrix3& angularYieldStrength, const hkSymmetricMatrix3& angularUltimateStrength);

		/// Check consistency of constraint members.
		virtual hkBool isValid() const;

		/// Get type from this constraint.
		virtual int getType() const;

	public:

		enum 
		{
			SOLVER_RESULT_LIN_0 = 0,		// Linear constraint 0
			SOLVER_RESULT_LIN_1 = 1,		// Linear constraint 1 
			SOLVER_RESULT_LIN_2 = 2,		// Linear constraint 2
			SOLVER_RESULT_ANG_0 = 3,		// Angular constraint 0
			SOLVER_RESULT_ANG_1 = 4,		// Angular constraint 1
			SOLVER_RESULT_ANG_2 = 5,		// Angular constraint 2
			SOLVER_RESULT_MAX = 6
		};

		struct Runtime
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DYNAMICS, hkpDeformableFixedConstraintData::Runtime);

			// Solver results must always be in the first position
			HK_ALIGN16( class hkpSolverResults m_solverResults[6/*VC6 doesn't like the scoping for SOLVER_RESULT_MAX*/] );
		};

		static inline Runtime* HK_CALL getRuntime( hkpConstraintRuntime* runtime ) { return reinterpret_cast<Runtime*>(runtime); }

	public:

		struct Atoms
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DYNAMICS, hkpDeformableFixedConstraintData::Atoms);
			HK_DECLARE_REFLECTION();

			struct hkpSetLocalTransformsConstraintAtom		m_transforms;
			struct hkpDeformableLinConstraintAtom			m_lin;
			struct hkpDeformableAngConstraintAtom			m_ang;

			Atoms() {}

			// Get a pointer to the first atom
			const hkpConstraintAtom* getAtoms() const { return &m_transforms; }

			int getSizeOfAllAtoms() const { return hkGetByteOffsetInt(this, &m_ang+1); }

			Atoms(hkFinishLoadedObjectFlag f)
			:	m_transforms(f)
			,	m_lin(f)
			,	m_ang(f)
			{}
		};

		HK_ALIGN16( struct Atoms m_atoms );

	public:

		// hkpConstraintData interface implementations
		virtual void getConstraintInfo( hkpConstraintData::ConstraintInfo& infoOut ) const;

		// hkpConstraintData interface implementations
		virtual void getRuntimeInfo( hkBool wantRuntime, hkpConstraintData::RuntimeInfo& infoOut ) const;

	public:

		/// Serialization constructor
		hkpDeformableFixedConstraintData(hkFinishLoadedObjectFlag f)
		:	hkpConstraintData(f)
		,	m_atoms(f)
		{}
};

#include <Physics/Dynamics/Constraint/Bilateral/DeformableFixed/hkpDeformableFixedConstraintData.inl>

#endif	//	HK_DYNAMICS2_DEFORMABLE_FIXED_CONSTRAINT_H

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
