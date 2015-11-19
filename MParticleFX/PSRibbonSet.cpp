#include "PSRibbonSet.h"

namespace Rad {

	ImplementRTTI(PS_RibbonSet, PS_Set);

	DF_PROPERTY_BEGIN(PS_RibbonSet)
		DF_PROPERTY_EX(PS_RibbonSet, mShaderClass, "Ribbon", "Shader", "PT_Class PS_Shader", PT_FixedString)
		DF_PROPERTY_EX(PS_RibbonSet, mTextureName, "Ribbon", "Texture", "PT_Filename", PT_String)
		DF_PROPERTY_ENUM(PS_RibbonSet, mBlendMode, "Ribbon", "BlendMode", PS_BlendMode)
		DF_PROPERTY(PS_RibbonSet, mKeepAspect, "Ribbon", "KeepAspect", PT_Bool)
		DF_PROPERTY(PS_RibbonSet, mSegment, "Ribbon", "Segment", PT_Int)
		DF_PROPERTY(PS_RibbonSet, mSegmentLife, "Ribbon", "SegmentLife", PT_Float)
		DF_PROPERTY(PS_RibbonSet, mLoose, "Ribbon", "Loose", PT_Bool)
	DF_PROPERTY_END()

	PS_RibbonSet::PS_RibbonSet()
		: mBlendMode(PS_BlendMode::ADD)
		, mKeepAspect(false)
		, mSegment(32)
		, mSegmentLife(0)
		, mLoose(false)
	{
		mTexture = RenderHelper::Instance()->GetWhiteTexture();

		SetShaderClass("PS_ShaderStandard");
	}

	PS_RibbonSet::~PS_RibbonSet()
	{
		_clear_particle();
	}

	void PS_RibbonSet::OnPropertyChanged(const Property * p)
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

	void PS_RibbonSet::AllocQuota(int quota)
	{
		mRenderObject._allocQuota(quota);
	}

	void PS_RibbonSet::AddRenderQueue(RenderQueue * rq)
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
					PS_Ribbon * ribbon = (PS_Ribbon *)p->InternalData;

					ribbon->_updateBuffer(p, &vertexBuffer, &indexBuffer, startVertex, primCount);
				}
			}

			mRenderObject._unlock(primCount);

			rq->AddRenderObject(&mRenderObject);
		}
	}

	void PS_RibbonSet::Init(ParticleSystem * parent)
	{
		PS_Set::Init(parent);

		mRenderObject.Init(this);
	}

	void PS_RibbonSet::Update(float elapsedTime)
	{
		PS_Set::Update(elapsedTime);

		if (mParticles.Size() > 0)
		{
			mBound = Aabb::Invalid;

			for (int i = 0; i < mParticles.Size(); ++i)
			{
				Particle * p = mParticles[i];

				Float3 vMin = p->Position - p->Size * 0.5f;
				Float3 vMax = p->Position + p->Size * 0.5f;

				mBound.minimum = Float3::Minimum(mBound.minimum, vMin);
				mBound.maximum = Float3::Maximum(mBound.maximum, vMax);

				if (p->InternalData)
				{
					PS_Ribbon * ribbon = (PS_Ribbon *)p->InternalData;

					ribbon->_update(p, elapsedTime);
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

	void PS_RibbonSet::SetShaderClass(const FixedString32 & name)
	{
		mShaderClass = name;

		mShader = NULL;

		if (mShaderClass != "")
		{
			mShader = Root::NEW_OBJECT_T<PS_Shader>(mShaderClass.c_str());
		}
	}

	void PS_RibbonSet::SetTexture(const String & name)
	{
		mTextureName = name;

		if (mTextureName != "")
		{
			mTexture = HWBufferManager::Instance()->LoadTexture(name, 0);
		}
		else
		{
			mTexture = RenderHelper::Instance()->GetWhiteTexture();
		}
	}

	void PS_RibbonSet::SetBlendMode(int blendMode)
	{
		mBlendMode = blendMode;
	}

	void PS_RibbonSet::SetKeepAspect(bool b)
	{
		mKeepAspect = b;
	}

	void PS_RibbonSet::SetSegment(int segment)
	{
		mSegment = Max(segment, 1);

		SetQuota(mQuota);
	}

	void PS_RibbonSet::SetSegmentLife(float life)
	{
		mSegmentLife = life;
	}

	void PS_RibbonSet::SetLoose(bool loose)
	{
		mLoose = loose;
	}

	Particle * PS_RibbonSet::_quest_particle()
	{
		Particle * p = PS_Set::_quest_particle();
		if (p)
		{
			p->InternalData = new PS_Ribbon(this);
		}

		return p;
	}

	void PS_RibbonSet::_free_particle(Particle * p)
	{
		if (p->InternalData)
		{
			PS_Ribbon * ribbon = (PS_Ribbon *)p->InternalData;

			delete ribbon;

			p->InternalData = NULL;
		}

		PS_Set::_free_particle(p);
	}

	void PS_RibbonSet::_clear_particle()
	{
		for (int i = 0; i < mParticles.Size(); ++i)
		{
			Particle * p = mParticles[i];

			if (p->InternalData)
			{
				PS_Ribbon * ribbon = (PS_Ribbon *)p->InternalData;

				delete ribbon;

				p->InternalData = NULL;
			}
		}

		PS_Set::_clear_particle();
	}

}