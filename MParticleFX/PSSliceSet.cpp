#include "PSSliceSet.h"
#include "PSSlice.h"

namespace Rad {

	ImplementRTTI(PS_SliceSet, PS_Set);

	DF_PROPERTY_BEGIN(PS_SliceSet)
		DF_PROPERTY_EX(PS_SliceSet, mShaderClass, "Slice", "Shader", "PT_Class PS_Shader", PT_FixedString)
		DF_PROPERTY_EX(PS_SliceSet, mTextureName, "Slice", "Texture", "PT_Filename", PT_String)
		DF_PROPERTY_ENUM(PS_SliceSet, mBlendMode, "Slice", "BlendMode", PS_BlendMode)
		DF_PROPERTY(PS_SliceSet, mKeepAspect, "Slice", "KeepAspect", PT_Bool)
		DF_PROPERTY(PS_SliceSet, mSegment, "Slice", "Segment", PT_Int)
		DF_PROPERTY(PS_SliceSet, mSegmentLife, "Slice", "SegmentLife", PT_Float)
	DF_PROPERTY_END()

	PS_SliceSet::PS_SliceSet()
		: mBlendMode(PS_BlendMode::ADD)
		, mKeepAspect(false)
		, mSegment(32)
		, mSegmentLife(0)
		, mUsePoint(false)
	{
		mTexture = RenderHelper::Instance()->GetWhiteTexture();

		mRenderObject.mParent = this;

		SetShaderClass("PS_ShaderStandard");
	}

	PS_SliceSet::~PS_SliceSet()
	{
		_clear_particle();
	}

	void PS_SliceSet::OnPropertyChanged(const Property * p)
	{
		if (p->name == "mShaderClass")
		{
			SetShaderClass(mShaderClass);
		}
		else if (p->name == "mTextureName")
		{
			SetTexture(mTextureName);
		}
		else if (p->name == "mSegment")
		{
			SetSegment(mSegment);
		}
		else
		{
			PS_Set::OnPropertyChanged(p);
		}
	}

	void PS_SliceSet::AllocQuota(int quota)
	{
		mRenderObject._allocQuota(quota);
	}

	void PS_SliceSet::AddRenderQueue(RenderQueue * rq)
	{
		if (mSorting && mParticles.Size())
		{
			Camera * pCamera = World::Instance()->GetCurrentRenderContext()->GetCamera();

			PS_SortOp op;
			op.mRender = this;
			op.mCameraPosition = pCamera->GetWorldPosition();

			Sort(&mParticles[0], mParticles.Size(), op);
		}

		if (mParticles.Size() > 0)
		{
			void * vertexBuffer = mRenderObject._lockVertex();
			void * indexBuffer = mRenderObject._lockIndex();
			int startVertex = 0;
			int primCount = 0;

			for (int i = 0; i < mParticles.Size(); ++i)
			{
				Particle * p = mParticles[i];

				if (p->InternalData)
				{
					PS_Slice * data = (PS_Slice *)p->InternalData;

					data->_updateBuffer(p, &vertexBuffer, &indexBuffer, startVertex, primCount);
				}
			}

			mRenderObject._unlock(primCount);

			rq->AddRenderObject(&mRenderObject);
		}
	}

	void PS_SliceSet::Update(float elapsedTime)
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
					PS_Slice * data = (PS_Slice *)p->InternalData;

					data->_update(p, elapsedTime);

					mBound.Merge(data->_getBound());
				}
			}

			if (IsLocalSpace())
			{
				mBound.minimum += mParent->GetWorldPosition();
				mBound.maximum += mParent->GetWorldPosition();
			}

			mRenderObject._update();
		}
	}

	void PS_SliceSet::SetShaderClass(const FixedString32 & name)
	{
		mShaderClass = name;

		mShader = NULL;

		if (mShaderClass != "")
		{
			mShader = Root::NEW_OBJECT_T<PS_Shader>(mShaderClass.c_str());
		}
	}

	void PS_SliceSet::SetTexture(const String & name)
	{
		mTextureName = name;

		if (mTexture != NULL)
		{
			mTexture = HWBufferManager::Instance()->LoadTexture(name, 0);
		}
		else
		{
			mTexture = RenderHelper::Instance()->GetWhiteTexture();
		}
	}

	void PS_SliceSet::SetBlendMode(int blendMode)
	{
		mBlendMode = blendMode;
	}

	void PS_SliceSet::SetKeepAspect(bool b)
	{
		mKeepAspect = b;
	}

	void PS_SliceSet::SetSegment(int segment)
	{
		mSegment = Max(segment, 1);

		SetQuota(mQuota);
	}

	void PS_SliceSet::SetSegmentLife(float life)
	{
		mSegmentLife = life;
	}

	void PS_SliceSet::SetPoint(const Float3 & point1, const Float3 & point2)
	{
		mPoint1 = point1;
		mPoint2 = point2;
		mUsePoint = true;
	}

	Particle * PS_SliceSet::_quest_particle()
	{
		Particle * p = PS_Set::_quest_particle();
		if (p)
		{
			p->InternalData = new PS_Slice(this);
		}

		return p;
	}

	void PS_SliceSet::_free_particle(Particle * p)
	{
		if (p->InternalData)
		{
			PS_Slice * data = (PS_Slice *)p->InternalData;

			delete data;

			p->InternalData = NULL;
		}

		PS_Set::_free_particle(p);
	}

	void PS_SliceSet::_clear_particle()
	{
		for (int i = 0; i < mParticles.Size(); ++i)
		{
			Particle * p = mParticles[i];

			if (p->InternalData)
			{
				PS_Slice * data = (PS_Slice *)p->InternalData;

				delete data;

				p->InternalData = NULL;
			}
		}

		PS_Set::_clear_particle();
	}

}