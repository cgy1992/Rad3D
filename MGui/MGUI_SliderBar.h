/*
*	SliderBar
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Widget.h"
#include "MGUI_Button.h"

namespace Rad { namespace MGUI {

	class MGUI_ENTRY SliderBarH : public Widget
	{
		DECLARE_RTTI();

	public:
		tEvent2<const Event *, int> E_PositionChanged;

	public:
		SliderBarH(const LookFeel * _lookfeel, Widget * _parent);
		virtual ~SliderBarH();

		void 
			SetRange(int _min, int _max);
		void 
			SetPosition(int position);

		int 
			GetMinValue() { return mMinValue; }
		int 
			GetMaxValue() { return mMaxValue; }
		int 
			GetPosition() { return mPosition; }

	protected:
		void 
			OnMouseDrag(const MouseEvent * e);
		void 
			OnMouseDown(const MouseEvent * e);
		void 
			OnMouseUp(const MouseEvent * e);

		void 
			OnTouchMove(const TouchEvent * e);
		void 
			OnTouchDown(const TouchEvent * e);
		void 
			OnTouchUp(const TouchEvent * e);

		void
			OnMove(float dx);

	protected:
		Button * mBnSlider;

		int mMinValue;
		int mMaxValue;
		int mPosition;

		float mMouseX;
	};

	
	//
	class MGUI_ENTRY SliderBarV : public Widget
	{
		DECLARE_RTTI();

	public:
		tEvent2<const Event *, int> E_PositionChanged;

	public:
		SliderBarV(const LookFeel * _lookfeel, Widget * _parent);
		virtual ~SliderBarV();

		void 
			SetRange(int _min, int _max);
		void 
			SetPosition(int position);

		int 
			GetMinValue() { return mMinValue; }
		int 
			GetMaxValue() { return mMaxValue; }
		int 
			GetPosition() { return mPosition; }

	protected:
		void 
			OnMouseDrag(const MouseEvent * e);
		void 
			OnMouseDown(const MouseEvent * e);
		void 
			OnMouseUp(const MouseEvent * e);

		void 
			OnTouchMove(const TouchEvent * e);
		void 
			OnTouchDown(const TouchEvent * e);
		void 
			OnTouchUp(const TouchEvent * e);

		void
			OnMove(float dy);

	protected:
		Button * mBnSlider;

		int mMinValue;
		int mMaxValue;
		int mPosition;

		float mMouseY;
	};

}}
