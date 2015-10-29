#include "TrackerControl.h"

TrackerControl::TrackerControl(int steps, float minLength, float maxLength, MGUI::Widget * _parent)
	: MGUI::Widget(NULL, _parent)
	, mSteps(steps)
	, mMinLength(minLength)
	, mMaxLength(maxLength)
{
	d_assert ((mSteps % 5) == 0);
}

TrackerControl::~TrackerControl()
{
}

void TrackerControl::UpdateRenderItem(MGUI::Layout * _layout)
{
	if (!mVisible)
		return ;

	MGUI::RenderItem * ri = _layout->GetRenderItem(GetAbsOrder(), MGUI::Helper::Instance()->GetWhiteTexture().c_ptr());

	MGUI::Rect clipRect = mParent->GetClipRect();
	const MGUI::Rect & myRect = this->GetAbsRect();
	MGUI::Rect rc;
	rc.x = myRect.x;
	rc.y = myRect.y + myRect.h - 3;
	rc.w = myRect.w;
	rc.h = 3;

	MGUI::Rect uv;
	uv.x = 0;
	uv.y = 0;
	uv.w = 1;
	uv.h = 1;

	MGUI::Rect _rect, _uv;
	if (MGUI::Helper::Instance()->Clip(_rect, _uv, rc, uv, clipRect))
	{
		ri->AddQuad(MGUI::Helper::Instance()->GetQuad(_rect, _uv, mColor));
	}

	float sx = myRect.x + 2, ex = myRect.x + myRect.w - 2;
	float w = ex - sx;
	float step = w / mSteps;

	for (int i = 0; i < mSteps + 1; ++i)
	{
		float x = sx + step * i;
		float h = (i % 5) == 0 ? mMaxLength : mMinLength;

		rc.x = x - 1;
		rc.y = myRect.y + myRect.h - 3 - h;
		rc.w = 3;
		rc.h = h;

		if (MGUI::Helper::Instance()->Clip(_rect, _uv, rc, uv, clipRect))
		{
			ri->AddQuad(MGUI::Helper::Instance()->GetQuad(_rect, _uv, mColor));
		}
	}

	for (int i = 0; i < mChildren.Size(); ++i)
	{
		MGUI::Widget * child = GetChild(i);
		child->UpdateRenderItem(_layout);
	}
}

float TrackerControl::MapUnit(float abs_x)
{
	const MGUI::Rect & myRect = this->GetAbsRect();

	float sx = myRect.x + 2, ex = myRect.x + myRect.w - 2;
	float w = ex - sx;

	float k = w > 0 ? (abs_x - sx) / w : 0;

	return Math::Clamp(k, 0.0f, 1.0f);
}



TrackerPointer::TrackerPointer(MGUI::Widget * _parent)
	: MGUI::CheckBox(NULL, _parent)
{
	STATE_COLOR[MGUI::eWidgetState::NORMAL] = Float4(0.6f, 0.6f, 0);
	STATE_COLOR[MGUI::eWidgetState::DISABLE] = Float4(0, 0, 0);
	STATE_COLOR[MGUI::eWidgetState::FOCUS] = Float4(1.0f, 1.0f, 0);
	STATE_COLOR[MGUI::eWidgetState::PRESS] = Float4(0.0f, 1.0f, 0);
	STATE_COLOR[MGUI::eWidgetState::SELECT] = Float4(0.0f, 0.8f, 0);
	STATE_COLOR[MGUI::eWidgetState::SELECT_DISABLE] = Float4(0, 0, 0);
	STATE_COLOR[MGUI::eWidgetState::SELECT_FOCUS] = Float4(0.0f, 1.0f, 0);
	STATE_COLOR[MGUI::eWidgetState::SELECT_PRESS] = Float4(0.0f, 1.0f, 0);
}

TrackerPointer::~TrackerPointer()
{
}

void TrackerPointer::UpdateRenderItem(MGUI::Layout * _layout)
{
	if (!mVisible)
		return ;

	MGUI::RenderItem * ri = _layout->GetRenderItem(GetAbsOrder(), MGUI::Helper::Instance()->GetWhiteTexture().c_ptr());
	MGUI::Rect clipRect = mParent->GetClipRect();
	Float4 color = STATE_COLOR[mState];

	MGUI::Rect uv;
	uv.x = 0;
	uv.y = 0;
	uv.w = 1;
	uv.h = 1;

	MGUI::Rect _rect, _uv;
	if (MGUI::Helper::Instance()->Clip(_rect, _uv, this->GetAbsRect(), uv, clipRect))
	{
		ri->AddQuad(MGUI::Helper::Instance()->GetQuad(_rect, _uv, color));
	}

	for (int i = 0; i < mChildren.Size(); ++i)
	{
		MGUI::Widget * child = GetChild(i);
		child->UpdateRenderItem(_layout);
	}
}