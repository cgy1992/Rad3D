#include "MGUI_KFAnimScale.h"
#include "MGUI_Widget.h"
#include "MGUI_Helper.h"

namespace Rad { namespace MGUI {

	ImplementRTTI(KFAnimScale, Component);

	KFAnimScale::KFAnimScale()
	{
		mKeyIndex = 0;
	}

	KFAnimScale::~KFAnimScale()
	{
	}

	int KFAnimScale::Update(float elapsedTime)
	{
		KF_Float kf;
		if (mAnimation.Update(kf, elapsedTime))
		{
			GetWidget()->SetScale(kf.data);

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

	void KFAnimScale::Append(float time, float scale)
	{
		KF_Float kf;
		kf.data = scale;

		mAnimation.GetKeyController()->Append(time, kf);
	}

}}