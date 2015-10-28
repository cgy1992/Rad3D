#include "MMeshGroup.h"
#include "MHWBufferManager.h"

namespace Rad {

	MeshGroup::MeshGroup()
	{
	}

	MeshGroup::~MeshGroup()
	{
		Destroy();
	}

	void MeshGroup::Append(Mesh * mesh)
	{
		if (mesh->GetSource() != NULL &&
			mesh->GetPrimCount() < 800 &&
			mesh->GetSLMode() != eStaticLightingMode::LIGHTING_MAP)
		{
			if (mesh->HasLightingColor())
			{
				mMeshBucket[1].Insert(mesh->GetSource().c_ptr(), mesh);
			}
			else
			{
				mMeshBucket[0].Insert(mesh->GetSource().c_ptr(), mesh);
			}
		}
	}

	void MeshGroup::Build()
	{
		_build(0);
		_build(1);
	}

	void MeshGroup::_build(int index)
	{
		const MultiMap<MeshSource *, Mesh *>::Node * i = mMeshBucket[index].Begin();
		while (i != mMeshBucket[index].End())
		{
			int iVertexCount = 0;
			int iIndexCount = 0;

			MeshSource * source = i->key;
			for (int j = 0; j < source->GetMeshBufferCount(); ++j)
			{
				MeshBuffer * mb = source->GetMeshBuffer(j);
				RenderOp * pRenderOp = mb->GetRenderOp();

				d_assert (
					!pRenderOp->vertexDeclarations[0].HasElement(eVertexSemantic::POSITION, eVertexType::FLOAT3) &&
					pRenderOp->vertexBuffers[0] != NULL &&
					pRenderOp->indexBuffer != NULL &&
					pRenderOp->primType != ePrimType::TRIANGLE_LIST);

				iVertexCount += pRenderOp->vertexBuffers[0]->GetCount();
				iIndexCount += pRenderOp->indexBuffer->GetCount();
			}

			int first = 0, last = 0;
			int vcount = 0, icount = 0;
			while (last < i->members.Size())
			{
				vcount += iVertexCount;
				icount += iIndexCount;
				last += 1;

				if (last == i->members.Size() ||
					iVertexCount > 4096 ||
					iIndexCount > 4096 * 3)
				{
					_genMesh(i->members, first, last, index != 0);

					first = last;
				}
			}

			i = i->Next();
		}
	}

	void MeshGroup::_genMesh(const Array<Mesh *> & arr, int first, int last, bool hasLightingColor)
	{
		MeshSourcePtr source = arr[0]->GetSource();
		Mesh * mesh = new Mesh;

		for (int i = 0; i < source->GetMeshBufferCount(); ++i)
		{
			SubMesh * submesh = mesh->NewSubMesh();
			VertexBufferPtr srcVB = source->GetMeshBuffer(i)->GetRenderOp()->vertexBuffers[0];
			IndexBufferPtr srcIB = source->GetMeshBuffer(i)->GetRenderOp()->indexBuffer;
			int p_offset = source->GetMeshBuffer(i)->GetRenderOp()->vertexDeclarations[0].GetElementOffset(eVertexSemantic::POSITION);
			int n_offset = source->GetMeshBuffer(i)->GetRenderOp()->vertexDeclarations[0].GetElementOffset(eVertexSemantic::NORMAL);
			int stride = srcVB->GetStride();

			VertexBufferPtr vb = HWBufferManager::Instance()->NewVertexBuffer(stride, srcVB->GetCount() * (last - first));
			
			const char * v_src = (const char *)srcVB->Lock(eLockFlag::READ);
			char * v_dest = (char *)vb->Lock(eLockFlag::WRITE);
			for (int j = first; j < last; ++j)
			{
				const Mat4 & worldTM = arr[j]->GetWorldTM();
				bool hasScale = arr[j]->GetWorldScale() != Float3(1, 1, 1);

				for (int k = 0; k < srcVB->GetCount(); ++k)
				{
					memcpy(v_dest, v_src, stride);

					Float3 * position = (Float3 *)(v_dest + p_offset);
					position->TransformA(worldTM);
					
					if (n_offset != -1)
					{
						Float3 * normal = (Float3 *)(v_dest + n_offset);
						normal->TransformN(worldTM);

						if (hasScale)
						{
							normal->Normalize();
						}
					}

					v_dest += stride;
					v_src += stride;
				}
			}
			vb->Unlock();
			srcVB->Unlock();

			IndexBufferPtr ib = HWBufferManager::Instance()->NewIndexBuffer(srcIB->GetCount() * (last - first));

			int startVertex = 0;
			const short * i_src = (const short *)srcIB->Lock(eLockFlag::READ);
			char * i_dest = (char *)ib->Lock(eLockFlag::WRITE);
			for (int j = first; j < last; ++j)
			{
				for (int k = 0; k < srcIB->GetCount(); ++k)
				{
					*i_dest++ = (*i_src++) + startVertex;
				}

				startVertex += srcVB->GetCount();
			}
			ib->Unlock();
			srcIB->Unlock();
			
			submesh->GetRenderOp()->vertexDeclarations[0] = source->GetMeshBuffer(i)->GetRenderOp()->vertexDeclarations[0];
			submesh->GetRenderOp()->vertexBuffers[0] = vb;
			submesh->GetRenderOp()->indexBuffer = ib;
			submesh->GetRenderOp()->primCount = ib->GetCount() / 3;
			submesh->GetRenderOp()->primType = ePrimType::TRIANGLE_LIST;

			if (hasLightingColor)
			{
				int count = submesh->GetRenderOp()->vertexBuffers[0]->GetCount();
				submesh->GetRenderOp()->vertexDeclarations[LIGHTING_COLOR_STREAM].AddElement(eVertexSemantic::LIGHTING_COLOR, eVertexType::UBYTE4);
				submesh->GetRenderOp()->vertexBuffers[LIGHTING_COLOR_STREAM] = HWBufferManager::Instance()->NewVertexBuffer(4, count);

				Array<Color> lightColors;
				Color * data = (Color *)submesh->GetRenderOp()->vertexBuffers[LIGHTING_COLOR_STREAM]->Lock(eLockFlag::WRITE);
				for (int j = first; j < last; ++j)
				{
					arr[j]->GetLightingColor(lightColors);
					d_assert (lightColors.Size() > 0);

					memcpy(data, &lightColors[0], 4 * count);

					startVertex += count;
					lightColors.Clear();
				}
				submesh->GetRenderOp()->vertexBuffers[LIGHTING_COLOR_STREAM]->Unlock();
			}

			*submesh->GetMaterial() = *source->GetMeshBuffer(i)->GetMaterial();

			submesh->SetMeshShader(source->GetMeshBuffer(i)->GetShader());
		}

		mesh->SetSLMode(hasLightingColor ? eStaticLightingMode::LIGHTING_COLOR : eStaticLightingMode::NONE);

		mMeshes.PushBack(mesh);
	}

	void MeshGroup::Destroy()
	{
		for (int i = 0; i < mMeshes.Size(); ++i)
		{
			delete mMeshes[i];
		}

		mMeshes.Clear();
	}

}