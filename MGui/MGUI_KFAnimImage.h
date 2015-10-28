/*
*	KFAnimImage
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Component.h"

namespace Rad { namespace MGUI {

	class MGUI_ENTRY KFAnimImage : public Component
	{
		DECLARE_RTTI();

		struct KeyFrame 
		{
			float time;
			TexturePtr skin;
			Rect uv;
		};

	public:
		tEvent2<Component *, int> E_KeyChanged;
		tEvent1<Component *> E_KeyEnd;

	public:
		KFAnimImage();
		virtual ~KFAnimImage();

		virtual void 
			Attach(ComponentOwner * owner);

		virtual int
			Update(float elapsedTime);

		void 
			SetLoop(bool loop);
		void 
			AddKeyFrame(float time, const String & skin, const Rect & uv = Rect(0, 0, 1, 1));
		float 
			GetLength();

	protected:
		Array<KeyFrame> mKeyFrames;
		bool mLooped;
		float mInternalTime;
		int mLastKey;
	};

}}