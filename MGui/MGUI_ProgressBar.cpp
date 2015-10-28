#include "MGUI_ProgressBar.h"
#include "MGUI_RenderItem.h"
#include "MGUI_Layout.h"
#include "MGUI_Helper.h"

namespace Rad { namespace MGUI {

	Progress::Progress()
		: mPercentage(100)
	{
	}

	Progress::~Progress()
	{
	}

	void Progress::SetPercentage(int _percentage)
	{
		mPercentage = _percentage;
		mPercentage = Math::Clamp(mPercentage, 0, 100);
	}

	int Progress::GetPrecentage()
	{
		return mPercentage;
	}






	ImplementRTTI(ProgressBarH, Widget);

	ProgressBarH::ProgressBarH(const LookFeel * _lookfeel, Widget * _parent)
		: Widget(_lookfeel, _parent)
	{
	}

	ProgressBarH::~ProgressBarH()
	{
	}

	void ProgressBarH::UpdateRenderItem(Layout * _layout)
	{
		if (!mVisible)
			return ;

		const LookFeel * _lookfeel = mLookFeel;


		if (_lookfeel)
		{
			RenderItem * ri = _layout->GetRenderItem(GetAbsOrder(), mSkin.c_ptr());

			Rect clipRect = mParent->GetClipRect();
			const Rect & myRect = mAbsRect;
			const Rect & clRect = mAbsClient;
			const Rect & uvRect = Helper::Instance()->MapUVRect(_lookfeel->GetUVRect(mState), mSkin.c_ptr());
			const Rect & uvClientRect = Helper::Instance()->MapUVRect(_lookfeel->GetUVClientRect(mState), mSkin.c_ptr());
			const Float4 & myColor = this->GetAbsColor();

			Rect _rect = myRect, _uv = uvRect;
			_rect.w = myRect.w * (mPercentage / 100.0f);
			_uv.w = uvRect.w * (mPercentage / 100.0f);

			Rect _myRect, _uvRect;

			if (Helper::Instance()->Clip(_myRect, _uvRect, _rect, _uv, clipRect))
			{
				ri->AddQuad(Helper::Instance()->GetQuad(_myRect, _uvRect, myColor));
			}
		}

		for (int i = 0; i < mChildren.Size(); ++i)
		{
			Widget * child = GetChild(i);
			child->UpdateRenderItem(_layout);
		}
	}



	ImplementRTTI(ProgressBarV, Widget);

	ProgressBarV::ProgressBarV(const LookFeel * _lookfeel, Widget * _parent)
		: Widget(_lookfeel, _parent)
	{
	}

	ProgressBarV::~ProgressBarV()
	{
	}

	void ProgressBarV::UpdateRenderItem(Layout * _layout)
	{
		if (!mVisible)
			return ;

		const LookFeel * _lookfeel = mLookFeel;

		if (_lookfeel)
		{
			RenderItem * ri = _layout->GetRenderItem(GetAbsOrder(), mSkin.c_ptr());

			Rect clipRect = mParent->GetClipRect();
			const Rect & myRect = mAbsRect;
			const Rect & clRect = mAbsClient;
			const Rect & uvRect = Helper::Instance()->MapUVRect(_lookfeel->GetUVRect(mState), mSkin.c_ptr());
			const Float4 & myColor = this->GetAbsColor();

			Rect _rect = myRect, _uv = uvRect;
			_rect.h = myRect.h * (mPercentage / 100.0f);
			_uv.h = uvRect.h * (mPercentage / 100.0f);

			Rect _myRect, _uvRect;

			if (Helper::Instance()->Clip(_myRect, _uvRect, _rect, _uv, clipRect))
			{
				ri->AddQuad(Helper::Instance()->GetQuad(_myRect, _uvRect, myColor));
			}
		}

		for (int i = 0; i < mChildren.Size(); ++i)
		{
			Widget * child = GetChild(i);
			child->UpdateRenderItem(_layout);
		}
	}








	ImplementRTTI(HImageProgressBar, ImageBox);

	HImageProgressBar::HImageProgressBar(Widget * _parent)
		: ImageBox(NULL, _parent)
	{
	}

	HImageProgressBar::~HImageProgressBar()
	{
	}

	void HImageProgressBar::UpdateRenderItem(Layout * _layout)
	{
		if (!mVisible)
			return ;

		if (mSkin != NULL)
		{
			RenderItem * ri = _layout->GetRenderItem(GetAbsOrder(), mSkin.c_ptr());

			Rect clipRect = mParent->GetClipRect();
			const Rect & myRect = mAbsRect;
			const Rect & clRect = mAbsClient;
			const Rect & uvRect = mUVRect;
			const Float4 & myColor = this->GetAbsColor();

			Rect _rect = myRect, _uv = uvRect;
			_rect.w = myRect.w * (mPercentage / 100.0f);
			_uv.w = uvRect.w * (mPercentage / 100.0f);

			Rect _myRect, _uvRect;

			if (Helper::Instance()->Clip(_myRect, _uvRect, _rect, _uv, clipRect))
			{
				ri->AddQuad(Helper::Instance()->GetQuad(_myRect, _uvRect, myColor));
			}
		}

		for (int i = 0; i < mChildren.Size(); ++i)
		{
			Widget * child = GetChild(i);
			child->UpdateRenderItem(_layout);
		}
	}



	ImplementRTTI(VImageProgressBar, ImageBox);

	VImageProgressBar::VImageProgressBar(Widget * _parent)
		: ImageBox(NULL, _parent)
	{
	}

	VImageProgressBar::~VImageProgressBar()
	{
	}

	void VImageProgressBar::UpdateRenderItem(Layout * _layout)
	{
		if (!mVisible)
			return ;

		if (mSkin != NULL)
		{
			RenderItem * ri = _layout->GetRenderItem(GetAbsOrder(), mSkin.c_ptr());

			Rect clipRect = mParent->GetClipRect();
			const Rect & myRect = mAbsRect;
			const Rect & clRect = mAbsClient;
			const Rect & uvRect = mUVRect;
			const Float4 & myColor = this->GetAbsColor();

			Rect _rect = myRect, _uv = uvRect;
			_rect.h = myRect.h * (mPercentage / 100.0f);
			_uv.h = uvRect.h * (mPercentage / 100.0f);

			Rect _myRect, _uvRect;

			if (Helper::Instance()->Clip(_myRect, _uvRect, _rect, _uv, clipRect))
			{
				ri->AddQuad(Helper::Instance()->GetQuad(_myRect, _uvRect, myColor));
			}
		}

		for (int i = 0; i < mChildren.Size(); ++i)
		{
			Widget * child = GetChild(i);
			child->UpdateRenderItem(_layout);
		}
	}

	//
	ImplementRTTI(CircleProgressBar, Widget);

	CircleProgressBar::CircleProgressBar(const LookFeel * _lookfeel, Widget * _parent)
		: Widget(_lookfeel, _parent)
		, mPercentage(0)
	{
	}

	CircleProgressBar::~CircleProgressBar()
	{
	}

	void CircleProgressBar::SetPercentage(float _percentage)
	{
		mPercentage = _percentage;
		mPercentage = Math::Clamp(mPercentage, 0.0f, 100.0f);
	}

	void CircleProgressBar::UpdateRenderItem(Layout * _layout)
	{
		if (!mVisible)
			return ;

		const LookFeel * _lookfeel = mLookFeel;

		while (_lookfeel && mSkin != NULL)
		{
			RenderItem * ri = _layout->GetRenderItem(GetAbsOrder(), mSkin.c_ptr());

			const Rect & clipRect = GetParent()->GetClipRect();
			const Rect & rect = this->GetAbsRect();
			const Rect & uv = Helper::Instance()->MapUVRect(_lookfeel->GetUVRect(mState), mSkin.c_ptr());
			const Float4 & color = this->GetAbsColor();

			Rect _myRect, _uvRect;
			if (!Helper::Instance()->Clip(_myRect, _uvRect, rect, uv, clipRect))
				break;

			float part = mPercentage * 0.08f;

			float x0 = _myRect.x;
			float y0 = _myRect.y;
			float x1 = _myRect.x + _myRect.w;
			float y1 = _myRect.y + _myRect.h;

			float u0 = _uvRect.x;
			float v0 = _uvRect.y;
			float u1 = _uvRect.x + _uvRect.w;
			float v1 = _uvRect.y + _uvRect.h;

			float ox = _myRect.cx();
			float oy = _myRect.cy();
			float ou = _uvRect.cx();
			float ov = _uvRect.cy();
			Vertex geo[3];

			geo[0].r = color.r; geo[0].g = color.g; geo[0].b = color.b; geo[0].a = color.a;
			geo[1].r = color.r; geo[1].g = color.g; geo[1].b = color.b; geo[1].a = color.a;
			geo[2].r = color.r; geo[2].g = color.g; geo[2].b = color.b; geo[2].a = color.a;


			if (part > 0)
			{
				float d = Math::Clamp(part, 0.0f, 1.0f);

				float dx = Math::Lerp(ox, x1, d);
				float du = Math::Lerp(ou, u1, d);

				// v1
				geo[0].x = ox; geo[0].y = oy;
				geo[0].u = ou; geo[0].v = ov;

				// v2
				geo[1].x = ox; geo[1].y = y0;
				geo[1].u = ou; geo[1].v = v0;

				// v3
				geo[2].x = dx; geo[2].y = y0;
				geo[2].u = du; geo[2].v = v0;

				ri->AddTri(geo);
			}
			
			if (part > 1)
			{
				float d = Math::Clamp(part - 1, 0.0f, 1.0f);

				float dy = Math::Lerp(y0, oy, d);
				float dv = Math::Lerp(v0, ov, d);

				// v1
				geo[0].x = ox; geo[0].y = oy;
				geo[0].u = ou; geo[0].v = ov;

				// v2
				geo[1].x = x1; geo[1].y = y0;
				geo[1].u = u1; geo[1].v = v0;

				// v3
				geo[2].x = x1; geo[2].y = dy;
				geo[2].u = u1; geo[2].v = dv;

				ri->AddTri(geo);
			}
			
			if (part > 2)
			{
				float d = Math::Clamp(part - 2, 0.0f, 1.0f);

				float dy = Math::Lerp(oy, y1, d);
				float dv = Math::Lerp(ov, v1, d);

				geo[0].x = ox; geo[0].y = oy;
				geo[0].u = ou; geo[0].v = ov;

				geo[1].x = x1; geo[1].y = oy;
				geo[1].u = u1; geo[1].v = ov;
				
				geo[2].x = x1; geo[2].y = dy;
				geo[2].u = u1; geo[2].v = dv;

				ri->AddTri(geo);
			}

			if (part > 3)
			{
				float d = Math::Clamp(part - 3, 0.0f, 1.0f);

				float dx = Math::Lerp(x1, ox, d);
				float du = Math::Lerp(u1, ou, d);

				geo[0].x = ox; geo[0].y = oy;
				geo[0].u = ou; geo[0].v = ov;

				geo[1].x = x1; geo[1].y = y1;
				geo[1].u = u1; geo[1].v = v1;

				geo[2].x = dx; geo[2].y = y1;
				geo[2].u = du; geo[2].v = v1;

				ri->AddTri(geo);
			}

			if (part > 4)
			{
				float d = Math::Clamp(part - 4, 0.0f, 1.0f);

				float dx = Math::Lerp(ox, x0, d);
				float du = Math::Lerp(ou, u0, d);

				geo[0].x = ox; geo[0].y = oy;
				geo[0].u = ou; geo[0].v = ov;

				geo[1].x = ox; geo[1].y = y1;
				geo[1].u = ou; geo[1].v = v1;

				geo[2].x = dx; geo[2].y = y1;
				geo[2].u = du; geo[2].v = v1;

				ri->AddTri(geo);
			}

			if (part > 5)
			{
				float d = Math::Clamp(part - 5, 0.0f, 1.0f);

				float dy = Math::Lerp(y1, oy, d);
				float dv = Math::Lerp(v1, ov, d);

				geo[0].x = ox; geo[0].y = oy;
				geo[0].u = ou; geo[0].v = ov;

				geo[1].x = x0; geo[1].y = y1;
				geo[1].u = u0; geo[1].v = v1;

				geo[2].x = x0; geo[2].y = dy;
				geo[2].u = u0; geo[2].v = dv;

				ri->AddTri(geo);
			}

			if (part > 6)
			{
				float d = Math::Clamp(part - 6, 0.0f, 1.0f);

				float dy = Math::Lerp(oy, y0, d);
				float dv = Math::Lerp(ov, v0, d);

				geo[0].x = ox; geo[0].y = oy;
				geo[0].u = ou; geo[0].v = ov;

				geo[1].x = x0; geo[1].y = oy;
				geo[1].u = u0; geo[1].v = ov;

				geo[2].x = x0; geo[2].y = dy;
				geo[2].u = u0; geo[2].v = dv;

				ri->AddTri(geo);
			}

			if (part > 7)
			{
				float d = Math::Clamp(part - 7, 0.0f, 1.0f);

				float dx = Math::Lerp(x0, ox, d);
				float du = Math::Lerp(u0, ou, d);

				// v1
				geo[0].x = ox; geo[0].y = oy;
				geo[0].u = ou; geo[0].v = ov;

				// v2
				geo[1].x = x0; geo[1].y = y0;
				geo[1].u = u0; geo[1].v = v0;

				// v3
				geo[2].x = dx; geo[2].y = y0;
				geo[2].u = du; geo[2].v = v0;

				ri->AddTri(geo);
			}

			break;
		}

		for (int i = 0; i < mChildren.Size(); ++i)
		{
			Widget * child = GetChild(i);
			child->UpdateRenderItem(_layout);
		}
	}

}}