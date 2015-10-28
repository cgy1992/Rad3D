#include "PhyMeshData.h"
#include "PhyInternal.h"

namespace Rad {

	PhyMeshData::PhyMeshData()
		: _dataId(NULL)
		, _vertexData(NULL)
		, _indexData(NULL)
	{
	}

	PhyMeshData::~PhyMeshData()
	{
		safe_delete_array (_vertexData);
		safe_delete_array (_indexData);

		PhyMeshDataPool::Instance()->Remove(this);
	}

	//
	ImplementSingleton(PhyMeshDataPool);

	PhyMeshDataPool::PhyMeshDataPool()
		: mMeshDataLinker(NULL)
	{
	}

	PhyMeshDataPool::~PhyMeshDataPool()
	{
		d_assert (mMeshDataLinker == NULL);
	}

	PhyMeshDataPtr PhyMeshDataPool::Load(
		const String & name,
		const Float3 * vertexData, int vertexCount,
		const int * indexData, int indexCount,
		const Float3 & scale)
	{
		d_assert (vertexData != NULL && indexData != NULL);

		String uname;
		uname.Format("%s_%.2f_%.2f_%.2f", name.c_str(), scale.x, scale.y, scale.z);

		// Find
		PhyMeshData * node = mMeshDataLinker;
		while (node != NULL)
		{
			if (node->_uname == uname)
				return node;

			node = LINKER_NEXT(node);
		}

		// Create New Mesh
		PhyMeshData * meshData = new PhyMeshData;

		meshData->_dataId = dGeomTriMeshDataCreate();
		meshData->_uname = uname;
		meshData->_vertexData = new Float3[vertexCount];
		meshData->_vertexCount = vertexCount;
		meshData->_indexData = new int[indexCount];
		meshData->_indexCount = indexCount;

		for (int i = 0; i < vertexCount; ++i)
		{
			meshData->_vertexData[i] = vertexData[i] * scale;
		}

		for (int i = 0; i < indexCount; ++i)
		{
			meshData->_indexData[i] = indexData[i];
		}

		dGeomTriMeshDataBuildSingle((dTriMeshDataID)(meshData->_dataId),
			meshData->_vertexData, sizeof(Float3), meshData->_vertexCount,
			meshData->_indexData, meshData->_indexCount, 3 * sizeof(int));

		LINKER_APPEND(mMeshDataLinker, meshData);

		return meshData;
	}

	void PhyMeshDataPool::Remove(PhyMeshData * data)
	{
		dGeomTriMeshDataDestroy((dTriMeshDataID)(data->_dataId));

		LINKER_REMOVE(mMeshDataLinker, data);
	}
}