#include "MGUI_KFAnimSize.h"
#include "MGUI_Widget.h"

namespace Rad { namespace MGUI {

	ImplementRTTI(KFAnimSize, Component);

	KFAnimSize::KFAnimSize()
	{
		mKeyIndex = 0;
	}

	KFAnimSize::~KFAnimSize()
	{
	}

	int KFAnimSize::Update(float elapsedTime)
	{
		KF_Float2 kf;
		if (mAnimation.Update(kf, elapsedTime))
		{
			Rect myRect = GetWidget()->GetRect();
			myRect.w = kf.data.x;
			myRect.h = kf.data.y;

			GetWidget()->SetRect(myRect);

			if (mKeyIndex != mAnimation.GetKeyIndex())
			{
				mKeyIndex = mAnimation.GetKeyIndex();
				E_KeyChanged(this, mKeyIndex);
			}

			if (mAnimation.IsEOF())
			{
				E_KeyEnd(this);
				return -1;
			}

			return 0;
		}

		return -1;
	}

	void KFAnimSize::Append(float time, float w, float h)
	{
		KF_Float2 kf;
		kf.data.x = w;
		kf.data.y = h;

		mAnimation.GetKeyController()->Append(time, kf);
	}

}}