/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2012 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */

#include <Physics/Dynamics/Destruction/Utilities/hkpDestructionSpuUtil.h>

//
//	Inverse mapping constructor

inline hkpBreakableMultiMaterial::InverseMappingDescriptor::InverseMappingDescriptor()
:	m_offset(0)
,	m_numKeys(0)
{}

//
//	Returns the i-th sub-material. The returned pointer is on PPU.

inline const hkpBreakableMaterial* hkpBreakableMultiMaterial::getSubMaterial(MaterialId subMaterialIndex) const
{
	// Return the PPU sub-material pointer.
	return m_subMaterials[subMaterialIndex];
}


//
//	Returns the inverse mapping

inline const hkpBreakableMultiMaterial::InverseMapping* hkpBreakableMultiMaterial::getInverseMapping() const
{
	return m_inverseMapping;
}

//
//	Clears the inverse mapping

inline void hkpBreakableMultiMaterial::clearInverseMapping()
{
	m_inverseMapping = HK_NULL;
}

//
//	Sets the i-th sub-material

inline void hkpBreakableMultiMaterial::setSubMaterial(MaterialId subMaterialIndex, hkpBreakableMaterial* subMaterial)
{
	m_subMaterials[subMaterialIndex] = subMaterial;
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
