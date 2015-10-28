#include "MGUI_KFAnimMove.h"
#include "MGUI_Widget.h"

namespace Rad { namespace MGUI {

	ImplementRTTI(KFAnimMove, Component);

	KFAnimMove::KFAnimMove()
	{
		mKeyIndex = 0;
	}

	KFAnimMove::~KFAnimMove()
	{
	}

	void KFAnimMove::Attach(ComponentOwner * owner)
	{
		Component::Attach(owner);

		mRect = GetWidget()->GetRect();
	}

	void KFAnimMove::Detach()
	{
		Component::Detach();
	}

	int KFAnimMove::Update(float elapsedTime)
	{
		KF_Float2 kf;
		if (mAnimation.Update(kf, elapsedTime))
		{
			_move(kf.data.x, kf.data.y);

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

	void KFAnimMove::_move(float x, float y)
	{
		Widget * widget = GetWidget();

		float wx, wy;
		wx = mRect.x;
		wy = mRect.y;

		wx += x;
		wy += y;

		widget->SetRectPosition(wx, wy);
	}

	void KFAnimMove::Append(float time, float x, float y)
	{
		KF_Float2 kf;
		kf.data.x = x;
		kf.data.y = y;

		mAnimation.GetKeyController()->Append(time, kf);
	}
}}