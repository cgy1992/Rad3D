/*
*	KFAnimRect
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Component.h"

namespace Rad { namespace MGUI {

	struct MGUI_ENTRY KF_Rect : IObject
	{
		DECLARE_PROPERTY(IObject);

		Rect data;

		void operator =(const KF_Rect & rk)
		{
			data = rk.data;
		}

		KF_Rect Lerp(const KF_Rect & rk, float t) const
		{	
			KF_Rect v;
			v.data.x = data.x + (rk.data.x - data.x) * t;
			v.data.y = data.y + (rk.data.y - data.y) * t;
			v.data.w = data.w + (rk.data.w - data.w) * t;
			v.data.h = data.h + (rk.data.h - data.h) * t;

			return v;
		}

		void In(ISerializer & IS)
		{
			IS >> data;	
		}

		void Out(OSerializer & OS) const
		{
			OS << data;	
		}
	};

	//
	class MGUI_ENTRY KFAnimRect : public Component
	{
		DECLARE_RTTI();

	public:
		tEvent2<Component *, int> E_KeyChanged;
		tEvent1<Component *> E_KeyEnd;

	public:
		KFAnimRect();
		virtual ~KFAnimRect();

		virtual int
			Update(float elapsedTime);

		void
			Append(float time, const Rect & rect);
		
		KF_Animation<KF_Rect> *
			GetAnimation() { return &mAnimation; }

	protected:
		KF_Animation<KF_Rect> mAnimation;
		int mKeyIndex;
	};

}}