/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2012 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */

#ifndef HK_DYNAMICS2_ANGULAR_FRICTION_CONSTRAINT_H
#define HK_DYNAMICS2_ANGULAR_FRICTION_CONSTRAINT_H

#include <Physics/Dynamics/Constraint/hkpConstraintData.h>
#include <Physics/ConstraintSolver/Constraint/Atom/hkpConstraintAtom.h>


/// The ragdoll constraint. You can use this constraint to realistically
/// simulate the behavior of body joints, such as hips, shoulders, and knees.
/// Ragdoll constraint allows you to activate motors to drive the 3 angles. If
/// you are using a motor with a target position to set, set it to 0. During
/// the hkpConstraintMotor::motor() callback the difference between the current
/// angle the desired angle will be passed in. This will result in your motor
/// driving the difference in the angle to zero. If the motor is enabled, all
/// friction will be disabled, however once you disable the motor, friction
/// will be enabled again.
class hkpAngularFrictionConstraintData : public hkpConstraintData
{
public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);

	hkpAngularFrictionConstraintData();

	/// Will dereference any motors held.
	~hkpAngularFrictionConstraintData();


	/// Setup a ragdoll constraint based on information in world space.
	/// Use the set angle limit methods to add in the extra data.
	/// Use the set motors and set angle limit methods to add in the extra data.
	/// Will set the target frame B to match the current setup frame for B
	/// \param bodyA			The attached body transform
	/// \param bodyB			The reference body transform
	/// \param pivot			The pivot point, specified in world space.
	/// \param twistAxisW		The twist axis, specified in world space.
	/// \param planeAxisW		The plane axis, specified in world space.
	void setInWorldSpace(const hkTransform& bodyATransform, const hkTransform& bodyBTransform,
		const hkVector4& pivot, const hkVector4& twistAxisW,
		const hkVector4& planeAxisW);

	/// Setup a ragdoll constraint based on information in body space.
	/// Use the set angle limit methods to add in the extra data.
	/// Use the set motors and set angle limit methods to add in the extra data.
	/// Will set the target frame B to match the current setup frame for B
	/// \param pivotA			The pivot point, specified in bodyA space.
	/// \param pivotB			The pivot point, specified in bodyB space.
	/// \param twistAxisA		The twist axis, specified in bodyA space.
	/// \param twistAxisB		The twist axis, specified in bodyB space.
	/// \param planeAxisA		The plane axis, specified in bodyA space.
	/// \param planeAxisB		The plane axis, specified in bodyB space.
	void setInBodySpace( const hkVector4& pivotA, const hkVector4& pivotB,
		const hkVector4& planeAxisA, const hkVector4& planeAxisB,
		const hkVector4& twistAxisA, const hkVector4& twistAxisB);

	/// Gets the constraint frame.
	/// \param constraintFrameA Column 0 = twist axis, Column 1 = plane, Column 2 = twist cross plane.
	void getConstraintFrameA( hkMatrix3& constraintFrameA ) const;

	/// Gets the constraint frame.
	/// \param constraintFrameB Column 0 = twist axis, Column 1 = plane, Column 2 = twist cross plane.
	void getConstraintFrameB( hkMatrix3& constraintFrameB ) const;


	/// Sets the friction value. Set this before adding to the system.
	/// Note that this value is an absolute torque value and is therefore
	/// dependent on the masses of constrained bodies and not limited
	/// between 0.0f and 1.0f. If trying to stiffen up ragdoll constraints,
	/// try setting this value sufficiently high so that constraints are
	/// completely stiff and then reduce until the desired behavior has
	/// been achieved.
	void setMaxFrictionTorque(hkReal tmag);

	/// Gets the friction value.
	inline hkReal getMaxFrictionTorque() const;

	/// Choose the body to be notified when the constraint's maximum impulse is breached.
	virtual void setBodyToNotify(int bodyIdx);

	/// Returns the index of the body that is notified when the constraint's maximum impulse limit is breached.
	virtual hkUint8 getNotifiedBodyIndex() const;

	/// Sets the solving method for this constraint. Use one of the hkpConstraintAtom::SolvingMethod as a value for method.
	virtual void setSolvingMethod(hkpConstraintAtom::SolvingMethod method);

	/// Get type from this constraint
	virtual int getType() const;

	hkBool isValid() const;


	enum
	{
		SOLVER_RESULT_FRICTION_0 = 0,		// the angular friction value
		SOLVER_RESULT_FRICTION_1 = 1,		// the angular friction value
		SOLVER_RESULT_FRICTION_2 = 2,		// the angular friction value

		SOLVER_RESULT_MAX = 3 // keep in sync with solver res array below
	};

	struct Runtime
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpAngularFrictionConstraintData::Runtime );

		class hkpSolverResults m_solverResults[3/*VC6 doesn't like the scoping for SOLVER_RESULT_MAX*/];

		/// Tells whether the previous target angles have been initialized.
		/// This is zeroed when created.
		hkUint8 m_initialized[3];

		/// Target angles for the individual motors
		hkReal m_previousTargetAngles[3];

		/// Soft recovery parameter for the cone limit.
		hkReal m_coneAngleOffset;
	};


	static inline Runtime* HK_CALL getRuntime( hkpConstraintRuntime* runtime ) { return reinterpret_cast<Runtime*>(runtime); }

	/// hkpConstraintData interface implementations
	virtual void getConstraintInfo( hkpConstraintData::ConstraintInfo& infoOut ) const;

	/// hkpConstraintData interface implementation
	virtual void getRuntimeInfo( hkBool wantRuntime, hkpConstraintData::RuntimeInfo& infoOut ) const;

	struct Atoms
	{
		// +version(1)
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpAngularFrictionConstraintData::Atoms );

		struct hkpSetLocalTransformsConstraintAtom	m_transforms;
		struct hkpAngFrictionConstraintAtom			m_angFriction;

		enum Axis
		{
			AXIS_TWIST = 0,
			AXIS_PLANES = 1,
			AXIS_CROSS_PRODUCT = 2
		};

		Atoms(){}

		/// Get a pointer to the first atom
		const hkpConstraintAtom* getAtoms() const { return &m_transforms; }

		/// Get the size of all atoms
		/// Note: we can't use sizeof(*this) because of align16 padding.
		int getSizeOfAllAtoms() const               { return hkGetByteOffsetInt(this, &m_angFriction + 1); }

		Atoms(hkFinishLoadedObjectFlag f)
			:	m_transforms(f)
			,	m_angFriction(f)
		{}
	};

	HK_ALIGN_REAL( struct Atoms m_atoms );
};

#endif	// HK_DYNAMICS2_ANGULAR_FRICTION_CONSTRAINT_H

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
