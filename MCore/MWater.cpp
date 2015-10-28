#include "MWater.h"
#include "MShaderFXManager.h"
#include "MHWBufferManager.h"
#include "MHWBufferManager.h"
#include "MRenderQueue.h"
#include "MRenderHelper.h"
#include "MWorld.h"
#include "MRoot.h"

namespace Rad {

	float d_GetWaterDepth(float x, float z, Water * pWater)
	{
		const Float3 & wp = pWater->GetPosition();

		float d = 0;
		x += wp.x, z += wp.z;

		if (Terrain::Instance()->IsValid())
		{
			float h = 0;
			if (Terrain::Instance()->GetHeightAt(h, x, z))
			{
				d = Max(0.0f, wp.y - h);
			}
		}

		return d;
	}

	Water::WaterDepthFunction Water::d_functoin = NULL;

#define WATER_VERSION 10000

	ImplementRTTI(Water, Node);

	DF_PROPERTY_BEGIN(Water)
		DF_PROPERTY_EX(Water, mShaderClass, "Shader", "Shader", "PT_Class WaterShader", PT_FixedString)
	DF_PROPERTY_END()

	Water::Water()
		: Node("Water")
		, mGridSize(0)
		, mGridWidth(0)
		, mGridHeight(0)
		, mInvGridSize(0)
		, mXSize(0)
		, mZSize(0)
		, mGridInfo(NULL)
	{
		mVersion = WATER_VERSION;

		mSortSpecial = true;

		mMaterial.blendMode = eBlendMode::ALPHA_BLEND;
		mMaterial.depthMode = eDepthMode::N_LESS_EQUAL;

		SetTmFlags(eTmFlags::TRANSLATE);

		SetShaderClass("WaterShaderStandard");
	}
	Water::~Water()
	{
		Destroy();
	}

	void Water::OnPropertyChanged(const Property * p)
	{
		if (p->name == "ShaderClass")
		{
			SetShaderClass(mShaderClass);
			return ;
		}

		Node::OnPropertyChanged(p);
	}

	void Water::Build(unsigned char * grid, float gridSize, int w, int h)
	{
		d_assert (w > 0 && h > 0 && gridSize > 0);

		Destroy();

		mGridSize = gridSize;
		mGridWidth = w, mGridHeight = h;
		mGridInfo = new unsigned char[w * h];
		memcpy(mGridInfo, grid, w * h);

		mXSize = mGridWidth * gridSize;
		mZSize = mGridHeight * gridSize;
		mInvGridSize = 1.0f / gridSize;
		
		mRenderOp.vertexDeclarations[0].AddElement(eVertexSemantic::POSITION, eVertexType::FLOAT2);
		mRenderOp.vertexDeclarations[0].AddElement(eVertexSemantic::TEXCOORD0, eVertexType::FLOAT1);

		int iGridCount = 0;

		for (int z = 0; z < mGridHeight; ++z)
		{
			const unsigned char * line_grid = mGridInfo + z * mGridWidth;
			
			for (int x = 0; x < mGridWidth; ++x)
			{
				if (line_grid[x])
					iGridCount += 1;
			}
		}

		if (iGridCount == 0)
			return ;

		int iVertexCount = iGridCount * 4;
		int iIndexCount = iGridCount * 6;

		mRenderOp.vertexBuffers[0] = HWBufferManager::Instance()->NewVertexBuffer(sizeof(float) * 3, iVertexCount);
		mRenderOp.indexBuffer = HWBufferManager::Instance()->NewIndexBuffer(iIndexCount);

		float * vdata = (float *)mRenderOp.vertexBuffers[0]->Lock(eLockFlag::WRITE);
		short * idata = (short *)mRenderOp.indexBuffer->Lock(eLockFlag::WRITE);

		int index = 0;
		for (int z = 0; z < mGridHeight; ++z)
		{
			const unsigned char * line_grid = mGridInfo + z * mGridWidth;

			for (int x = 0; x < mGridWidth; ++x)
			{
				if (line_grid[x])
				{
					float x0 = x * mGridSize;
					float x1 = (x + 1) * mGridSize;
					float z0 = z * mGridSize;
					float z1 = (z + 1) * mGridSize;

					if (d_functoin)
					{
						*vdata++ = x0; *vdata++ = z0; *vdata++ = d_functoin(x0, z0, this);
						*vdata++ = x1; *vdata++ = z0; *vdata++ = d_functoin(x1, z0, this);
						*vdata++ = x0; *vdata++ = z1; *vdata++ = d_functoin(x0, z1, this);
						*vdata++ = x1; *vdata++ = z1; *vdata++ = d_functoin(x1, z1, this);
					}
					else
					{
						*vdata++ = x0; *vdata++ = z0; *vdata++ = d_GetWaterDepth(x0, z0, this);
						*vdata++ = x1; *vdata++ = z0; *vdata++ = d_GetWaterDepth(x1, z0, this);
						*vdata++ = x0; *vdata++ = z1; *vdata++ = d_GetWaterDepth(x0, z1, this);
						*vdata++ = x1; *vdata++ = z1; *vdata++ = d_GetWaterDepth(x1, z1, this);
					}

					*idata++ = index + 2;
					*idata++ = index + 1;
					*idata++ = index + 0;

					*idata++ = index + 2;
					*idata++ = index + 3;
					*idata++ = index + 1;

					index += 4;
				}
			}
		}

		mRenderOp.vertexBuffers[0]->Unlock();
		mRenderOp.indexBuffer->Unlock();

		mRenderOp.primCount = iIndexCount / 3;
		mRenderOp.primType = ePrimType::TRIANGLE_LIST;

		Aabb aabb = Aabb(0, 0, 0, mGridSize * mGridWidth, 0, mGridSize * mGridHeight);
		SetLocalAabb(aabb);

		mColMesh.Alloc(iVertexCount, iIndexCount);
		Float3 * colVert = mColMesh.GetVertices();
		int * colIdx = mColMesh.GetIndices();

		index = 0;
		for (int z = 0; z < mGridHeight; ++z)
		{
			const unsigned char * line_grid = mGridInfo + z * mGridWidth;

			for (int x = 0; x < mGridWidth; ++x)
			{
				if (line_grid[x])
				{
					float x0 = x * mGridSize;
					float x1 = (x + 1) * mGridSize;
					float z0 = z * mGridSize;
					float z1 = (z + 1) * mGridSize;

					*colVert++ = Float3(x0, 0, z0);
					*colVert++ = Float3(x1, 0, z0);
					*colVert++ = Float3(x0, 0, z1);
					*colVert++ = Float3(x1, 0, z1);
					
					*colIdx++ = index + 2;
					*colIdx++ = index + 1;
					*colIdx++ = index + 0;

					*colIdx++ = index + 2;
					*colIdx++ = index + 3;
					*colIdx++ = index + 1;

					index += 4;
				}
			}
		}

		ChangeTm(eTmFlags::UNKNOWN);
	}

	void Water::Destroy()
	{
		safe_delete_array (mGridInfo);
		mGridSize = 0;
		mGridWidth = mGridHeight = 0;

		mXSize = mZSize = 0;
		mInvGridSize = 0;

		mRenderOp.vertexDeclarations[0].Clear();
		mRenderOp.vertexBuffers[0] = NULL;
		mRenderOp.indexBuffer = NULL;
		mRenderOp.primCount = 0;

		mColMesh.Clear();
	}

	int Water::SortSpecial(RenderObject * obj2, const Float3 & eye)
	{
		RenderObject * obj1 = this;
		Float3 p0, p1;

		obj1->_getWorldPosition(p0);
		obj2->_getWorldPosition(p1);

		if (eye.y >= p0.y)
		{
			return p0.y < p1.y ? -1 : (p0.y > p1.y ? 1 : 0);
		}
		else
		{
			return p0.y > p1.y ? -1 : (p0.y < p1.y ? 1 : 0);
		}
	}

	void Water::Update(float elapsedTime)
	{
		Node::Update(elapsedTime);

		if (mWaterShader != NULL)
		{
			mWaterShader->Update(this, elapsedTime);
		}
	}

	void Water::AddRenderQueue(RenderQueue * rq)
	{
		SetRenderCallBack(eRenderCallBack::SHADER, mWaterShader.c_ptr());
		rq->AddRenderObject(this);
	}

	void Water::Optimize()
	{
		int min_x = 0, min_z = 0;
		int max_x = 0, max_z = 0;

		for (int z = 0; z < mGridHeight; ++z)
		{
			const unsigned char * line_grid = mGridInfo + z * mGridWidth;

			for (int x = 0; x < mGridWidth; ++x)
			{
				if (line_grid[x])
				{
					min_x = Min(min_x, x);
					min_z = Min(min_z, z);

					max_x = Max(max_x, x);
					max_z = Max(max_z, z);
				}
			}
		}

		if (min_x == 0 && min_z == 0 &&
			max_x == mGridWidth - 1 && max_z == mGridHeight - 1)
			return ;

		int newWidth = max_x - min_x + 1;
		int newHeight = max_z - min_z + 1;

		unsigned char * newGrid = new unsigned char[newWidth * newHeight];
		
		for (int z = 0; z < newHeight; ++z)
		{
			for (int x = 0; x < newWidth; ++x)
			{
				newGrid[z * newWidth + x] = mGridInfo[(z + min_z) * mGridWidth + (x + min_x)];
			}
		}

		Build(newGrid, mGridSize, newWidth, newHeight);

		safe_delete_array(newGrid);
	}

	bool Water::MapCoord(int & x, int & z, float fx, float fz)
	{
		fx -= Node::GetWorldPosition().x;
		fz -= Node::GetWorldPosition().z;

		if (fx < 0 || fx > mXSize ||
			fz < 0 || fz > mZSize)
			return false;

		x = (int)(fx * mInvGridSize);
		z = (int)(fz * mInvGridSize);

		x = Math::Clamp(x, 0, mGridWidth - 1);
		z = Math::Clamp(z, 0, mGridHeight - 1);

		return true;
	}

	void Water::SetShaderClass(const FixedString32 & name)
	{
		mShaderClass = name;

		mWaterShader = Root::NEW_OBJECT_T<WaterShader>(name.c_str());
		d_assert (mWaterShader != NULL);
	}

	void Water::SetWaterShader(WaterShaderPtr shader)
	{
		mWaterShader = shader;

		mShaderClass = shader->GetRTTI()->Name();
	}

	void Water::Serialize(Serializer & sl)
	{
		Node::Serialize(sl);

		if (sl.IsOut())
		{
			OSerializer & OS = (OSerializer &)sl;

			float gridSize = GetGridSize();
			int gridWidth = GetGridWidth();
			int gridHeight = GetGridHeight();

			OS << gridSize;
			OS << gridWidth;
			OS << gridHeight;

			if (gridSize > 0 && gridWidth > 0 && gridHeight > 0)
			{
				OS.Write(GetGridInfo(), gridWidth * gridHeight);

				int hasDepth = 0;
				OS.Write(&hasDepth, sizeof(int));
			}

			if (mWaterShader != NULL)
			{
				const char * className = mWaterShader->GetRTTI()->Name();
				int nameLen = strlen(className);

				OS << nameLen;
				OS.Write(className, nameLen);
				mWaterShader->Serialize(OS);
			}
			else
			{
				OS << 0;
			}
		}
		else
		{
			ISerializer & IS = (ISerializer &)sl;

			d_assert (mVersion == WATER_VERSION);

			float gridSize = 0;
			int gridWidth = 0, gridHeight = 0;

			IS >> gridSize;
			IS >> gridWidth;
			IS >> gridHeight;

			if (gridWidth > 0 && gridHeight > 0 && gridSize > 0)
			{
				unsigned char * grid = new unsigned char[gridWidth * gridHeight];
				IS.Read(grid, gridWidth * gridHeight);

				int hasDepth = 0;
				IS >> hasDepth;

				float * depthMap = NULL;
				if (hasDepth == 1)
				{
					depthMap = new float[(gridWidth + 1) * (gridHeight + 1)];
					IS.Read(depthMap, sizeof(float) * (gridWidth + 1) * (gridHeight + 1));
				}

				Build(grid, gridSize, gridWidth, gridHeight);

				safe_delete_array(grid);
				safe_delete_array(depthMap);
			}

			int nameLen;
			char name[128];

			IS >> nameLen;

			if (nameLen > 0)
			{
				d_assert (nameLen < 32);
				IS.Read(name, nameLen);
				name[nameLen] = 0;

				SetShaderClass(name);
				d_assert (mWaterShader != NULL);

				mWaterShader->Serialize(IS);
			}
		}
	}

}