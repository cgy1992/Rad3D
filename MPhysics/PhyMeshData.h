/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PhyEntry.h"

namespace Rad {

	class PhyMesh;
	class PhyMeshDataPool;

	class PHY_ENTRY PhyMeshData
	{
		DECLARE_REF();
		DECLARE_ALLOC();
		DECLARE_LINKER(PhyMeshData);

		friend class PhyMesh;
		friend class PhyMeshDataPool;

	public:
		PhyMeshData();
		virtual ~PhyMeshData();

		void
			Release() { delete this; }

		int 
			_getVertexCount() { return _vertexCount; }
		int 
			_getIndexCount() { return _indexCount; }

	protected:
		PhyHandle _dataId;

		String _uname;
		Float3 * _vertexData;
		int _vertexCount;
		int * _indexData;
		int _indexCount;

	};

	typedef SmartPtr<PhyMeshData> PhyMeshDataPtr;

	//
	class PHY_ENTRY PhyMeshDataPool : public Singleton<PhyMeshDataPool>
	{
	public:
		PhyMeshDataPool();
		~PhyMeshDataPool();

		PhyMeshDataPtr 
			Load(const String & name, 
				const Float3 * vertexData, int vertexCount,
				const int * indexData, int indexCount,
				const Float3 & scale);

		void 
			Remove(PhyMeshData * data);

	protected:
		PhyMeshData * mMeshDataLinker;
	};

}