/*
*	ScrollView
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Widget.h"

namespace Rad { namespace MGUI {

	class MGUI_ENTRY ScrollViewH : public Widget
	{
		DECLARE_RTTI();

	public:
		tEvent1<float> E_Scroll;

	public:
		ScrollViewH(const LookFeel * _lookfeel, Widget * _parent);
		virtual ~ScrollViewH();

		void 
			Append(Widget * widget);
		void 
			Remove(Widget * widget, bool _delete = true);
		void 
			Remove(int index, bool _delete = true);
		void 
			Clear();

		int 
			GetCount();
		Widget * 
			GetItem(int index);
		const Rect & 
			GetItemRect();

		virtual void 
			OnUpdate(float elapsedTime);

	protected:
		virtual void 
			OnMouseDrag(float _x, float _y);
		virtual void 
			OnMouseDown(int _id, float _x, float _y);
		virtual void 
			OnMouseUp(int _id, float _x, float _y);

		virtual void 
			OnTouchDown(int id, float x, float y);
		virtual void 
			OnTouchUp(int id, float x, float y);
		virtual void 
			OnTouchMove(int id, float x, float y);
		virtual void 
			OnTouchCancel(int id, float x, float y);

		void 
			OnScroll(float dx);

		void 
			_updateItem();

	protected:
		Array<Widget *> mItems;

		Float2 mMousePosition;
		Field<Float2, MAX_TOUCH_POINTS> mTouchPosition;

		float mAnimSpeed;
		bool mAnimable;
		Rect mItemRect;
	};

	class MGUI_ENTRY ScrollViewV : public Widget
	{
		DECLARE_RTTI();

	public:
		tEvent1<float> E_Scroll;

	public:
		ScrollViewV(const LookFeel * _lookfeel, Widget * _parent);
		virtual ~ScrollViewV();

		void 
			Append(Widget * widget);
		void 
			Remove(Widget * widget, bool _delete = true);
		void 
			Remove(int index, bool _delete = true);
		void 
			Clear();

		int 
			GetCount();
		Widget * 
			GetItem(int index);
		const Rect & 
			GetItemRect();

		virtual void 
			OnUpdate(float elapsedTime);

	protected:
		virtual void 
			OnMouseDrag(float _x, float _y);
		virtual void 
			OnMouseDown(int _id, float _x, float _y);
		virtual void 
			OnMouseUp(int _id, float _x, float _y);

		virtual void 
			OnTouchDown(int id, float x, float y);
		virtual void 
			OnTouchUp(int id, float x, float y);
		virtual void 
			OnTouchMove(int id, float x, float y);
		virtual void 
			OnTouchCancel(int id, float x, float y);

		void 
			OnScroll(float dy);

		void 
			_updateItem();

	protected:
		Array<Widget *> mItems;

		Float2 mMousePosition;
		Field<Float2, MAX_TOUCH_POINTS> mTouchPosition;

		float mAnimSpeed;
		bool mAnimable;
		Rect mItemRect;
	};

}}