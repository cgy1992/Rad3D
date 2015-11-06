#include "PSGroupSet.h"

namespace Rad {

	ImplementRTTI(PS_GroupSet, PS_Set);

	DF_PROPERTY_BEGIN(PS_GroupSet)
		DF_PROPERTY(PS_GroupSet, mSetIndex, "Group", "SetIndex", PT_Int4)
	DF_PROPERTY_END()

	PS_GroupSet::PS_GroupSet()
		: mSetIndex(-1, -1, -1, -1)
	{
	}

	PS_GroupSet::~PS_GroupSet()
	{
		_clear_particle();
	}

	void PS_GroupSet::OnPropertyChanged(const Property * p)
	{
		if (p->name == "mSetIndex")
		{
			_updateSource();
		}
		else
		{
			PS_Set::OnPropertyChanged(p);
		}
	}

	void PS_GroupSet::AllocQuota(int quota)
	{
	}

	void PS_GroupSet::AddRenderQueue(RenderQueue * rq)
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

	void PS_GroupSet::Update(float elapsedTime)
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
					PS_Group * data = (PS_Group *)p->InternalData;

					data->_update(p);
				}

				Float3 vMin = p->Position - p->Size * 0.5f;
				Float3 vMax = p->Position + p->Size * 0.5f;

				mBound.minimum = Float3::Minimum(mBound.minimum, vMin);
				mBound.maximum = Float3::Maximum(mBound.maximum, vMax);
			}

			if (IsLocalSpace())
			{
				mBound.minimum += mParent->GetWorldPosition();
				mBound.maximum += mParent->GetWorldPosition();
			}
		}
	}

	void PS_GroupSet::SetSetIndex(const Int4 & sets)
	{
		mSetIndex = sets;

		_updateSource();
	}

	Particle * PS_GroupSet::_quest_particle()
	{
		if (mSource != NULL)
		{
			Particle * p = PS_Set::_quest_particle();
			if (p)
			{
				p->InternalData = new PS_Group(this);
			}

			return p;
		}
		
		return NULL;
	}

	void PS_GroupSet::_free_particle(Particle * p)
	{
		if (p->InternalData)
		{
			PS_Group * data = (PS_Group *)p->InternalData;

			delete data;

			p->InternalData = NULL;
		}

		PS_Set::_free_particle(p);
	}

	void PS_GroupSet::_clear_particle()
	{
		for (int i = 0; i < mParticles.Size(); ++i)
		{
			Particle * p = mParticles[i];

			if (p->InternalData)
			{
				PS_Group * data = (PS_Group *)p->InternalData;

				delete data;

				p->InternalData = NULL;
			}
		}

		PS_Set::_clear_particle();
	}

	void PS_GroupSet::_updateSource()
	{
		if (mParent->GetUserData(0) == (void *)1)
			return ;

		if (Root::InEditor())
		{
			mSource = NULL;

			int i0 = mSetIndex.x;
			if (i0 >= 0 && i0 < mParent->GetSetCount() && mParent->GetSet(i0) == this)
				return ;

			int i1 = mSetIndex.y;
			if (i1 >= 0 && i1 < mParent->GetSetCount() && mParent->GetSet(i1) == this)
				return ;

			int i2 = mSetIndex.z;
			if (i2 >= 0 && i2 < mParent->GetSetCount() && mParent->GetSet(i2) == this)
				return ;

			int i3 = mSetIndex.w;
			if (i3 >= 0 && i3 < mParent->GetSetCount() && mParent->GetSet(i3) == this)
				return ;

			OSerializerTester tester;
			mParent->SerializeTempalte(tester);

			byte * buffer = new byte[tester.Size()];
			OSerializerM os(buffer, tester.Size(), false);

			mParent->SerializeTempalte(os);

			mSource = new PS_Source("", "", true);
			mSource->Load();
			mSource->SetStream(new MemoryStream(buffer, tester.Size(), true));
		}
		else
		{
			mSource = mParent->GetSource();
		}
	}

}