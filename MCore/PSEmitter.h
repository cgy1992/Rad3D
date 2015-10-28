/*
*	PSEmiter
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MTypes.h"
#include "PSParticle.h"

namespace Rad {

	class PS_Set;
	class ParticleSystem;

	class M_ENTRY PS_Emitter : public IObject
	{
		DECLARE_RTTI();
		DECLARE_POOL_ALLOC();
		DECLARE_PROPERTY(IObject)

	protected:
		bool mEnable;
		bool mSizeable;

		Float3 mPosition;
		Float3 mRotation;
		Float3 mDirection;
		Float3 mCommonDirection;

		int mFirstEmit;
		float mRate;
		float mDurationTime;

		Float4 mMinColor, mMaxColor;
		Float2 mAngle;
		Float2 mLife;
		Float2 mSpeed;
		Float2 mDimension;
		Float3 mMinSize, mMaxSize;
		Float3 mMinRotation, mMaxRotation;

		Int2 mUVRect;

	public:
		PS_Emitter();
		virtual ~PS_Emitter();

		virtual void
			Init(PS_Set * render);
		virtual void
			Reset();

		PS_Set * 
			GetParent() { return mParent; }

		void 
			SetEnable(bool enable);
		bool 
			IsEnable() const;

		void
			SetSizeable(bool able);
		bool
			IsSizeable() const;

		void 
			SetRate(float rate);
		float 
			GetRate() const;

		void 
			SetDurationTime(float time);
		float 
			GetDurationTime() const;

		void 
			SetPosition(const Float3 & pos);
		const Float3 & 
			GetPosition() const;

		void 
			SetRotation(const Float3 & angles);
		void 
			SetDirection(const Float3 & dir, const Float3 & commonDir);
		const Float3 & 
			GetDirection() const;
		const Float3 & 
			GetCommonDirection() const;

		void 
			SetColor(const Float4 & _min, const Float4 & _max);
		const Float4 & 
			GetMinColor() const;
		const Float4 & 
			GetMaxColor() const;

		void 
			SetAngle(const Float2 & v);
		const Float2 &
			GetAngle() const { return mAngle; }

		void 
			SetLife(const Float2 & v);
		const Float2 & 
			GetLife() const { return mLife; }

		void 
			SetSpeed(const Float2 & v);
		const Float2 & 
			GetSpeed() const { return mSpeed; }

		void 
			SetDimension(const Float2 & v);
		const Float2 & 
			GetDimension() const { return mDimension; }

		void
			SetSize(const Float3 & minSize, const Float3 & maxSize);
		const Float3 &
			GetMinSize() const { return mMinSize; }
		const Float3 &
			GetMaxSize() const { return mMaxSize; }

		void
			SetRotation(const Float3 & minRotation, const Float3 & maxRotation);
		const Float3 &
			GetMinRotation() const { return mMinRotation; }
		const Float3 &
			GetMaxRotation() const { return mMaxRotation; }

		void 
			SetUVRect(const Int2 & xy);
		const Int2 &
			GetUVRect() const;
		RectF
			GetUVRect(int x, int y) const;
		RectF
			GetUVRect(int index) const;
		int
			GetUVRectCount() const;

		int 
			GetEmitCount() const;

		virtual void 
			InitParticle(Particle * p);
		bool 
			Emit(float fElapsedTime);

		virtual void
			OnPropertyChanged(const Property * p);

		virtual void
			Serialize(Serializer & sl);

		Float3 
			RandomDirection();
		Float3
			RandomRotation();
		Float3 
			RandomSize();
		Float4 
			RandomColor();
		RectF 
			RandomUVRect();
		float 
			RandomSpeed();
		float 
			RandomLife();
		
		virtual IObject *
			Clone();

	protected:
		PS_Set * mParent;

		Float2 mUVRectStep;
		float mInternalTime;
		float mLastEmitTime;

		int mEmitCount;
	};

}