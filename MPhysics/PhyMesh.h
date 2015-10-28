/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PhyEntity.h"
#include "PhyMeshData.h"

namespace Rad {

	class PHY_ENTRY PhyMesh : public PhyEntity
	{
		DECLARE_RTTI();

	public:
		PhyMesh(eType type);
		virtual ~PhyMesh();

		void 
			SetMeshData(PhyMeshDataPtr data);
		PhyMeshDataPtr 
			GetMeshData();

		void 
			SetMass(float mass);

		virtual void
			OnAttach(Node * owner);
		virtual void 
			OnLoaded();
		virtual void 
			OnUnload();
		virtual void 
			OnChangeTm(int flags);

	protected:
		PhyMeshDataPtr mMeshData;
	};

}