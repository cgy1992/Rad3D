#include "PSLineSet.h"

namespace Rad {

	ImplementRTTI(PS_LineSet, PS_Set);

	DF_PROPERTY_BEGIN(PS_LineSet)
		DF_PROPERTY_EX(PS_LineSet, mShaderClass, "Line", "Shader", "PT_Class PS_Shader", PT_FixedString)
		DF_PROPERTY_EX(PS_LineSet, mTextureName, "Line", "Texture", "PT_Filename", PT_String)
		DF_PROPERTY_ENUM(PS_LineSet, mBlendMode, "Line", "BlendMode", PS_BlendMode)
		DF_PROPERTY(PS_LineSet, mKeepAspect, "Line", "KeepAspect", PT_Bool)
		DF_PROPERTY(PS_LineSet, mSegment, "Line", "Segment", PT_Int)
		DF_PROPERTY(PS_LineSet, mTileScale, "Line", "TileScale", PT_Float)
		DF_PROPERTY(PS_LineSet, mWaveStep, "Wave", "Step", PT_Float)
		DF_PROPERTY(PS_LineSet, mWaveDir, "Wave", "Direction", PT_Float3)
		DF_PROPERTY(PS_LineSet, mWaveFreq, "Wave", "Frequency", PT_Float3)
		DF_PROPERTY(PS_LineSet, mWaveWeight, "Wave", "Weight", PT_Float3)
	DF_PROPERTY_END()

	PS_LineSet::PS_LineSet()
		: mBlendMode(PS_BlendMode::ADD)
		, mKeepAspect(false)
		, mSegment(32)
		, mTileScale(0)
		, mWaveStep(0)
		, mWaveDir(0, 1, 0)
		, mWaveFreq(0, 0, 0)
		, mWaveWeight(0, 0, 0)
		, mUsePoint(false)
	{
		mTexture = RenderHelper::Instance()->GetWhiteTexture();

		SetShaderClass("PS_ShaderStandard");
	}

	PS_LineSet::~PS_LineSet()
	{
		_clear_particle();
	}

	void PS_LineSet::OnPropertyChanged(const Property * p)
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

	void PS_LineSet::AllocQuota(int quota)
	{
		mRenderObject._allocQuota(quota);
	}

	void PS_LineSet::AddRenderQueue(RenderQueue * rq)
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
					PS_Line * line = (PS_Line *)p->InternalData;

					line->_updateBuffer(p, &vertexBuffer, &indexBuffer, startVertex, primCount);
				}
			}

			mRenderObject._unlock(primCount);

			rq->AddRenderObject(&mRenderObject);
		}
	}

	void PS_LineSet::Init(ParticleSystem * parent)
	{
		PS_Set::Init(parent);

		mRenderObject.Init(this);
	}

	void PS_LineSet::Update(float elapsedTime)
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
					PS_Line * line = (PS_Line *)p->InternalData;

					line->_update(p);

					mBound.Merge(line->_getBound());
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
	
	void PS_LineSet::SetShaderClass(const FixedString32 & name)
	{
		mShaderClass = name;

		mShader = NULL;

		if (mShaderClass != "")
		{
			mShader = Root::NEW_OBJECT_T<PS_Shader>(mShaderClass.c_str());
		}
	}

	void PS_LineSet::SetTexture(const String & name)
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

	void PS_LineSet::SetBlendMode(int blendMode)
	{
		mBlendMode = blendMode;
	}

	void PS_LineSet::SetKeepAspect(bool b)
	{
		mKeepAspect = b;
	}

	void PS_LineSet::SetSegment(int segment)
	{
		mSegment = Max(segment, 1);

		SetQuota(mQuota);
	}

	void PS_LineSet::SetTileScale(float scale)
	{
		mTileScale = scale;
	}

	void PS_LineSet::SetPoint(const Float3 & point1, const Float3 & point2)
	{
		mPoint1 = point1;
		mPoint2 = point2;
		mUsePoint = true;
	}

	Particle * PS_LineSet::_quest_particle()
	{
		Particle * p = PS_Set::_quest_particle();
		if (p)
		{
			p->InternalData = new PS_Line(this);
		}

		return p;
	}

	void PS_LineSet::_free_particle(Particle * p)
	{
		if (p->InternalData)
		{
			PS_Line * line = (PS_Line *)p->InternalData;

			delete line;

			p->InternalData = NULL;
		}

		PS_Set::_free_particle(p);
	}

	void PS_LineSet::_clear_particle()
	{
		for (int i = 0; i < mParticles.Size(); ++i)
		{
			Particle * p = mParticles[i];

			if (p->InternalData)
			{
				PS_Line * line = (PS_Line *)p->InternalData;

				delete line;

				p->InternalData = NULL;
			}
		}

		PS_Set::_clear_particle();
	}

}