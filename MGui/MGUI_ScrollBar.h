/*
*	ScrollBar
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Widget.h"
#include "MGUI_Button.h"

namespace Rad { namespace MGUI {

	class MGUI_ENTRY ScrollBarH : public Widget
	{
		DECLARE_RTTI();

	public:
		tEvent2<const Event *, float> E_PositionChanged;

	public:
		ScrollBarH(const LookFeel * _lookfeel, Widget * _parent);
		virtual ~ScrollBarH();

		virtual void 
			OnUpdate(float elapsedTime);

		void 
			SetRange(float _min, float _max);
		void 
			SetPosition(float position);

		float 
			GetMinValue() { return mMinValue; }
		float 
			GetMaxValue() { return mMaxValue; }
		float 
			GetPosition() { return mPosition; }

		void 
			SetStepValue(float _value) { mStepValue = _value; }
		float 
			GetStepValue() { return mStepValue; }


	protected:
		void 
			OnScrollLeftClick(const MouseEvent * e);
		void 
			OnScrollRightClick(const MouseEvent * e);
		void 
			OnScrollDrag(const MouseEvent * e);
		void 
			OnScrollPress(const MouseEvent * e);
		void 
			OnScrollRelease(const MouseEvent * e);

	protected:
		Button * mBnScrollLeft;
		Button * mBnScrollRight;
		Button * mBnScroll;

		float mMinValue;
		float mMaxValue;
		float mPosition;
		float mStepValue;

		float mMouseOldX;
		float mDragPosition;
		float mScrollRatio;
	};


	class MGUI_ENTRY ScrollBarV : public Widget
	{
		DECLARE_RTTI();

	public:
		tEvent2<const Event *, float> E_PositionChanged;

	public:
		ScrollBarV(const LookFeel * _lookfeel, Widget * _parent);
		virtual ~ScrollBarV();

		virtual void 
			OnUpdate(float elapsedTime);

		void 
			SetRange(float _min, float _max);
		void 
			SetPosition(float position);

		float 
			GetMinValue() { return mMinValue; }
		float 
			GetMaxValue() { return mMaxValue; }
		float 
			GetPosition() { return mPosition; }

		void 
			SetStepValue(float _value) { mStepValue = _value; }
		float 
			GetStepValue() { return mStepValue; }

	protected:
		void 
			OnScrollUpClick(const MouseEvent * e);
		void 
			OnScrollDownClick(const MouseEvent * e);
		void 
			OnScrollDrag(const MouseEvent * e);
		void 
			OnScrollPress(const MouseEvent * e);
		void 
			OnScrollRelease(const MouseEvent * e);

	protected:
		Button * mBnScrollUp;
		Button * mBnScrollDown;
		Button * mBnScroll;

		float mMinValue;
		float mMaxValue;
		float mPosition;
		float mStepValue;

		float mMouseOldY;
		float mDragPosition;
		float mScrollRatio;
	};

}}
