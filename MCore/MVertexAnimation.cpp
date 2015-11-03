#include "MVertexAnimation.h"
#include "MAnimationController.h"
#include "MMesh.h"

namespace Rad {

	VertexAnimation::VertexAnimation(int meshId, int vertexCount)
		: mMeshId(meshId)
		, mVertexCount(vertexCount)
	{
	}

	VertexAnimation::~VertexAnimation()
	{
		for (int i = 0; i < mFrames.Size(); ++i)
		{
			safe_delete_array(mFrames[i].positions);
			safe_delete_array(mFrames[i].normals);
		}
	}

	void VertexAnimation::Update(Mesh * mesh, AnimationController * controller)
	{
		SubMesh * submesh = mesh->GetSubMesh(mMeshId);
		VertexBufferPtr buffer = submesh->QueryVertexAnimationBuffer();

		d_assert (buffer->GetCount() == mVertexCount);

		float time = controller->GetPosition();
		float weight = controller->GetWeight();

		int start = 0, end = mFrames.Size() - 1;
		while (start < end)
		{
			int mid = (start + end + 1) / 2;

			float cmp = time - mFrames[mid].time;
			if (cmp < 0)
			{
				end = mid - 1;
			}
			else if (cmp > 0)
			{
				start = mid;
			}
			else
			{
				start = mid;
				break;
			}
		}

		int k1 = start;
		if (k1 < mFrames.Size())
		{
			void * vertexData = buffer->Lock(eLockFlag::READ_WRITE);
			int stride = buffer->GetStride();

			if (k1 < mFrames.Size() - 1)
			{
				int k2 = k1 + 1;
				float t = (time - mFrames[k1].time) / (mFrames[k2].time - mFrames[k1].time);

				if (mFrames[k1].positions != NULL)
				{
					char * dest = (char *)vertexData;

					for (int i = 0; i < mVertexCount; ++i)
					{
						Float3 p = Float3::Lerp(mFrames[k1].positions[i], mFrames[k2].positions[i], t);

						p = Float3::Lerp(*(Float3*)dest, p, weight);

						*(Float3*)dest = p; 

						dest += stride;
					}
				}

				if (mFrames[k1].normals != NULL)
				{
					char * dest = (char *)vertexData + sizeof(Float3);

					for (int i = 0; i < mVertexCount; ++i)
					{
						Float3 p = Float3::Lerp(mFrames[k1].normals[i], mFrames[k2].normals[i], t);

						p = Float3::Lerp(*(Float3*)dest, p, weight);
						p.Normalize();

						*(Float3*)dest = p; 

						dest += stride;
					}
				}
			}
			else
			{
				if (mFrames[k1].positions != NULL)
				{
					char * dest = (char *)vertexData;

					for (int i = 0; i < mVertexCount; ++i)
					{
						Float3 p = Float3::Lerp(*(Float3*)dest, mFrames[k1].positions[i], controller->GetWeight());

						*(Float3*)dest = p; 

						dest += stride;
					}
				}

				if (mFrames[k1].normals != NULL)
				{
					char * dest = (char *)vertexData + sizeof(Float3);

					for (int i = 0; i < mVertexCount; ++i)
					{
						Float3 p = Float3::Lerp(*(Float3*)dest, mFrames[k1].normals[i], weight);
						p.Normalize();

						*(Float3*)dest = p; 

						dest += stride;
					}
				}
			}

			buffer->Unlock();
		}
	}

	VertexAnimation::KeyFrame * VertexAnimation::CreateKeyFrame(float time)
	{
		mFrames.Resize(mFrames.Size() + 1);
		mFrames.Back().time = time;
		mFrames.Back().positions = NULL;
		mFrames.Back().normals = NULL;

		return &mFrames[mFrames.Size() - 1];
	}

	void VertexAnimation::RemoveKeyFrame(int index)
	{
		safe_delete_array(mFrames[index].positions);
		safe_delete_array(mFrames[index].normals);

		mFrames.Erase(index);
	}

	int VertexAnimation::GetFrameCount()
	{
		return mFrames.Size();
	}

	VertexAnimation::KeyFrame * VertexAnimation::GetKeyFrame(int index)
	{
		return &mFrames[index];
	}

}