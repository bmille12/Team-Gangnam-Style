/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2012 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */

#ifndef HKP_CONSTRAINT_DATA_H
#define HKP_CONSTRAINT_DATA_H

#include <Physics/ConstraintSolver/Solve/hkpSolverResults.h>
#include <Physics/ConstraintSolver/Solve/hkpSolverElemTemp.h>
#include <Physics/Dynamics/Constraint/hkpConstraintInfo.h>
#include <Physics/ConstraintSolver/Constraint/Atom/hkpSimpleConstraintAtom.h>

class hkpConstraintQueryIn;
class hkpConstraintQueryOut;
class hkpConstraintOwner;
class hkpConstraintInstance;

extern const hkClass hkpConstraintDataClass;
extern const hkClass hkpConstraintInfoClass;

// All runtime information of the constraints must start with an array of solver results
typedef void hkpConstraintRuntime;


/// The base class for constraints built from a set of constraint atoms. Provides an interface to obtain the
/// constraint atoms and their memory requirements. Defines the structure of the constraint's runtime data, if any,
/// and provides methods to access its data.
/// \sa hkpConstraintAtom
class hkpConstraintData : public hkReferencedObject
{
	public:

		/// Types of built-in constraints.
		enum ConstraintType
		{
			CONSTRAINT_TYPE_BALLANDSOCKET = 0,
			CONSTRAINT_TYPE_HINGE = 1,
			CONSTRAINT_TYPE_LIMITEDHINGE = 2,
			CONSTRAINT_TYPE_POINTTOPATH = 3,
			CONSTRAINT_TYPE_PRISMATIC = 6,
			CONSTRAINT_TYPE_RAGDOLL = 7,
			CONSTRAINT_TYPE_STIFFSPRING = 8,
			CONSTRAINT_TYPE_WHEEL = 9,
			CONSTRAINT_TYPE_GENERIC = 10,
			CONSTRAINT_TYPE_CONTACT = 11,
			CONSTRAINT_TYPE_BREAKABLE = 12,
			CONSTRAINT_TYPE_MALLEABLE = 13,
			CONSTRAINT_TYPE_POINTTOPLANE = 14,
			CONSTRAINT_TYPE_PULLEY = 15,
			CONSTRAINT_TYPE_ROTATIONAL = 16,
			CONSTRAINT_TYPE_HINGE_LIMITS = 18,
			CONSTRAINT_TYPE_RAGDOLL_LIMITS = 19,
			CONSTRAINT_TYPE_CUSTOM = 20,
			CONSTRAINT_TYPE_RACK_AND_PINION = 21,
			CONSTRAINT_TYPE_COG_WHEEL = 22,
			CONSTRAINT_TYPE_FIXED = 23,
			CONSTRAINT_TYPE_DEFORMABLE_FIXED = 24,
			
			// Constraint Chains
			BEGIN_CONSTRAINT_CHAIN_TYPES = 100,        
			CONSTRAINT_TYPE_STIFF_SPRING_CHAIN = 100,
			CONSTRAINT_TYPE_BALL_SOCKET_CHAIN = 101,
			CONSTRAINT_TYPE_POWERED_CHAIN = 102
		};

		/// Atoms structure information 
		struct ConstraintInfo : public hkpConstraintInfo
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpConstraintData::ConstraintInfo );
			HK_FORCE_INLINE ConstraintInfo() { m_extraSchemaSize = 0; }

			struct hkpConstraintAtom* m_atoms;
			hkUint32 m_sizeOfAllAtoms;
			hkUint32 m_extraSchemaSize;
		};

		/// Runtime data information
		struct RuntimeInfo
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpConstraintData::RuntimeInfo );

			int m_sizeOfExternalRuntime;
			int m_numSolverResults;
		};

	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT );
		HK_DECLARE_REFLECTION();

		/// Default constructor. You cannot instantiate this class directly - see derived classes.
		/// This constructor just does some debug checks for alignment.
		HK_FORCE_INLINE hkpConstraintData();

		/// Serialization constructor.
		hkpConstraintData(hkFinishLoadedObjectFlag f) : hkReferencedObject(f) {}

		/// Destructor.
		HK_FORCE_INLINE virtual ~hkpConstraintData();


		/// Get the type of constraint.
		virtual int getType() const = 0;

		/// Get the atoms structure of this constraint.
		virtual void getConstraintInfo( ConstraintInfo& infoOut ) const = 0;

		/// Check member variables of constraint for consistency. This is called by the constraint constructors.
		/// This method will be empty for some constraints.
		virtual hkBool isValid() const = 0;
		
		/// Set the user data.
		HK_FORCE_INLINE void setUserData( hkUlong data );

		/// Get the user data (initialized to zero).
		HK_FORCE_INLINE hkUlong getUserData() const;

		//
		// Impulse limiting interface
		//

		/// Sets the maximum linear impulse the solver is allowed to apply to maintain this constraint.
		/// Call with HK_REAL_MAX to effectively disable the limit.
		virtual void setMaximumLinearImpulse(hkReal maxLinearImpulse) { HK_WARN(0xad809031, "setMaximumLinearImpulse() called on a constraint that doesn't support it."); }

		/// Sets the maximum angular impulse the solver is allowed to apply to maintain this constraint.
		/// Call with HK_REAL_MAX to effectively disable the limit.
		virtual void setMaximumAngularImpulse(hkReal maxAngularImpulse) { HK_WARN(0xad809032, "setMaximumAngularImpulse() called on a constraint that doesn't support it."); }

		/// Sets the impulse above which the solver fires constraint-impulse-limit breached callbacks.
		/// Call with HK_REAL_MAX to effectively disable breach reporting.
		/// \sa hkpContactImpulseLimitBreachedListener
		virtual void setBreachImpulse(hkReal breachImpulse) { HK_WARN(0xad809033, "setBreachImpulse() called on a constraint that doesn't support it."); }

		/// Gets the maximum linear impulse that can be applied by this constraint.
		virtual hkReal getMaximumLinearImpulse() const { return HK_REAL_MAX; }

		/// Gets the maximum angular impulse that can be applied by this constraint.
		virtual hkReal getMaximumAngularImpulse() const { return HK_REAL_MAX; }
		
		/// Gets the impulse above which the solver fires constraint-impulse-limit breached callbacks.
		virtual hkReal getBreachImpulse() const { return HK_REAL_MAX; }

		/// Choose the body to be notified when the constraint's breach impulse limit is exceeded.
		/// Only one of the bodies can be notified. Typically you will want to notify the body with the lesser strength.
		/// 
		/// The callbacks are passed by the world to any attached hkpContactImpulseLimitBreachedListeners.
		/// The suggested way to handle those callbacks is to use a hkpBreakOffPartsUtil.
		/// hkpBreafOffPartsUtil converts breached constraints into the "breached contact point" representation
		/// so that they can be treated uniformly by the hkpBreafOffPartsListener. To handle the callbacks you should
		/// implement your own hkpBreakOffPartsListener and attach it to the hkpBreakOffPartsUtil.
		/// See this demo for reference: Demos/Physics/Api/Dynamics/RigidBodies/BreakOffParts/BreakOffPartsDemo
		virtual void setBodyToNotify(int bodyIdx) {}

		/// Returns the index of the body that is notified when the constraint's breach impulse is exceeded.
		virtual hkUint8 getNotifiedBodyIndex() const { HK_ASSERT2(0xad809034, false, "getNotifiedBodyIndex() called on a constaint that doesn't support max impulse."); return 0xff; }

		//
		// Stabilization interface
		//

		/// Sets the solving method for this constraint.
		virtual void setSolvingMethod( hkpConstraintAtom::SolvingMethod method ) {}

		/// Sets the inertia stabilization factor. Returns HK_FAILURE if the factor is not defined for this constraint.
		virtual hkResult setInertiaStabilizationFactor( const hkReal inertiaStabilizationFactorIn ) { return HK_FAILURE; }

		/// Gets the inertia stabilization factor. Returns HK_FAILURE if the factor is not defined for this constraint.
		virtual hkResult getInertiaStabilizationFactor( hkReal& inertiaStabilizationFactorOut ) const { return HK_FAILURE; }
		
		//
		// Runtime data interface
		//

		/// Get information about the size of the runtime data needed.
		/// Notes:
		/// * If wantRuntime is set, you ask for a runtime information.
		/// * Some constraints have their private runtime, so no need to externally allocate runtime info.
		/// * Some constraints might not want to use any runtime information.
		/// * All Runtimes should start with an array of hkpSolverResults.
		virtual void getRuntimeInfo( hkBool wantRuntime, RuntimeInfo& infoOut ) const = 0;

		/// Access to the solver results in a generic way. Use getRuntimeInfo to get the number of solver results.
		virtual hkpSolverResults* getSolverResults( hkpConstraintRuntime* runtime );

		/// Initialize the runtime data. The default implementation simply zeros all values.
		virtual void addInstance( hkpConstraintInstance* constraint, hkpConstraintRuntime* runtime, int sizeOfRuntime ) const;

		/// Drop an instance reference to this data.
		HK_FORCE_INLINE void removeInstance( hkpConstraintInstance* constraint, hkpConstraintRuntime* runtime, int sizeOfRuntime ) const;

	public:

		//
		// Internal functions
		//

		// Sets up the constraint for the solver.
		virtual void buildJacobian( const hkpConstraintQueryIn &in, hkpConstraintQueryOut &out ) { HK_ASSERT2(0xad567bbd, false, "Function deprecated"); }

		// Does the constraint need a single-threaded callback before Jacobians are built from atoms?
		virtual hkBool isBuildJacobianCallbackRequired() const { return false; }

		// The callback. Note that all callbacks are performed in single threaded mode.
		virtual void buildJacobianCallback( const hkpConstraintQueryIn &in, const hkpConstraintQueryOut& out ) { HK_ASSERT2(0xad567bbd, false, "This function must be overwritten, if the constraint requires a callback."); }
		
		static void HK_CALL getConstraintInfoUtil( const hkpConstraintAtom* atoms, int sizeOfAllAtoms, ConstraintInfo& infoOut );

	public:
		
		/// For any use you want. Not used by the engine.
		hkUlong m_userData; //+default(0)
};

#include <Physics/Dynamics/Constraint/hkpConstraintData.inl>

#endif // HKP_CONSTRAINT_DATA_H

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
