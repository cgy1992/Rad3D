#include "MGUI_ScrollBar.h"
#include "MGUI_Engine.h"

namespace Rad { namespace MGUI {

	ImplementRTTI(ScrollBarH, Widget);

	ScrollBarH::ScrollBarH(const LookFeel * _lookfeel, Widget * _parent)
		: Widget(_lookfeel, _parent)
		, mMouseOldX(0)
		, mDragPosition(0)
		, mScrollRatio(0)
		, mPosition(0)
		, mMinValue(0)
		, mMaxValue(10)
		, mStepValue(1)
	{
		d_assert (_lookfeel != NULL);

		mBnScrollLeft = new Button(_lookfeel->GetChild("ScrollLeft"), this);
		mBnScrollRight = new Button(_lookfeel->GetChild("ScrollRight"), this);
		mBnScroll = new Button(_lookfeel->GetChild("ScrollTrack"), this);

		mBnScrollLeft->SetAlign(eAlign::LEFT | eAlign::V_CENTER);
		mBnScrollRight->SetAlign(eAlign::RIGHT | eAlign::V_CENTER);
		mBnScroll->SetAlign(eAlign::V_CENTER);

		mBnScrollLeft->E_MouseClick += new cListener1<ScrollBarH, const MouseEvent *>(this, &ScrollBarH::OnScrollLeftClick);
		mBnScrollRight->E_MouseClick += new cListener1<ScrollBarH, const MouseEvent *>(this, &ScrollBarH::OnScrollRightClick);
		mBnScroll->E_MouseDrag += new cListener1<ScrollBarH, const MouseEvent *>(this, &ScrollBarH::OnScrollDrag);
		mBnScroll->E_MouseDown += new cListener1<ScrollBarH, const MouseEvent *>(this, &ScrollBarH::OnScrollPress);
		mBnScroll->E_MouseUp += new cListener1<ScrollBarH, const MouseEvent *>(this, &ScrollBarH::OnScrollRelease);
	}

	ScrollBarH::~ScrollBarH()
	{
	}

	void ScrollBarH::SetRange(float _min, float _max)
	{
		d_assert (_min <= _max);
		mMinValue = _min;
		mMaxValue = _max;
		mPosition = Math::Clamp(mPosition, mMinValue, mMaxValue);
	}

	void ScrollBarH::SetPosition(float position)
	{
		position = Math::Clamp(position, mMinValue, mMaxValue);

		if (mPosition != position)
		{
			mPosition = position;

			Event e(&E_PositionChanged);
			e.sender = this;

			E_PositionChanged(&e, position);
		}
	}

	void ScrollBarH::OnUpdate(float elapsedTime)
	{
		const Rect & clRect = GetClient();

		mBnScrollLeft->_updateTM();
		mBnScrollRight->_updateTM();

		float cw = clRect.w, ch = clRect.h;

		Rect myRectLeft, myRectRight, myRectScroll;

		myRectLeft = mBnScrollLeft->GetRect();
		myRectRight = mBnScrollRight->GetRect();

		myRectScroll.y = 0;
		myRectScroll.h = mBnScroll->GetRect().h;

		if (myRectLeft.x + myRectLeft.w >= myRectRight.x)
		{
			mBnScroll->SetVisible(false);
		}
		else
		{
			mBnScroll->SetVisible(true);

			const LookFeel * scrollLookFeel = (const LookFeel *)mBnScroll->GetLookFeel();

			float minWidth = scrollLookFeel->GetUVRect(eWidgetState::NORMAL).w;
			float maxWidth = myRectRight.x - myRectLeft.h;
			float x = myRectLeft.x + myRectLeft.w;
			float k = Min(1.0f, maxWidth / (mMaxValue - mMinValue));

			if (mMaxValue - mMinValue < 1)
			{
				myRectScroll.x = x + 0;
				myRectScroll.w = maxWidth;
				mScrollRatio = 0;
			}
			else
			{
				float length = maxWidth - (mMaxValue - mMinValue) * k;
				length = Max(minWidth, length);

				mScrollRatio = (mMaxValue - mMinValue) / (maxWidth - length);

				myRectScroll.x = x + (mPosition - mMinValue) / (mMaxValue - mMinValue) * (maxWidth - length);
				myRectScroll.w = length;
			}
		}

		mBnScroll->SetRect(myRectScroll);
	}

	void ScrollBarH::OnScrollLeftClick(const MouseEvent * e)
	{
		SetPosition(mPosition - mStepValue * mScrollRatio);
	}

	void ScrollBarH::OnScrollRightClick(const MouseEvent * e)
	{
		SetPosition(mPosition + mStepValue * mScrollRatio);
	}

	void ScrollBarH::OnScrollDrag(const MouseEvent * e)
	{
		SetPosition(mDragPosition + (e->x - mMouseOldX) * mScrollRatio);
	}

	void ScrollBarH::OnScrollPress(const MouseEvent * e)
	{
		if (e->id == InputCode::MKC_LEFT)
		{
			mMouseOldX = e->x;
			mDragPosition = mPosition;
		}
	}

	void ScrollBarH::OnScrollRelease(const MouseEvent * e)
	{
		if (e->id == InputCode::MKC_LEFT)
		{
			mMouseOldX = e->x;
			mDragPosition = 0;
		}
	}


	//
	ImplementRTTI(ScrollBarV, Widget);

	ScrollBarV::ScrollBarV(const LookFeel * _lookfeel, Widget * _parent)
		: Widget(_lookfeel, _parent)
		, mMouseOldY(0)
		, mDragPosition(0)
		, mPosition(0)
		, mMinValue(0)
		, mMaxValue(10)
		, mScrollRatio(0)
		, mStepValue(1)
	{
		d_assert (_lookfeel != NULL);

		mBnScrollUp = new Button(_lookfeel->GetChild("ScrollUp"), this);
		mBnScrollDown = new Button(_lookfeel->GetChild("ScrollDown"), this);
		mBnScroll = new Button(_lookfeel->GetChild("ScrollTrack"), this);

		mBnScrollUp->SetAlign(eAlign::H_CENTER | eAlign::TOP);
		mBnScrollDown->SetAlign(eAlign::H_CENTER | eAlign::BOTTOM);
		mBnScroll->SetAlign(eAlign::H_CENTER);

		mBnScrollUp->E_MouseClick += new cListener1<ScrollBarV, const MouseEvent *>(this, &ScrollBarV::OnScrollUpClick);
		mBnScrollDown->E_MouseClick += new cListener1<ScrollBarV, const MouseEvent *>(this, &ScrollBarV::OnScrollDownClick);
		mBnScroll->E_MouseDrag += new cListener1<ScrollBarV, const MouseEvent *>(this, &ScrollBarV::OnScrollDrag);
		mBnScroll->E_MouseDown += new cListener1<ScrollBarV, const MouseEvent *>(this, &ScrollBarV::OnScrollPress);
		mBnScroll->E_MouseUp += new cListener1<ScrollBarV, const MouseEvent *>(this, &ScrollBarV::OnScrollRelease);
	}

	ScrollBarV::~ScrollBarV()
	{
	}

	void ScrollBarV::SetRange(float _min, float _max)
	{
		d_assert (_min <= _max);
		mMinValue = _min;
		mMaxValue = _max;
		mPosition = Math::Clamp(mPosition, mMinValue, mMaxValue);
	}

	void ScrollBarV::SetPosition(float position)
	{
		position = Math::Clamp(position, mMinValue, mMaxValue);

		if (mPosition != position)
		{
			mPosition = position;

			Event e(&E_PositionChanged);
			e.sender = this;

			E_PositionChanged(&e, position);
		}
	}

	void ScrollBarV::OnUpdate(float elapsedTime)
	{
		mBnScrollUp->_updateTM();
		mBnScrollDown->_updateTM();

		const Rect & clRect = GetClient();

		float cw = clRect.w, ch = clRect.h;

		Rect myRectUp, myRectDown, myRectScroll;

		myRectUp = mBnScrollUp->GetRect();
		myRectDown = mBnScrollDown->GetRect();

		myRectScroll.x = 0;
		myRectScroll.w = mBnScroll->GetRect().w;

		if (myRectUp.y + myRectUp.h >= myRectDown.y)
		{
			mBnScroll->SetVisible(false);
		}
		else
		{
			mBnScroll->SetVisible(true);

			const LookFeel * scrollLookFeel = (const LookFeel *)mBnScroll->GetLookFeel();

			float minHeight = scrollLookFeel->GetUVRect(eWidgetState::NORMAL).h;
			float maxHeight = myRectDown.y - myRectUp.h;
			float y = myRectUp.y + myRectUp.h;
			float k = Min(1.0f, maxHeight / (mMaxValue - mMinValue));

			if (mMaxValue - mMinValue < 1)
			{
				myRectScroll.y = y + 0;
				myRectScroll.h = maxHeight;
				mScrollRatio = 0;
			}
			else
			{
				float length = maxHeight - (mMaxValue - mMinValue) * k;
				length = Max(minHeight, length);

				mScrollRatio = (mMaxValue - mMinValue) / (maxHeight - length);

				myRectScroll.y = y + (mPosition - mMinValue) / (mMaxValue - mMinValue) * (maxHeight - length);
				myRectScroll.h = length;
			}
		}

		mBnScroll->SetRect(myRectScroll);

		mBnScroll->_updateTM();

		int i = 0;
	}

	void ScrollBarV::OnScrollUpClick(const MouseEvent * e)
	{
		SetPosition(mPosition - mStepValue * mScrollRatio);
	}

	void ScrollBarV::OnScrollDownClick(const MouseEvent * e)
	{
		SetPosition(mPosition + mStepValue * mScrollRatio);
	}

	void ScrollBarV::OnScrollDrag(const MouseEvent * e)
	{
		SetPosition(mDragPosition + (e->y - mMouseOldY) * mScrollRatio);
	}

	void ScrollBarV::OnScrollPress(const MouseEvent * e)
	{
		if (e->id == InputCode::MKC_LEFT)
		{
			mMouseOldY = e->y;
			mDragPosition = mPosition;
		}
	}

	void ScrollBarV::OnScrollRelease(const MouseEvent * e)
	{
		if (e->id == InputCode::MKC_LEFT)
		{
			mMouseOldY = e->y;
			mDragPosition = 0;
		}
	}

}}