#include "MAnimationSerializer.h"
#include "MAnimation.h"
#include "MMeshSerializer.h"

namespace Rad {

	void AnimationSerializer::Load(Animation * anim, DataStreamPtr stream)
	{
		int magic, version;

		stream->Read(&magic, sizeof(int));
		stream->Read(&version, sizeof(int));

		d_assert (magic == K_Magic && version == K_Version);

		int ckId = 0;
		while (stream->Read(&ckId, sizeof(int)) == sizeof(int))
		{
			switch (ckId)
			{
			case MC_ANIMATION:
			case MC_ANIMATION0:
				ReadAnimation(NULL, anim, stream);
				break;

			case MC_ANIMATION_EVENT:
				ReadAnimationEvent(anim, stream);
				break;
			}
		}
	}

	void AnimationSerializer::ReadAnimation(MeshSource * mesh, Animation * anim, DataStreamPtr stream)
	{
		int version = -1;

		stream->Read(&version, sizeof (int));

		d_assert (
			version == K_Animation_V0 ||
			version == K_Animation_V1 ||
			version == K_Animation_V2);

		if (mesh != NULL)
		{
			d_assert (anim == NULL);

			char name[128];
			if (version == K_Animation_V0)
			{
				stream->Read(name, 128);
			}
			else
			{
				stream->ReadString(name, 128);
			}

			anim = mesh->NewAnimation(name);
		}
	
		if (version > K_Animation_V0)
		{
			float len = 0;
			stream->Read(&len, sizeof(float));
			anim->SetLength(len);
		}
		
		int numAnims = 0;
		stream->Read(&numAnims, sizeof(int));

		for (int i = 0; i < numAnims; ++i)
		{
			int boneId = 0, frames = 0;

			stream->Read(&boneId, sizeof(int));
			stream->Read(&frames, sizeof(int));

			if (frames == 0)
				continue;

			SkeletonAnimation * sk_anim = new SkeletonAnimation(boneId);
			for (int j = 0; j < frames; ++j)
			{
				float time = 0;
				stream->Read(&time, sizeof(float));

				SkeletonAnimation::KeyFrame * kf = sk_anim->CreateKeyFrame(time);
				stream->Read(&kf->position, sizeof(float) * 3);
				stream->Read(&kf->rotation, sizeof(float) * 4);
				stream->Read(&kf->scale, sizeof(float) * 3);
			}

			anim->AddSkeletonAnimation(sk_anim);
		}

		if (version >= K_Animation_V2)
		{
			numAnims = 0;
			stream->Read(&numAnims, sizeof(int));

			for (int i = 0; i < numAnims; ++i)
			{
				int meshId = 0;
				int vcount = 0;
				stream->Read(&meshId, sizeof(int));
				stream->Read(&vcount, sizeof(int));

				d_assert (meshId > 0 && vcount > 0);
				d_assert (mesh == NULL || mesh->GetMeshBuffer(i)->GetRenderOp()->vertexBuffers[0]->GetCount() == vcount);

				VertexAnimation * vx_anim = new VertexAnimation(meshId, vcount);

				int frames = 0;
				stream->Read(&frames, sizeof(int));
				for (int j = 0; j < frames; ++j)
				{
					float time = 0;
					stream->Read(&time, sizeof(float));

					int mask = 0;
					stream->Read(&mask, sizeof(int));

					VertexAnimation::KeyFrame * kf = vx_anim->CreateKeyFrame(time);
					if (mask & 1)
					{
						kf->positions = new Float3[vcount];
						stream->Read(kf->positions, sizeof(Float3) * vcount);
					}

					if (mask & 2)
					{
						kf->normals = new Float3[vcount];
						stream->Read(kf->normals, sizeof(Float3) * vcount);
					}
				}

				anim->AddVertexAnimation(vx_anim);
			}
		}

		if (version == K_Animation_V0)
		{
			anim->_calcuLength();
		}
	}

	void AnimationSerializer::ReadAnimationEvent(Animation * anim, DataStreamPtr stream)
	{
		int count = 0;
		stream->Read(&count, sizeof(int));

		for (int i = 0; i < count; ++i)
		{
			float time;
			FixedString32 name;

			stream->Read(&time, sizeof(time));
			stream->Read(&name[0], sizeof(name));

			anim->AddEvent(time, name);
		}
	}

	bool AnimationSerializer::Save(Animation * anim, const String & filename)
	{
		FILE * fp = fopen(filename.c_str(), "wb");
		if (!fp)
			return false;

		OSerializerF stream(fp, true);

		int magic = K_Magic, version = K_Version;

		stream.Write(&magic, sizeof(int));
		stream.Write(&version, sizeof(int));

		WriteAnimation(NULL, anim, &stream);

		return true;
	}

	void AnimationSerializer::WriteAnimation(MeshSource * mesh, Animation * anim, OSerializer * stream)
	{
		int ckId = MC_ANIMATION;
		int version = K_Animation_V2;
		stream->Write(&ckId, sizeof(int));
		stream->Write(&version, sizeof(int));

		if (mesh != NULL)
		{
			stream->WriteString(anim->GetName().c_str());
		}

		float len = anim->GetLength();
		stream->Write(&len, sizeof(float));

		// Skeleton Animation
		int numAnim = anim->GetSkeletonAnimationCount();
		stream->Write(&numAnim, sizeof(int));

		for (int j = 0; j < numAnim; ++j)
		{
			SkeletonAnimation * sk_anim = anim->GetSkeletonAnimation(j);

			int bn = sk_anim->GetBoneHandle();
			int frames = sk_anim->GetFrameCount();
			stream->Write(&bn, sizeof(int));
			stream->Write(&frames, sizeof(int));

			for (int k = 0; k < frames; ++k)
			{
				SkeletonAnimation::KeyFrame * kf = sk_anim->GetKeyFrame(k);
				stream->Write(&kf->time, sizeof(float));
				stream->Write(&kf->position, sizeof(float) * 3);
				stream->Write(&kf->rotation, sizeof(float) * 4);
				stream->Write(&kf->scale, sizeof(float) * 3);
			}
		}

		// Vertex Animation
		numAnim = anim->GetVertexAnimationCount();
		stream->Write(&numAnim, sizeof(int));

		for (int j = 0; j < numAnim; ++j)
		{
			VertexAnimation * vx_anim = anim->GetVertexAnimation(j);

			int meshId = vx_anim->GetMeshId();
			int vcount = vx_anim->GetVertexCount();
			int frames = vx_anim->GetFrameCount();

			stream->Write(&meshId, sizeof(int));
			stream->Write(&vcount, sizeof(int));
			stream->Write(&frames, sizeof(int));

			for (int k = 0; k < frames; ++k)
			{
				VertexAnimation::KeyFrame * kf = vx_anim->GetKeyFrame(k);
				stream->Write(&kf->time, sizeof(float));

				int mask = 0;
				if (kf->positions)
					mask |= 1;
				if (kf->normals)
					mask |= 2;

				stream->Write(&mask, sizeof(float));

				if (kf->positions)
					stream->Write(kf->positions, sizeof(Float3) * frames);

				if (kf->normals)
					stream->Write(kf->normals, sizeof(Float3) * frames);
			}
		}

		// Event
		if (anim->GetEventCount() > 0)
		{
			AnimationSerializer::WriteAnimationEvent(anim, stream);
		}
	}

	void AnimationSerializer::WriteAnimationEvent(Animation * anim, OSerializer * stream)
	{
		int ckId = MC_ANIMATION_EVENT;
		stream->Write(&ckId, sizeof(int));

		int count = anim->GetEventCount();
		stream->Write(&count, sizeof(int));

		for (int i = 0; i < count; ++i)
		{
			Animation::Event * e = anim->GetEvent(i);

			stream->Write(&e->time, sizeof(e->time));
			stream->Write(&e->name[0], sizeof(e->name));
		}
	}

}
