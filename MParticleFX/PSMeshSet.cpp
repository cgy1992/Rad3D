#include "PSMeshSet.h"

namespace Rad {

	ImplementRTTI(PS_MeshSet, PS_Set);

	DF_PROPERTY_BEGIN(PS_MeshSet)
		DF_PROPERTY_EX(PS_MeshSet, mMeshName, "Mesh", "Filename", "PT_Filename mesh(*.mesh)|*.mesh|", PT_String)
	DF_PROPERTY_END()

	PS_MeshSet::PS_MeshSet()
	{
	}

	PS_MeshSet::~PS_MeshSet()
	{
		_clear_particle();
	}

	void PS_MeshSet::OnPropertyChanged(const Property * p)
	{
		if (p->name == "mMeshName")
		{
			SetMesh(mMeshName);
		}
		else
		{
			PS_Set::OnPropertyChanged(p);
		}
	}

	void PS_MeshSet::AllocQuota(int quota)
	{
	}

	void PS_MeshSet::AddRenderQueue(RenderQueue * rq)
	{
		if (mSorting && mParticles.Size())
		{
			Camera * pCamera = World::Instance()->GetCurrentRenderContext()->GetCamera();

			PS_SortOp op;
			op.mRender = this;
			op.mCameraPosition = pCamera->GetWorldPosition();

			Sort(&mParticles[0], mParticles.Size(), op);
		}
	}

	void PS_MeshSet::Update(float elapsedTime)
	{
		PS_Set::Update(elapsedTime);

		if (mParticles.Size() > 0)
		{
			mBound = Aabb::Invalid;

			for (int i = 0; i < mParticles.Size(); ++i)
			{
				Particle * p = mParticles[i];

				if (p->InternalData)
				{
					PS_Mesh * data = (PS_Mesh *)p->InternalData;

					data->_update(p);
				}

				Float3 vMin = p->Position - p->Size * 0.5f;
				Float3 vMax = p->Position + p->Size * 0.5f;

				mBound.minimum = Float3::Minimum(mBound.minimum, vMin);
				mBound.maximum = Float3::Maximum(mBound.maximum, vMax);
			}

			if (IsLocalSpace())
			{
				mBound.Transform(mParent->GetWorldTM());
			}
		}
	}

	void PS_MeshSet::SetMesh(const String & name)
	{
		mMeshName = name;

		if (mMeshName != "")
		{
			mSource = MeshManager::Instance()->LoadMesh(mMeshName);
		}
		else
		{
			mSource = NULL;
		}
	}

	Particle * PS_MeshSet::_quest_particle()
	{
		Particle * p = PS_Set::_quest_particle();
		if (p)
		{
			p->InternalData = new PS_Mesh(this);
		}

		return p;
	}

	void PS_MeshSet::_free_particle(Particle * p)
	{
		if (p->InternalData)
		{
			PS_Mesh * data = (PS_Mesh *)p->InternalData;

			delete data;

			p->InternalData = NULL;
		}

		PS_Set::_free_particle(p);
	}

	void PS_MeshSet::_clear_particle()
	{
		for (int i = 0; i < mParticles.Size(); ++i)
		{
			Particle * p = mParticles[i];

			if (p->InternalData)
			{
				PS_Mesh * data = (PS_Mesh *)p->InternalData;

				delete data;

				p->InternalData = NULL;
			}
		}

		PS_Set::_clear_particle();
	}

}