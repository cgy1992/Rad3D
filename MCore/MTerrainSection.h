/*
*	TerrainSection
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MWorldSection.h"

namespace Rad {

	class TerrainMesh;

	class M_ENTRY TerrainSection : public ISectionData
	{
		DECLARE_RTTI();

	public:
		TerrainSection();
		virtual ~TerrainSection();

		virtual bool
			Init(WorldSection * section, const rml_node * def);

		virtual void
			Update(float elapsedTime) {}

		virtual void
			Serialize(Serializer & sl);

		const Array<int> &
			GetLayers() { return mLayers; }
		const Array<TerrainMesh *>
			GetMeshes() { return mMeshes; }

	protected:
		Array<int> mLayers;
		Array<TerrainMesh *> mMeshes;
	};
}