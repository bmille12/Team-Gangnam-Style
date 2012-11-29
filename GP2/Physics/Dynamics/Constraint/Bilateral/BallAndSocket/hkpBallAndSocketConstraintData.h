/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2012 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */

#ifndef HKP_BALL_AND_SOCKET_CONSTRAINT_DATA_H
#define HKP_BALL_AND_SOCKET_CONSTRAINT_DATA_H

#include <Physics/Dynamics/Constraint/hkpConstraintData.h>
#include <Physics/ConstraintSolver/Constraint/Atom/hkpConstraintAtom.h>

extern const class hkClass hkpBallAndSocketConstraintDataAtomsClass;
extern const hkClass hkpBallAndSocketConstraintDataClass;



	/// The ball-and-socket or point-to-point constraint.
class hkpBallAndSocketConstraintData : public hkpConstraintData
{
	public:
	
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

		hkpBallAndSocketConstraintData();

			/// Sets the construction information with body space information.
			/// \param pivotA The constraint pivot point, specified in bodyA's space.
			/// \param pivotB The constraint pivot point, specified in bodyB's space.
		void setInBodySpace(const hkVector4& pivotA, const hkVector4& pivotB);

			/// Sets the construction information with world space information. Will use the
			/// given transforms to work out the two local pivots.
			/// \param bodyA The first rigid body transform
			/// \param bodyB The second rigid body transform
			/// \param pivot The constraint pivot point, specified in world space.
		void setInWorldSpace(const hkTransform& bodyATransform, const hkTransform& bodyBTransform, 
							const hkVector4& pivot);

			/// Sets the impulse above which this constraint is reported as breached.
			/// Set it to HK_REAL_MAX to disable breach reporting.
		virtual void setBreachImpulse(hkReal breachImpulse);

			/// Gets the impulse above which this constraint is reported as breached.
		virtual hkReal getBreachImpulse() const;

			/// Sets the maximum linear impulse the solver is allowed to apply to maintain this constraint.
			/// Call with HK_REAL_MAX to effectively disable the limit.
		virtual void setMaximumLinearImpulse(hkReal maxLinearImpulse);

			/// Gets the maximum impulse that can be applied by this constraint.
		virtual hkReal getMaximumLinearImpulse() const;

			/// Choose the body to be notified when the constraint's maximum impulse is breached.
		virtual void setBodyToNotify(int bodyIdx);

			/// Returns the index of the body that is notified when the constraint's maximum impulse is breached.
		virtual hkUint8 getNotifiedBodyIndex() const;

			/// Check consistency of constraint members.
		virtual hkBool isValid() const;

			/// Get type from this constraint.
		virtual int getType() const;

			/// Sets the solving method for this constraint. Use one of the hkpConstraintAtom::SolvingMethod as a value for method.
		virtual void setSolvingMethod(hkpConstraintAtom::SolvingMethod method);

			/// Gets the inertia stabilization factor, returns HK_FAILURE if the factor is not defined for the given constraint.
		virtual hkResult getInertiaStabilizationFactor(hkReal& inertiaStabilizationFactorOut) const;

			/// Sets the inertia stabilization factor, return HK_FAILURE if the factor is not defined for the given constraint.
		virtual hkResult setInertiaStabilizationFactor(const hkReal inertiaStabilizationFactorIn);
		
	public:
			
		//
		//	Solver interface
		//

		enum 
		{
			SOLVER_RESULT_LIN_0 = 0,		// linear constraint 
			SOLVER_RESULT_LIN_1 = 1,		// linear constraint 
			SOLVER_RESULT_LIN_2 = 2,		// linear constraint 
			SOLVER_RESULT_MAX = 3
		};

		struct Runtime
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpBallAndSocketConstraintData::Runtime );

			class hkpSolverResults m_solverResults[3/*VC6 doesn't like the scoping for SOLVER_RESULT_MAX*/];
		};

		inline const Runtime* getRuntime( hkpConstraintRuntime* runtime ) const { return reinterpret_cast<Runtime*>(runtime); }

	public:

		struct Atoms
		{
			// +version(1)
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpBallAndSocketConstraintData::Atoms );
			HK_DECLARE_REFLECTION();

			struct hkpSetLocalTranslationsConstraintAtom	m_pivots;
			struct hkpSetupStabilizationAtom				m_setupStabilization;
			struct hkpBallSocketConstraintAtom				m_ballSocket;

			Atoms()	{}

			// get a pointer to the first atom
			const hkpConstraintAtom* getAtoms() const { return &m_pivots; }

			int getSizeOfAllAtoms() const { return hkGetByteOffsetInt(this, &m_ballSocket+1); }

			Atoms(hkFinishLoadedObjectFlag f)
			:	m_pivots(f)
			,	m_setupStabilization(f)
			,	m_ballSocket(f)
			{}
		};

		HK_ALIGN_REAL( struct Atoms m_atoms );

	public:

		//
		// hkpConstraintData interface implementations
		//

		virtual void getConstraintInfo( hkpConstraintData::ConstraintInfo& infoOut ) const;

		virtual void getRuntimeInfo( hkBool wantRuntime, hkpConstraintData::RuntimeInfo& infoOut ) const;

	public:

		hkpBallAndSocketConstraintData(hkFinishLoadedObjectFlag f) : hkpConstraintData(f), m_atoms(f) {}
};


#endif // HKP_BALL_AND_SOCKET_CONSTRAINT_DATA_H

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
