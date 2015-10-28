#include "MMeshOptimizer.h"
#include "MHWBufferManager.h"
#include "MHashMap.h"

namespace Rad {

	void MeshOptimizer::Optimize(MeshSourcePtr mesh, int flags)
	{
		if (flags & MESH_OPT_VERTEX)
			optimizeVertex(mesh);

		if (flags & MESH_OPT_SKIN)
			optimizeSkin(mesh);
	}
}

using namespace Rad;

namespace opt_vertex {

	struct t_mbuffer
	{
		Array<int> mVertexData;
		Array<short> mIndexData;

		void Add(int vid)
		{
			mIndexData.PushBack(_addVertex(vid));
		}

		void Clear()
		{
			mVertexData.Clear();
			mIndexData.Clear();
		}

	protected:
		int _addVertex(int vid)
		{
			int i = 0;

			for (i = 0; i < mVertexData.Size(); ++i)
			{
				if (mVertexData[i] == vid)
					return i;
			}

			mVertexData.PushBack(vid);

			return mVertexData.Size() - 1;
		}
	};
}

void MeshOptimizer::optimizeVertex(MeshSourcePtr mesh)
{
	opt_vertex::t_mbuffer mbuffer;

	for (int i = 0; i < mesh->GetMeshBufferCount(); ++i)
	{
		MeshBuffer * mb = mesh->GetMeshBuffer(i);

		VertexDeclaration * vd = &mb->GetRenderOp()->vertexDeclarations[0];
		int stride = vd->GetVertexSize();

		int vertexCount = mb->GetRenderOp()->vertexBuffers[0]->GetCount();
		int indexCount = mb->GetRenderOp()->indexBuffer->GetCount();

		byte * vertexBuffer = (byte *)mb->GetRenderOp()->vertexBuffers[0]->Lock(eLockFlag::READ);
		word * indexBuffer = (word *)mb->GetRenderOp()->indexBuffer->Lock(eLockFlag::READ_WRITE);

		for (int v = 0; v < indexCount; ++v)
		{
			d_assert(indexBuffer[v] < vertexCount);
			mbuffer.Add(indexBuffer[v]);
		}

		VertexBufferPtr vb = HWBufferManager::Instance()->NewVertexBuffer(stride, mbuffer.mVertexData.Size());
		byte * vdata = (byte *)vb->Lock(eLockFlag::WRITE);
		for (int v = 0; v < mbuffer.mVertexData.Size(); ++v)
		{
			int index = mbuffer.mVertexData[v];

			memcpy(vdata, vertexBuffer + index * stride, stride);

			vdata += stride;
		}
		vb->Unlock();

		for (int v = 0; v < mbuffer.mIndexData.Size(); ++v)
		{
			indexBuffer[v] = mbuffer.mIndexData[v];
		}

		mb->GetRenderOp()->vertexBuffers[0]->Unlock();
		mb->GetRenderOp()->indexBuffer->Unlock();

		mb->GetRenderOp()->vertexBuffers[0] = vb;
		
		mbuffer.Clear();
	}
}

namespace opt_skin {

	struct t_vertex
	{
		int index;
		byte bi[4];
		float * bw;
	};

	struct t_mtl
	{
		int doubleSide;
		int opacity;
		Float4 emissive;
		Float4 ambient;
		Float4 diffuse;
		Float4 specular;
		float specularPower;

		String emissiveMap;
		String diffuseMap;
		String normalMap;
		String specularMap;
	};

	struct t_mbuffer
	{
		typedef HashMap<word, word, t_hash<word> > VertexNrMap;

		MeshBuffer * mMeshBuffer;
		VertexNrMap mVertexNumberMap;
		Array<t_vertex> mVertexData;
		Array<word> mIndexData;
		Array<short> mBoneIdMap;

	public:
		t_mbuffer(MeshBuffer * mb) { mMeshBuffer = mb; }
		~t_mbuffer() {}

		bool TestFace(const t_vertex * v)
		{
			int numNewBones = 0;

			const unsigned char * bindex = NULL;
			const Array<short> & boneIdMap = mMeshBuffer->GetBoneIdMap();

			for (int i = 0; i < 3; ++i)
			{
				const t_vertex & vx = v[i];

				if (vx.bw[0] > 0 && !_hasBone(boneIdMap[vx.bi[0]]))
					++numNewBones;
				if (vx.bw[1] > 0 && !_hasBone(boneIdMap[vx.bi[1]]))
					++numNewBones;
				if (vx.bw[2] > 0 && !_hasBone(boneIdMap[vx.bi[2]]))
					++numNewBones;
				if (vx.bw[3] > 0 && !_hasBone(boneIdMap[vx.bi[3]]))
					++numNewBones;
			}

			return mBoneIdMap.Size() + numNewBones <= MAX_HW_BONE;
		}

		void AddFace(const t_vertex * v, const word * ib)
		{
			const Array<short> & boneIdMap = mMeshBuffer->GetBoneIdMap();

			for (int i = 0; i < 3; ++i)
			{
				bool found = false;
				word vertexNr = ib[i];

				VertexNrMap::Node * whr = mVertexNumberMap.Find(vertexNr);
				VertexNrMap::Node * end = mVertexNumberMap.End();

				if (whr != end)
				{
					word index = whr->value;
					mIndexData.PushBack(index);

					whr = mVertexNumberMap.Next(whr);

					found = true;
				}

				if (found)
					continue;

				t_vertex vx = v[i];

				vx.bi[0] = vx.bw[0] > 0 ? _getBoneIndex(boneIdMap[vx.bi[0]]) : 0;
				vx.bi[1] = vx.bw[1] > 0 ? _getBoneIndex(boneIdMap[vx.bi[1]]) : 0;
				vx.bi[2] = vx.bw[2] > 0 ? _getBoneIndex(boneIdMap[vx.bi[2]]) : 0;
				vx.bi[3] = vx.bw[3] > 0 ? _getBoneIndex(boneIdMap[vx.bi[3]]) : 0;

				mVertexData.PushBack(vx);

				int vertexId = mVertexData.Size() - 1;
				mVertexNumberMap.Insert(vertexNr, vertexId);
				mIndexData.PushBack(vertexId);
			}
		}

	protected:
		bool _hasBone(short id)
		{
			for (int i = 0; i < mBoneIdMap.Size(); ++i)
			{
				if (mBoneIdMap[i] == id)
					return true;
			}

			return false;
		}

		int _getBoneIndex(short id)
		{
			int i = 0;

			for (i = 0; i < mBoneIdMap.Size(); ++i)
			{
				if (mBoneIdMap[i] == id)
					return i;
			}

			mBoneIdMap.PushBack(id);

			return i;
		}
	};

}

bool _checkVertexElem(MeshBuffer * mb)
{
	VertexDeclaration * vd = &mb->GetRenderOp()->vertexDeclarations[0];
	int v_ok = 0;

	if (vd->GetElementOffset(eVertexSemantic::POSITION) != -1)
		++v_ok;

	if (vd->GetElementOffset(eVertexSemantic::NORMAL) != -1)
		++v_ok;

	if (vd->GetElementOffset(eVertexSemantic::TEXCOORD0) != -1)
		++v_ok;

	if (vd->GetElementOffset(eVertexSemantic::BONE_INDEX) != -1)
		++v_ok;

	if (vd->GetElementOffset(eVertexSemantic::BONE_WEIGHT) != -1)
		++v_ok;

	return v_ok == 5;
}

void MeshOptimizer::optimizeSkin(MeshSourcePtr mesh)
{
	Array<opt_skin::t_mbuffer *> mbuffers;

	for (int i = 0; i < mesh->GetMeshBufferCount(); ++i)
	{
		MeshBuffer * mb = mesh->GetMeshBuffer(i);
		if (mb->GetBoneIdMap().Size() <= MAX_HW_BONE)
			continue;
		if (!_checkVertexElem(mb))
			continue;

		VertexDeclaration * vd = &mb->GetRenderOp()->vertexDeclarations[0];

		int stride = vd->GetVertexSize();
		int posoff = vd->GetElementOffset(eVertexSemantic::POSITION);
		int nrmoff = vd->GetElementOffset(eVertexSemantic::NORMAL);
		int tcoff = vd->GetElementOffset(eVertexSemantic::TEXCOORD0);
		int bioff = vd->GetElementOffset(eVertexSemantic::BONE_INDEX);
		int bwoff = vd->GetElementOffset(eVertexSemantic::BONE_WEIGHT);

		opt_skin::t_vertex vb[3];
		word ib[3]; 
		int vertexCount = mb->GetRenderOp()->vertexBuffers[0]->GetCount();
		int indexCount = mb->GetRenderOp()->indexBuffer->GetCount();
		int lastIndex = -1;

		byte * vertexBuffer = (byte *)mb->GetRenderOp()->vertexBuffers[0]->Lock(eLockFlag::READ);
		word * indexBuffer = (word *)mb->GetRenderOp()->indexBuffer->Lock(eLockFlag::READ);

		for (int v = 0; v < indexCount / 3; ++v)
		{
			ib[0] = indexBuffer[v * 3 + 0];
			ib[1] = indexBuffer[v * 3 + 1];
			ib[2] = indexBuffer[v * 3 + 2];

			const byte * buff = vertexBuffer + ib[0] * stride;
			vb[0].index = ib[0];
			vb[0].bw = (float *)(buff + bwoff);
			memcpy(vb[0].bi, (byte *)(buff + bioff), 4);

			buff = vertexBuffer + ib[1] * stride;
			vb[1].index = ib[1];
			vb[1].bw = (float *)(buff + bwoff);
			memcpy(vb[1].bi, (byte *)(buff + bioff), 4);

			buff = vertexBuffer + ib[2] * stride;
			vb[2].index = ib[2];
			vb[2].bw = (float *)(buff + bwoff);
			memcpy(vb[2].bi, (byte *)(buff + bioff), 4);

			bool found = false;

			if (lastIndex != -1 && mbuffers[lastIndex]->TestFace(vb))
			{
				mbuffers[lastIndex]->AddFace(vb, ib);
				found = true;
			}
			else
			{
				for (int j = 0; j < mbuffers.Size(); ++j)
				{
					if (mbuffers[j]->TestFace(vb))
					{
						mbuffers[j]->AddFace(vb, ib);
						lastIndex = j;
						found = true;
						break;
					}
				}
			}

			if (!found)
			{
				opt_skin::t_mbuffer * newBuffer = new opt_skin::t_mbuffer(mb);

				newBuffer->AddFace(vb, ib);

				lastIndex = mbuffers.Size();
				mbuffers.PushBack(newBuffer);
			}
		}

		for (int j = 0; j < mbuffers.Size(); ++j)
		{
			opt_skin::t_mbuffer * buff = mbuffers[j];
			MeshBuffer * newBuffer = mesh->NewMeshBuffer(i + 1 + j);
			newBuffer->GetRenderOp()->vertexDeclarations = mb->GetRenderOp()->vertexDeclarations;
			newBuffer->GetRenderOp()->primCount = buff->mIndexData.Size() / 3;
			newBuffer->GetRenderOp()->primType = ePrimType::TRIANGLE_LIST;

			VertexBufferPtr vb = HWBufferManager::Instance()->NewVertexBuffer(stride, buff->mVertexData.Size());
			byte * vdata = (byte *)vb->Lock(eLockFlag::WRITE);
			for (int v = 0; v < buff->mVertexData.Size(); ++v)
			{
				opt_skin::t_vertex & vtx = buff->mVertexData[v];

				memcpy(vdata, vertexBuffer + vtx.index * stride, stride);
				memcpy(vdata + bioff, vtx.bi, 4);

				vdata += stride;
			}
			vb->Unlock();

			IndexBufferPtr ib = HWBufferManager::Instance()->NewIndexBuffer(buff->mIndexData.Size());
			word * idata = (word *)ib->Lock(eLockFlag::WRITE);
			for (int v = 0; v < buff->mIndexData.Size(); ++v)
			{
				idata[v] = buff->mIndexData[v];
			}
			ib->Unlock();

			newBuffer->GetRenderOp()->vertexBuffers[0] = vb;
			newBuffer->GetRenderOp()->indexBuffer = ib;

			newBuffer->GetBoneIdMap() = buff->mBoneIdMap;

			*newBuffer->GetMaterial() = *mb->GetMaterial();

			delete buff;
		}

		mb->GetRenderOp()->vertexBuffers[0]->Unlock();
		mb->GetRenderOp()->indexBuffer->Unlock();

		mesh->DeleteMeshBuffer(i);

		mbuffers.Clear();
	}
}











