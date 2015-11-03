#include "PSDecalSet.h"

namespace Rad {

	ImplementRTTI(PS_DecalSet, PS_Set);

	DF_PROPERTY_BEGIN(PS_DecalSet)
		DF_PROPERTY_EX(PS_DecalSet, mShaderClass, "Decal", "Shader", "PT_Class PS_Shader", PT_FixedString)
		DF_PROPERTY_EX(PS_DecalSet, mTextureName, "Decal", "Texture", "PT_Filename", PT_String)
		DF_PROPERTY_ENUM(PS_DecalSet, mBlendMode, "Decal", "BlendMode", PS_BlendMode)
		DF_PROPERTY(PS_DecalSet, mKeepAspect, "Decal", "KeepAspect", PT_Bool)
		DF_PROPERTY(PS_DecalSet, mCenter, "Decal", "Center", PT_Float2)
		DF_PROPERTY(PS_DecalSet, mGrids, "Decal", "Grids", PT_Int2)
	DF_PROPERTY_END()

	PS_DecalSet::PS_DecalSet()
		: mBlendMode(PS_BlendMode::ADD)
		, mKeepAspect(false)
		, mCenter(0.5f, 0.5f)
		, mGrids(4, 4)
	{
		mTexture = RenderHelper::Instance()->GetWhiteTexture();
		mRenderObject.mParent = this;

		SetShaderClass("PS_ShaderStandard");
	}

	PS_DecalSet::~PS_DecalSet()
	{
		_clear_particle();
	}

	void PS_DecalSet::OnPropertyChanged(const Property * p)
	{
		if (p->name == "mShaderClass")
		{
			SetShaderClass(mShaderClass);
		}
		else if (p->name == "mTextureName")
		{
			SetTexture(mTextureName);
		}
		else if (p->name == "mGrids")
		{
			SetGrids(mGrids);
		}
		else
		{
			PS_Set::OnPropertyChanged(p);
		}
	}

	void PS_DecalSet::AllocQuota(int quota)
	{
		mRenderObject._allocQuota(quota);
	}

	void PS_DecalSet::AddRenderQueue(RenderQueue * rq)
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
					PS_Decal * data = (PS_Decal *)p->InternalData;

					data->_updateBuffer(p, &vertexBuffer, &indexBuffer, startVertex, primCount);
				}
			}

			mRenderObject._unlock(primCount);

			rq->AddRenderObject(&mRenderObject);
		}
	}

	void PS_DecalSet::Update(float elapsedTime)
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
					PS_Decal * ribbon = (PS_Decal *)p->InternalData;

					ribbon->_update(p);
				}
			}

			if (IsLocalSpace())
			{
				mBound.Transform(mParent->GetWorldTM());
			}

			mRenderObject._update();
		}
	}

	void PS_DecalSet::SetShaderClass(const FixedString32 & name)
	{
		mShaderClass = name;

		mShader = NULL;
		if (mShaderClass != "")
		{
			mShader = Root::NEW_OBJECT_T<PS_Shader>(mShaderClass.c_str());
		}
	}

	void PS_DecalSet::SetBlendMode(int blendMode)
	{
		mBlendMode = blendMode;
	}

	void PS_DecalSet::SetTexture(const String & name)
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

	void PS_DecalSet::SetKeepAspect(bool b)
	{
		mKeepAspect = b;
	}

	void PS_DecalSet::SetGrids(const Int2 & grids)
	{
		mGrids.x = Max(grids.x, 1);
		mGrids.y = Max(grids.y, 1);

		SetQuota(mQuota);
	}

	void PS_DecalSet::SetCenter(const Float2 & center)
	{
		mCenter.x = Math::Clamp(center.x, 0.0f, 1.0f);
		mCenter.y = Math::Clamp(center.y, 0.0f, 1.0f);
	}

	Particle * PS_DecalSet::_quest_particle()
	{
		Particle * p = PS_Set::_quest_particle();
		if (p)
		{
			p->InternalData = new PS_Decal(this);
		}

		return p;
	}

	void PS_DecalSet::_free_particle(Particle * p)
	{
		if (p->InternalData)
		{
			PS_Decal * data = (PS_Decal *)p->InternalData;

			delete data;

			p->InternalData = NULL;
		}

		PS_Set::_free_particle(p);
	}

	void PS_DecalSet::_clear_particle()
	{
		for (int i = 0; i < mParticles.Size(); ++i)
		{
			Particle * p = mParticles[i];

			if (p->InternalData)
			{
				PS_Decal * data = (PS_Decal *)p->InternalData;

				delete data;

				p->InternalData = NULL;
			}
		}

		PS_Set::_clear_particle();
	}

}