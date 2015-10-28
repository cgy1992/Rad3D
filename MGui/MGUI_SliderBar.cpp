#include "MGUI_SliderBar.h"
#include "MGUI_Engine.h"

namespace Rad { namespace MGUI {

	ImplementRTTI(SliderBarH, Widget);

	SliderBarH::SliderBarH(const LookFeel * _lookfeel, Widget * _parent)
		: Widget(_lookfeel, _parent)
		, mMouseX(0)
		, mMinValue(0)
		, mMaxValue(10)
		, mPosition(0)
	{
		mBnSlider = new Button(_lookfeel->GetChild("SliderTrack"), this);
		mBnSlider->SetAlign(eAlign::V_CENTER);

		mBnSlider->E_MouseDown += new cListener1<SliderBarH, const MouseEvent *>(this, &SliderBarH::OnMouseDown);
		mBnSlider->E_MouseUp += new cListener1<SliderBarH, const MouseEvent *>(this, &SliderBarH::OnMouseUp);
		mBnSlider->E_MouseDrag += new cListener1<SliderBarH, const MouseEvent *>(this, &SliderBarH::OnMouseDrag);

		mBnSlider->E_TouchDown += new cListener1<SliderBarH, const TouchEvent *>(this, &SliderBarH::OnTouchDown);
		mBnSlider->E_TouchUp += new cListener1<SliderBarH, const TouchEvent *>(this, &SliderBarH::OnTouchUp);
		mBnSlider->E_TouchMove += new cListener1<SliderBarH, const TouchEvent *>(this, &SliderBarH::OnTouchMove);
	}

	SliderBarH::~SliderBarH()
	{
	}

	void SliderBarH::SetRange(int _min, int _max)
	{
		d_assert (_min <= _max);
		mMinValue = _min;
		mMaxValue = _max;
		mPosition = Math::Clamp(mPosition, mMinValue, mMaxValue);
	}

	void SliderBarH::SetPosition(int position)
	{
		position = Math::Clamp(position, mMinValue, mMaxValue);

		if (mPosition != position)
		{
			mPosition = position;

			const Rect & rect = mBnSlider->GetRect();
			const Rect & client = GetClient();
			float k = float(position - mMinValue) / (mMaxValue - mMinValue);
			
			mBnSlider->SetRectPosition(k * (client.w - rect.w), 0);

			Event e(&E_PositionChanged);
			e.sender = this;

			E_PositionChanged(&e, position);
		}
	}

	void SliderBarH::OnMouseDrag(const MouseEvent * e)
	{
		OnMove(e->x - mMouseX);
	}

	void SliderBarH::OnMouseDown(const MouseEvent * e)
	{
		if (e->id == InputCode::MKC_LEFT)
			mMouseX = e->x;
	}

	void SliderBarH::OnMouseUp(const MouseEvent * e)
	{
		if (e->id == InputCode::MKC_LEFT)
			mMouseX = 0;
	}

	void SliderBarH::OnTouchMove(const TouchEvent * e)
	{
		if (e->id == 0)
			OnMove(e->x - mMouseX);
	}

	void SliderBarH::OnTouchDown(const TouchEvent * e)
	{
		if (e->id == 0)
			mMouseX = e->x;
	}

	void SliderBarH::OnTouchUp(const TouchEvent * e)
	{
		if (e->id == 0)
			mMouseX = 0;
	}

	void SliderBarH::OnMove(float dx)
	{
		const Rect & rect = mBnSlider->GetRect();
		const Rect & client = GetClient();

		float max_x = client.w - rect.w;
		float x = Math::Clamp(rect.x + dx, 0.0f, max_x);
		int pos = (int)(mMinValue + (mMaxValue - mMinValue) * ((x + 0.1f) / max_x));

		SetPosition(pos);

		mBnSlider->SetRectPosition(x, 0);
		mMouseX = 0;
	}


	//
	ImplementRTTI(SliderBarV, Widget);

	SliderBarV::SliderBarV(const LookFeel * _lookfeel, Widget * _parent)
		: Widget(_lookfeel, _parent)
		, mMouseY(0)
		, mMinValue(0)
		, mMaxValue(10)
		, mPosition(0)
	{
		mBnSlider = new Button(_lookfeel->GetChild("SliderTrack"), this);
		mBnSlider->SetAlign(eAlign::V_CENTER);

		mBnSlider->E_MouseDown += new cListener1<SliderBarV, const MouseEvent *>(this, &SliderBarV::OnMouseDown);
		mBnSlider->E_MouseUp += new cListener1<SliderBarV, const MouseEvent *>(this, &SliderBarV::OnMouseUp);
		mBnSlider->E_MouseDrag += new cListener1<SliderBarV, const MouseEvent *>(this, &SliderBarV::OnMouseDrag);

		mBnSlider->E_TouchDown += new cListener1<SliderBarV, const TouchEvent *>(this, &SliderBarV::OnTouchDown);
		mBnSlider->E_TouchUp += new cListener1<SliderBarV, const TouchEvent *>(this, &SliderBarV::OnTouchUp);
		mBnSlider->E_TouchMove += new cListener1<SliderBarV, const TouchEvent *>(this, &SliderBarV::OnTouchMove);
	}

	SliderBarV::~SliderBarV()
	{
	}

	void SliderBarV::SetRange(int _min, int _max)
	{
		d_assert (_min <= _max);
		mMinValue = _min;
		mMaxValue = _max;
		mPosition = Math::Clamp(mPosition, mMinValue, mMaxValue);
	}

	void SliderBarV::SetPosition(int position)
	{
		position = Math::Clamp(position, mMinValue, mMaxValue);

		if (mPosition != position)
		{
			mPosition = position;

			const Rect & rect = mBnSlider->GetRect();
			const Rect & client = GetClient();
			float k = float(position - mMinValue) / (mMaxValue - mMinValue);

			mBnSlider->SetRectPosition(k * (client.h - rect.h), 0);

			Event e(&E_PositionChanged);
			e.sender = this;

			E_PositionChanged(&e, position);
		}
	}

	void SliderBarV::OnMouseDrag(const MouseEvent * e)
	{
		OnMove(e->y - mMouseY);
	}

	void SliderBarV::OnMouseDown(const MouseEvent * e)
	{
		if (e->id == InputCode::MKC_LEFT)
			mMouseY = e->y;
	}

	void SliderBarV::OnMouseUp(const MouseEvent * e)
	{
		if (e->id == InputCode::MKC_LEFT)
			mMouseY = 0;
	}

	void SliderBarV::OnTouchMove(const TouchEvent * e)
	{
		if (e->id == 0)
			OnMove(e->y - mMouseY);
	}

	void SliderBarV::OnTouchDown(const TouchEvent * e)
	{
		if (e->id == 0)
			mMouseY = e->y;
	}

	void SliderBarV::OnTouchUp(const TouchEvent * e)
	{
		if (e->id == 0)
			mMouseY = 0;
	}

	void SliderBarV::OnMove(float dy)
	{
		const Rect & rect = mBnSlider->GetRect();
		const Rect & client = GetClient();

		float max_y = client.h - rect.h;
		float y = Math::Clamp(rect.y + dy, 0.0f, max_y);
		int pos = (int)(mMinValue + (mMaxValue - mMinValue) * ((y + 0.1f) / max_y));

		SetPosition(pos);

		mBnSlider->SetRectPosition(0, y);
		mMouseY = 0;
	}

}}