#include "PSBillboardSet.h"
#include "PSSource.h"

namespace Rad {

	ImplementRTTI(PS_BillboardSet, PS_Set);

	DF_PROPERTY_BEGIN(PS_BillboardSet)
		DF_PROPERTY_EX(PS_BillboardSet, mShaderClass, "Billboard", "Shader", "PT_Class PS_Shader", PT_FixedString)
		DF_PROPERTY_EX(PS_BillboardSet, mTextureName, "Billboard", "Texture", "PT_Filename", PT_String)
		DF_PROPERTY_ENUM(PS_BillboardSet, mBillboardType, "Billboard", "Type", PS_BillboardType)
		DF_PROPERTY(PS_BillboardSet, mBillboardCenter, "Billboard", "Center", PT_Float2)
		DF_PROPERTY(PS_BillboardSet, mCommonDirection, "Billboard", "CommonDir", PT_Float3)
		DF_PROPERTY(PS_BillboardSet, mCommonUpVector, "Billboard", "CommonUp", PT_Float3)
		DF_PROPERTY_ENUM(PS_BillboardSet, mBlendMode, "Billboard", "BlendMode", PS_BlendMode)
		DF_PROPERTY(PS_BillboardSet, mDepthCheck, "Billboard", "DepthCheck", PT_Bool)
		DF_PROPERTY(PS_BillboardSet, mAccurateFacing, "Billboard", "AccFacing", PT_Bool)
		DF_PROPERTY(PS_BillboardSet, mKeepAspect, "Billboard", "KeepAspect", PT_Bool)
	DF_PROPERTY_END()

	PS_BillboardSet::PS_BillboardSet()
		: mBillboardType(PS_BillboardType::POINT)
		, mBillboardCenter(0.5f, 0.5f)
		, mCommonDirection(0, 0, 1)
		, mCommonUpVector(0, 1, 0)
		, mBlendMode(PS_BlendMode::ADD)
		, mDepthCheck(true)
		, mAccurateFacing(false)
		, mKeepAspect(false)
	{
		mBillboard.mParent = this;

		SetShaderClass("PS_ShaderStandard");
	}

	PS_BillboardSet::~PS_BillboardSet()
	{
	}

	void PS_BillboardSet::OnPropertyChanged(const Property * p)
	{
		if (p->name == "mShaderClass")
		{
			SetShaderClass(mShaderClass);
		}
		else if (p->name == "mTextureName")
		{
			SetTexture(mTextureName);
		}
		else if (p->name == "mBillboardCenter")
		{
			SetBillboardCenter(mBillboardCenter);
		}
		else
		{
			PS_Set::OnPropertyChanged(p);
		}
	}

	void PS_BillboardSet::AllocQuota(int quota)
	{
		mBillboard._allocBuffer(quota);
	}

	void PS_BillboardSet::AddRenderQueue(RenderQueue * rq)
	{
		if (mSorting && mParticles.Size())
		{
			Camera * pCamera = World::Instance()->GetCurrentRenderContext()->GetCamera();

			PS_SortOp op;
			op.mRender = this;
			op.mCameraPosition = pCamera->GetWorldPosition();

			Sort(&mParticles[0], mParticles.Size(), op);
		}

		mBillboard._updateBuffer();

		rq->AddRenderObject(&mBillboard);
	}

	void PS_BillboardSet::Update(float elapsedTime)
	{
		PS_Set::Update(elapsedTime);

		if (mParticles.Size() > 0)
		{
			mBound = Aabb::Invalid;

			for (int i = 0; i < mParticles.Size(); ++i)
			{
				const Particle * p = mParticles[i];

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

			mBillboard._update();
		}
	}

	void PS_BillboardSet::SetShaderClass(const FixedString32 & name)
	{
		mShaderClass = name;

		mShader = NULL;

		if (mShaderClass != "")
		{
			mShader = Root::NEW_OBJECT_T<PS_Shader>(mShaderClass.c_str());
		}
	}

	void PS_BillboardSet::SetBillboardType(int type)
	{
		mBillboardType = type;
	}

	void PS_BillboardSet::SetBillboardCenter(const Float2 & center)
	{
		mBillboardCenter.x = Math::Clamp(center.x, 0.0f, 1.0f);
		mBillboardCenter.y = Math::Clamp(center.y, 0.0f, 1.0f);
	}

	void PS_BillboardSet::SetCommonDirection(const Float3 & dir)
	{
		mCommonDirection = dir;
		mCommonDirection.Normalize();
	}

	void PS_BillboardSet::SetCommonUpVector(const Float3 & up)
	{
		mCommonUpVector = up;
		mCommonUpVector.Normalize();
	}

	void PS_BillboardSet::SetBlendMode(int blendMode)
	{
		mBlendMode = blendMode;
	}

	void PS_BillboardSet::SetTexture(const String & name)
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

	void PS_BillboardSet::SetAccurateFacing(bool facing)
	{
		mAccurateFacing = facing;
	}

	void PS_BillboardSet::SetKeepAspect(bool b)
	{
		mKeepAspect = b;
	}

	void PS_BillboardSet::SetDepthCheck(bool b)
	{
		mDepthCheck = b;
	}
}
