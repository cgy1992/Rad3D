#include "MGUI_KFAnimColor.h"
#include "MGUI_Widget.h"

namespace Rad { namespace MGUI {

	ImplementRTTI(KFAnimColor, Component);

	KFAnimColor::KFAnimColor()
	{
		mKeyIndex = 0;
	}

	KFAnimColor::~KFAnimColor()
	{
	}

	int KFAnimColor::Update(float elapsedTime)
	{
		KF_Float4 kf;
		if (mAnimation.Update(kf, elapsedTime))
		{
			GetWidget()->SetColor(kf.data);
			
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

	void KFAnimColor::Append(float time, const Float4 & color)
	{
		KF_Float4 kf;
		kf.data = color;

		mAnimation.GetKeyController()->Append(time, kf);
	}

}}