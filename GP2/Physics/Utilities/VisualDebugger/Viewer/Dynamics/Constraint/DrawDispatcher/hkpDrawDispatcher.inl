/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2012 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */

#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/Constraint/Drawer/hkpHingeDrawer.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/Constraint/Drawer/hkpLimitedHingeDrawer.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/Constraint/Drawer/hkpRagdollDrawer.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/Constraint/Drawer/hkpPrismaticDrawer.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/Constraint/Drawer/hkpWheelDrawer.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/Constraint/Drawer/hkpStiffSpringDrawer.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/Constraint/Drawer/hkpBallSocketDrawer.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/Constraint/Drawer/hkpPointToPathDrawer.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/Constraint/Drawer/hkpPointToPlaneDrawer.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/Constraint/Drawer/hkpPulleyDrawer.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/Constraint/Drawer/hkpHingeLimitsDrawer.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/Constraint/Drawer/hkpRagdollLimitsDrawer.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/Constraint/Drawer/hkpRackAndPinionDrawer.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/Constraint/Drawer/hkpCogWheelDrawer.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/Constraint/Drawer/hkpFixedConstraintDrawer.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/Constraint/Drawer/hkpDeformableFixedConstraintDrawer.h>

HK_FORCE_INLINE void hkpDispatchDraw(const hkpConstraintData* constraintData, const hkTransform& localToWorldA, const hkTransform& localToWorldB, hkDebugDisplayHandler* displayHandler, int id, int tag, hkReal scale )
{
	int type = constraintData->getType();

	switch(type)
	{
	case hkpConstraintData::CONSTRAINT_TYPE_BALLANDSOCKET:
		{
			hkpBallSocketDrawer drawer;
			drawer.setScale(scale);
			const hkpBallAndSocketConstraintData* data = static_cast<const hkpBallAndSocketConstraintData*>(constraintData);
			drawer.drawConstraint(data, localToWorldA, localToWorldB, displayHandler, id, tag);
		}
		break;

	case hkpConstraintData::CONSTRAINT_TYPE_FIXED:
		{
			hkpFixedConstraintDrawer drawer;
			drawer.setScale(scale);
			const hkpFixedConstraintData* data = static_cast<const hkpFixedConstraintData*>(constraintData);
			drawer.drawConstraint(data, localToWorldA, localToWorldB, displayHandler, id, tag);
		}
		break;

	case hkpConstraintData::CONSTRAINT_TYPE_DEFORMABLE_FIXED:
		{
			hkpDeformableFixedConstraintDrawer drawer;
			drawer.setScale(scale);
			const hkpDeformableFixedConstraintData* data = static_cast<const hkpDeformableFixedConstraintData*>(constraintData);
			drawer.drawConstraint(data, localToWorldA, localToWorldB, displayHandler, id, tag);
		}
		break;

	case hkpConstraintData::CONSTRAINT_TYPE_HINGE:
		{
			hkpHingeDrawer drawer;
			drawer.setScale(scale);
			const hkpHingeConstraintData* data = static_cast<const hkpHingeConstraintData*>(constraintData);
			drawer.drawConstraint(data, localToWorldA, localToWorldB, displayHandler, id, tag);
		}
		break;

	case hkpConstraintData::CONSTRAINT_TYPE_LIMITEDHINGE:
		{
			hkpLimitedHingeDrawer drawer;
			drawer.setScale(scale);
			const hkpLimitedHingeConstraintData* data = static_cast<const hkpLimitedHingeConstraintData*>(constraintData);
			drawer.drawConstraint(data, localToWorldA, localToWorldB, displayHandler, id, tag);
		}
		break;

	case hkpConstraintData::CONSTRAINT_TYPE_PRISMATIC:
		{
			hkpPrismaticDrawer drawer;
			drawer.setScale(scale);
			const hkpPrismaticConstraintData* data = static_cast<const hkpPrismaticConstraintData*>(constraintData);
			drawer.drawConstraint(data, localToWorldA, localToWorldB, displayHandler, id, tag);
		}
		break;

	case hkpConstraintData::CONSTRAINT_TYPE_STIFFSPRING:
		{
			hkpStiffSpringDrawer drawer;
			drawer.setScale(scale);
			const hkpStiffSpringConstraintData* data = static_cast<const hkpStiffSpringConstraintData*>(constraintData);
			drawer.drawConstraint(data, localToWorldA, localToWorldB, displayHandler, id, tag);
		}
		break;

	case hkpConstraintData::CONSTRAINT_TYPE_WHEEL:
		{
			hkpWheelDrawer drawer;
			drawer.setScale(scale);
			const hkpWheelConstraintData* data = static_cast<const hkpWheelConstraintData*>(constraintData);
			drawer.drawConstraint(data, localToWorldA, localToWorldB, displayHandler, id, tag);
		}
		break;

	case hkpConstraintData::CONSTRAINT_TYPE_POINTTOPATH:
		{
			hkpPointToPathDrawer drawer;
			drawer.setScale(scale);
			const hkpPointToPathConstraintData* data = static_cast<const hkpPointToPathConstraintData*>(constraintData);
			drawer.drawConstraint(data, localToWorldA, localToWorldB, displayHandler, id, tag);
		}
		break;

	case hkpConstraintData::CONSTRAINT_TYPE_POINTTOPLANE:
		{
			hkpPointToPlaneDrawer drawer;
			drawer.setScale(scale);
			const hkpPointToPlaneConstraintData* data = static_cast<const hkpPointToPlaneConstraintData*>(constraintData);
			drawer.drawConstraint(data, localToWorldA, localToWorldB, displayHandler, id, tag);
		}
		break;

	case hkpConstraintData::CONSTRAINT_TYPE_RAGDOLL:
		{
			hkpRagdollDrawer drawer;
			drawer.setScale(scale);
			const hkpRagdollConstraintData* data = static_cast<const hkpRagdollConstraintData*>(constraintData);
			drawer.drawConstraint(data, localToWorldA, localToWorldB, displayHandler, id, tag);
		}
		break;

	case hkpConstraintData::CONSTRAINT_TYPE_PULLEY:
		{
			hkpPulleyDrawer drawer;
			drawer.setScale(scale);
			const hkpPulleyConstraintData* data = static_cast<const hkpPulleyConstraintData*>(constraintData);
			drawer.drawConstraint(data, localToWorldA, localToWorldB, displayHandler, id, tag);
		}
		break;

	case hkpConstraintData::CONSTRAINT_TYPE_RACK_AND_PINION:
		{
			hkpRackAndPinionDrawer drawer;
			drawer.setScale(scale);
			const hkpRackAndPinionConstraintData* data = static_cast<const hkpRackAndPinionConstraintData*>(constraintData);
			drawer.drawConstraint(data, localToWorldA, localToWorldB, displayHandler, id, tag);
		}
		break;

	case hkpConstraintData::CONSTRAINT_TYPE_COG_WHEEL:
		{
			hkpCogWheelDrawer drawer;
			drawer.setScale(scale);
			const hkpCogWheelConstraintData* data = static_cast<const hkpCogWheelConstraintData*>(constraintData);
			drawer.drawConstraint(data, localToWorldA, localToWorldB, displayHandler, id, tag);
		}
		break;
	case hkpConstraintData::CONSTRAINT_TYPE_HINGE_LIMITS:
		{
			hkpHingeLimitsDrawer drawer;
			drawer.setScale(scale);
			const hkpHingeLimitsData* data = static_cast<const hkpHingeLimitsData*>(constraintData);
			drawer.drawConstraint(data, localToWorldA, localToWorldB, displayHandler, id, tag);
		}
		break;
	case hkpConstraintData::CONSTRAINT_TYPE_RAGDOLL_LIMITS:
		{
			hkpRagdollLimitsDrawer drawer;
			drawer.setScale(scale);
			const hkpRagdollLimitsData* data = static_cast<const hkpRagdollLimitsData*>(constraintData);
			drawer.drawConstraint(data, localToWorldA, localToWorldB, displayHandler, id, tag);
		}
		break;
	}
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
