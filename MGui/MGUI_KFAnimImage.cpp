#include "MGUI_KFAnimImage.h"
#include "MGUI_ImageBox.h"

namespace Rad { namespace MGUI {

	ImplementRTTI(KFAnimImage, Component);

	KFAnimImage::KFAnimImage()
		: mLooped(false)
		, mInternalTime(0)
		, mLastKey(-1)
	{
	}

	KFAnimImage::~KFAnimImage()
	{
	}

	void KFAnimImage::Attach(ComponentOwner * owner)
	{
		d_assert (KIND_OF(ImageBox, owner));

		Component::Attach(owner);
	}

	int KFAnimImage::Update(float elapsedTime)
	{
		if (mKeyFrames.Size())
		{
			ImageBox * pImageBox = (ImageBox *)GetWidget();

			float length = GetLength();
			mInternalTime += elapsedTime;

			if (mLooped && length > 0)
			{
				while (mInternalTime >= length)
					mInternalTime -= length;
			}

			if (mInternalTime < length)
			{
				int key = 0;

				if (mKeyFrames.Size() > 1 && mInternalTime > mKeyFrames[0].time)
				{
					for (int i = 1; i < mKeyFrames.Size(); ++i)
					{
						if (mKeyFrames[i].time > mInternalTime)
							break;

						key = i;
					}
				}

				if (mLastKey != key)
				{
					const KeyFrame & kf = mKeyFrames[key];

					pImageBox->SetSkin(kf.skin->GetName());
					pImageBox->SetUVRect(kf.uv);

					if (kf.skin != NULL)
					{
						int skinW = (int)(kf.skin->GetWidth() * kf.uv.w);
						int skinH = (int)(kf.skin->GetHeight() * kf.uv.h);
						Rect rect = pImageBox->GetRect();

						float cx = rect.x + rect.w / 2;
						float cy = rect.y + rect.h / 2;

						rect.x = cx - skinW / 2.0f;
						rect.y = cy - skinH / 2.0f;
						rect.w = (float)skinW;
						rect.h = (float)skinH;

						pImageBox->SetRect(rect);
					}

					mLastKey = key;
					E_KeyChanged(this, key);
				}
			}
			else
			{
				int key = mKeyFrames.Size() - 1;

				if (mLastKey != key)
				{
					const KeyFrame & kf = mKeyFrames[key];

					pImageBox->SetSkin(kf.skin->GetName());
					pImageBox->SetUVRect(kf.uv);

					if (kf.skin != NULL)
					{
						int skinW = (int)(kf.skin->GetWidth() * kf.uv.w);
						int skinH = (int)(kf.skin->GetHeight() * kf.uv.h);
						Rect rect = pImageBox->GetRect();

						float cx = rect.x + rect.w / 2;
						float cy = rect.y + rect.h / 2;

						rect.x = cx - skinW / 2.0f;
						rect.y = cy - skinH / 2.0f;
						rect.w = (float)skinW;
						rect.h = (float)skinH;

						pImageBox->SetRect(rect);
					}

					mLastKey = key;
					E_KeyChanged(this, key);
				}

				if (!mLooped)
				{
					E_KeyEnd(this);

					return -1;
				}
			}

			return 0;
		}

		return -1;
	}

	void KFAnimImage::AddKeyFrame(float time, const String & skin, const Rect & uv)
	{
		d_assert (time >= GetLength());

		KeyFrame frame = { time, HWBufferManager::Instance()->LoadTexture(skin, -1), uv};

		mKeyFrames.PushBack(frame);
	}

	void KFAnimImage::SetLoop(bool loop)
	{
		mLooped = loop;
	}

	float KFAnimImage::GetLength()
	{
		return mKeyFrames.Size() > 0 ? mKeyFrames[mKeyFrames.Size() - 1].time : 0;
	}

}}