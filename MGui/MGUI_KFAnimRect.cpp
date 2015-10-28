#include "MGUI_KFAnimRect.h"
#include "MGUI_Widget.h"

namespace Rad { namespace MGUI {

	DF_PROPERTY_BEGIN(KF_Rect)
		DF_PROPERTY(KF_Rect, data, "", "", PT_Float4)
	DF_PROPERTY_END()
	
	ImplementRTTI(KFAnimRect, Component);

	KFAnimRect::KFAnimRect()
	{
		mKeyIndex = 0;
	}

	KFAnimRect::~KFAnimRect()
	{
	}

	int KFAnimRect::Update(float elapsedTime)
	{
		KF_Rect kf;
		if (mAnimation.Update(kf, elapsedTime))
		{
			GetWidget()->SetRect(kf.data);

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

	void KFAnimRect::Append(float time, const Rect & rect)
	{
		KF_Rect kf;
		kf.data = rect;

		mAnimation.GetKeyController()->Append(time, kf);
	}

}}