/*
*	KeyController
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MObject.h"
#include "MFunctional.h"

namespace Rad {

	class M_ENTRY IKeyController
	{
	public:
		IKeyController() {}
		virtual ~IKeyController() {}

		virtual void
			Clear() = 0;

		virtual int
			Size() = 0;

		virtual void
			Append(float _time, const IObject * _value) = 0;
		virtual void 
			Insert(int _i, float _time, const IObject * _value) = 0;
		virtual void
			Erase(int _i) = 0;

		virtual void
			SetKeyFrame(int _i, float time, const IObject * val) = 0;
		virtual void
			GetKeyFrame(int _i, float * time, IObject ** val) = 0;

		virtual void 
			Adjust() = 0;

		virtual void
			Serialize(Serializer & sl) = 0;
	};

	template<class T>
	class TKeyController : public IKeyController
	{
	public:
		struct KF {
			float time;
			T value;

			KF() {}
			KF(float _t, const T & v) : time(_t), value(v) {}
		};

		struct KF_Compare
		{
			int operator ()(const KF & a, const KF & b) const
			{
				return a.time < b.time ? -1 : (a.time > b.time ? 1 : 0);
			}
		};

	public:
		TKeyController() {}
		virtual ~TKeyController() {}

		void Clear() 
		{ 
			mKeyFrames.Clear(); 
		}

		int Size()
		{ 
			return mKeyFrames.Size(); 
		}

		void Append(float _time, const IObject * _value) 
		{ 
			Insert(Size(), _time, _value);
		}

		void Append(float _time, const T & _value)
		{ 
			Insert(Size(), KF(_time, _value));
		}

		void Insert(int _i, float _time, const IObject * _value)
		{
			d_assert (_time <= 1.0f);

			KF kf;
			kf.time = _time;

			if (_value != NULL)
			{
				kf.value.Copy(_value);
			}

			Insert(_i, kf);
		}

		void Insert(int _i, const KF & _frame)
		{
			mKeyFrames.Insert(_i, _frame);
		}

		void Erase(int _i)
		{ 
			mKeyFrames.Erase(_i);
		}

		bool GetValue(T & _value, float _time, bool _lerp) const
		{
			int start = 0, end = mKeyFrames.Size() - 1;
			while (start < end)
			{
				int mid = (start + end + 1) / 2;

				float cmp = _time - mKeyFrames[mid].time;
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
			if (k1 < mKeyFrames.Size())
			{
				int k2;
				if (_time < mKeyFrames[k1].time || k1 == mKeyFrames.Size() - 1)
					k2 = k1;
				else
					k2 = k1 + 1;

				if (_lerp && k1 < k2)
				{
					const KF & key1 = mKeyFrames[k1];
					const KF & key2 = mKeyFrames[k2];

					float t = (_time - key1.time) / (key2.time - key1.time);

					_value = key1.value.Lerp(key2.value, t);
				}
				else
				{
					_value = mKeyFrames[k1].value;
				}

				return true;
			}

			return false;
		}

		bool GetValue(T & _value, float _time, bool _lerp, int & _last) const
		{
			int k1 = _last;
			for (int i = _last + 1; i < mKeyFrames.Size(); ++i)
			{
				if (mKeyFrames[i].time > _time)
					break;

				k1 = i;
			}

			_last = k1;

			if (k1 < mKeyFrames.Size())
			{
				int k2;
				if (_time < mKeyFrames[k1].time || k1 == mKeyFrames.Size() - 1)
					k2 = k1;
				else
					k2 = k1 + 1;

				if (_lerp && k1 < k2)
				{
					const KF & key1 = mKeyFrames[k1];
					const KF & key2 = mKeyFrames[k2];

					float t = (_time - key1.time) / (key2.time - key1.time);

					_value = key1.value.Lerp(key2.value, t);
				}
				else
				{
					_value = mKeyFrames[k1].value;
				}

				return true;
			}

			return false;
		}

		void Copy(TKeyController * rk)
		{
			Clear();

			for (int i = 0; i < rk->mKeyFrames.Size(); ++i)
			{
				mKeyFrames.PushBack(rk->mKeyFrames[i]);
			}
		}

		void Adjust()
		{
			KF_Compare op;

			if (mKeyFrames.Size() > 0)
			{
				Sort(&mKeyFrames[0], mKeyFrames.Size(), op);
			}
		}

		void Serialize(Serializer & sl)
		{
			if (sl.IsOut())
			{
				OSerializer & OS = (OSerializer &)sl;

				OS << mKeyFrames.Size();
				for (int i = 0; i < mKeyFrames.Size(); ++i)
				{
					const KF & kf = mKeyFrames[i];

					OS << kf.time;
					OS << kf.value;
				}
			}
			else
			{
				ISerializer & IS = (ISerializer &)sl;

				int count = 0;
				IS >> count;
				for (int i = 0; i < count; ++i)
				{
					KF kf;

					IS >> kf.time;
					IS >> kf.value;

					mKeyFrames.PushBack(kf);
				}
			}
		}

		void SetKeyFrame(int _i, float time, const IObject * val)
		{
			mKeyFrames[_i].time = time;

			if (val)
			{
				mKeyFrames[_i].value.Copy(val);
			}
		}

		void GetKeyFrame(int _i, float * time, IObject ** val)
		{
			if (time != NULL)
				*time = mKeyFrames[_i].time;

			if (val != NULL)
				*val = &mKeyFrames[_i].value;
		}

		KF & operator [](int _i)
		{
			return mKeyFrames[_i];
		}

	protected:
		Array<KF> mKeyFrames;
	};

	//
	template <class KF>
	class KF_Animation
	{
	public:
		KF_Animation() : mLooped(false), mPosition(0), mKeyIndex(0) {}
		~KF_Animation() {}

		TKeyController<KF> * GetKeyController()
		{ 
			return &mKeyFrame;
		}

		void  SetLooped(bool looped)
		{
			mLooped = looped;
		}

		bool GetLooped()
		{ 
			return mLooped;
		}

		float GetLength()
		{
			return mKeyFrame.Size() > 0 ? mKeyFrame[mKeyFrame.Size() - 1].time : 0;
		}

		bool IsEOF()
		{
			return !mLooped && mPosition == GetLength();
		}

		void SetPosition(float position)
		{ 
			mPosition = position; 
			mKeyIndex = 0;
		}

		float GetPosition() 
		{ 
			return mPosition; 
		}

		int GetKeyIndex()
		{
			return mKeyIndex;
		}

		bool Update(KF & kf, float elapsedTime, bool lerp = true)
		{
			float length = GetLength();
			if (length > 0)
			{
				mPosition += elapsedTime;

				if (mPosition >= length)
				{
					if (mLooped)
					{
						while (mPosition > length)
							mPosition -= length;

						mKeyIndex = 0;
					}
					else
					{
						mPosition = length;
					}
				}

				return mKeyFrame.GetValue(kf, mPosition, lerp, mKeyIndex);
			}

			return false;
		}

	protected:
		TKeyController<KF> mKeyFrame;
		bool mLooped;
		float mPosition;
		int mKeyIndex;
	};


	//
#define DECLARE_KEYVALUE(className) \
	void operator =(const className & rk)		\
	{											\
		data = rk.data;							\
	}											\
	\
	className Lerp(const className & rk, float t) const \
	{											\
		className v;							\
		v.data = data + (rk.data - data) * t;	\
		return v;								\
	}											\
	\
	ISerializer & operator >>(ISerializer & IS) \
	{											\
		IS >> data;								\
		return IS;								\
	}											\
	\
	OSerializer & operator <<(OSerializer & OS) \
	{											\
		OS << data;								\
		return OS;								\
	}

	//
	struct M_ENTRY KF_Float : public IObject
	{
		DECLARE_PROPERTY(IObject);
		DECLARE_KEYVALUE(KF_Float);

		float data;

		KF_Float() : data(0) {}
	};

	typedef TKeyController<KF_Float> KeyControllerFloat;

	//
	struct M_ENTRY KF_Float2 : public IObject
	{
		DECLARE_PROPERTY(IObject);
		DECLARE_KEYVALUE(KF_Float2);

		Float2 data;
	};

	typedef TKeyController<Float2> KeyControllerFloat2;

	//
	struct M_ENTRY KF_Float3 : public IObject
	{
		DECLARE_PROPERTY(IObject);
		DECLARE_KEYVALUE(KF_Float3);

		Float3 data;
	};

	typedef TKeyController<KF_Float3> KeyControllerFloat3;

	//
	struct M_ENTRY KF_Float4 : public IObject
	{
		DECLARE_PROPERTY(IObject);
		DECLARE_KEYVALUE(KF_Float4);

		Float4 data;
	};

	typedef TKeyController<KF_Float4> KeyControllerFloat4;

	//
	struct M_ENTRY KF_Color3 : public IObject
	{
		DECLARE_PROPERTY(IObject);
		DECLARE_KEYVALUE(KF_Color3);

		Float3 data;
	};

	typedef TKeyController<KF_Color3> KeyControllerColor3;

	struct M_ENTRY KF_Color4 : public IObject
	{
		DECLARE_PROPERTY(IObject);
		DECLARE_KEYVALUE(KF_Color4)

		Float4 data;
	};

	typedef TKeyController<KF_Color4> KeyControllerColor4;

	//
	struct M_ENTRY KF_Trasnform : public IObject
	{
		DECLARE_PROPERTY(IObject);

		Float3 position;
		Quat rotation;
		Float3 scale;

		KF_Trasnform Lerp(const KF_Trasnform & rk, float t) const
		{
			KF_Trasnform tm;

			tm.position = Float3::Lerp(position, rk.position, t);
			tm.rotation = Quat::Slerp(rotation, rk.rotation, t);
			tm.scale = Float3::Lerp(scale, rk.scale, t);

			return tm;
		}

		ISerializer & operator >>(ISerializer & IS)
		{
			IS >> position;
			IS >> rotation;
			IS >> scale;
			return IS;
		}

		OSerializer & operator <<(OSerializer & OS)
		{
			OS << position;
			OS << rotation;
			OS << scale;
			return OS;
		}
	};

	typedef TKeyController<KF_Trasnform> KeyControllerTransform;
}