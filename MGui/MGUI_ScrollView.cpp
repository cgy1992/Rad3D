#include "MGUI_ScrollView.h"

namespace Rad { namespace MGUI {

	ImplementRTTI(ScrollViewH, Widget);

	ScrollViewH::ScrollViewH(const LookFeel * _lookfeel, Widget * _parent)
		: Widget(_lookfeel, _parent)
	{
		mAnimable = false;
		mAnimSpeed = 250;
	}

	ScrollViewH::~ScrollViewH()
	{
	}

	void ScrollViewH::Append(Widget * widget)
	{
		d_assert (widget->GetParent() == NULL &&
			(widget->GetAlign()._value & eAlign::H_STRETCH) == 0);

		_notifyAttach(widget);

		mItems.PushBack(widget);
	}

	void ScrollViewH::Remove(Widget * widget, bool _delete)
	{
		d_assert (widget->GetParent() == this);

		for (int i = 0; i < mItems.Size(); ++i)
		{
			if (mItems[i] == widget)
			{
				Remove(i, _delete);
				return ;
			}
		}

		d_assert (0);
	}

	void ScrollViewH::Remove(int index, bool _delete)
	{
		if (_delete)
		{
			delete mItems[index];
		}
		else
		{
			_notifyDetach(mItems[index]);
		}

		mItems.Erase(index);
	}

	void ScrollViewH::Clear()
	{
		for (int i = 0; i < mItems.Size(); ++i)
		{
			delete mItems[i];
		}
		mItems.Clear();
	}

	int ScrollViewH::GetCount()
	{
		return mItems.Size();
	}

	Widget * ScrollViewH::GetItem(int index)
	{
		return mItems[index];
	}

	const Rect & ScrollViewH::GetItemRect()
	{
		return mItemRect;
	}

	void ScrollViewH::_updateItem()
	{
		float x = mItemRect.x;
		for (int i = 0; i < mItems.Size(); ++i)
		{
			mItems[i]->SetRectPosition(x, 0);
			mItems[i]->_updateTM();

			x += mItems[i]->GetRect().w;
		}
	}

	void ScrollViewH::OnUpdate(float elapsedTime)
	{
		float x0 = 0, y0 = 0;
		float x1 = 0, y1 = 0;

		for (int i = 0; i < mItems.Size(); ++i)
		{
			const Rect & rc = mItems[i]->GetRect();

			x0 = Min(x0, rc.x);
			y0 = Min(y0, rc.y);
			x1 = Max(x1, rc.x + rc.w);
			y1 = Max(y1, rc.y + rc.h);
		}
		
		mItemRect.w = x1 - x0;
		mItemRect.h = y1 - y0;

		if (mAnimable)
		{
			x0 = mItemRect.x;
			y0 = mItemRect.y;
			x1 = x0 + mItemRect.w;
			y1 = y0 + mItemRect.h;

			float dt = elapsedTime;
			float dx = 0;

			dt = Max(dt, 0.1f);
			dx = dt * mAnimSpeed;

			if (x0 > 0)
			{
				x0 = Max(0.0f, x0 - dx);
				x1 = x0 + mItemRect.w;
			}
			else if (x0 < 0 && x1 < mClient.w)
			{
				x1 = Min(mClient.w, x1 + dx);
				x1 = Min(mItemRect.w, x1);
				x0 = x1 - mItemRect.w;
			}
			
			mItemRect.x = x0;
			mItemRect.y = y0;
		}

		_updateItem();
	}

	void ScrollViewH::OnMouseDrag(float x, float y)
	{
		float dx = x - mMousePosition.x;
		float dy = y - mMousePosition.y;

		OnScroll(dx);

		mMousePosition.x = x;
		mMousePosition.y = y;
	}

	void ScrollViewH::OnMouseDown(int id, float x, float y)
	{
		if (id == InputCode::MKC_LEFT)
		{
			mMousePosition.x = x;
			mMousePosition.y = y;

			mAnimable = false;
		}
	}

	void ScrollViewH::OnMouseUp(int id, float x, float y)
	{
		if (id == InputCode::MKC_LEFT)
		{
			mMousePosition.x = x;
			mMousePosition.y = y;

			mAnimable = true;
		}
	}

	void ScrollViewH::OnTouchDown(int id, float x, float y)
	{
		mTouchPosition[id].x = x;
		mTouchPosition[id].y = y;

		mAnimable = false;
	}

	void ScrollViewH::OnTouchUp(int id, float x, float y)
	{
		mTouchPosition[id].x = x;
		mTouchPosition[id].y = y;

		mAnimable = true;
	}

	void ScrollViewH::OnTouchCancel(int id, float x, float y)
	{
		mTouchPosition[id].x = x;
		mTouchPosition[id].y = y;

		mAnimable = true;
	}

	void ScrollViewH::OnTouchMove(int id, float x, float y)
	{
		float dx = x - mTouchPosition[id].x;
		float dy = y - mTouchPosition[id].y;

		OnScroll(dx);

		mTouchPosition[id].x = x;
		mTouchPosition[id].y = y;
	}

	void ScrollViewH::OnScroll(float dx)
	{
		if (mRect.w <= 1)
			return ;

		float x0 = mItemRect.x;
		float x1 = x0 + mItemRect.w;

		if (dx < 0)
		{
			float k = 1 - Min(-x0, mRect.w - x1) / (mRect.w * 0.35f);
			dx *= Math::Clamp(k, 0.0f, 1.0f);
		}
		else
		{
			float k = 1 - x0 / (mRect.w * 0.35f);
			dx *= Math::Clamp(k, 0.0f, 1.0f);
		}

		dx = Math::Floor(dx);
		mItemRect.x += dx;

		E_Scroll(dx);
	}


	//
	ImplementRTTI(ScrollViewV, Widget);

	ScrollViewV::ScrollViewV(const LookFeel * _lookfeel, Widget * _parent)
		: Widget(_lookfeel, _parent)
	{
		mAnimable = false;
		mAnimSpeed = 250;
	}

	ScrollViewV::~ScrollViewV()
	{
	}

	void ScrollViewV::Append(Widget * widget)
	{
		d_assert (widget->GetParent() == NULL &&
			(widget->GetAlign()._value & eAlign::V_STRETCH) == 0);

		_notifyAttach(widget);

		mItems.PushBack(widget);
	}

	void ScrollViewV::Remove(Widget * widget, bool _delete)
	{
		d_assert (widget->GetParent() == this);

		for (int i = 0; i < mItems.Size(); ++i)
		{
			if (mItems[i] == widget)
			{
				Remove(i, _delete);
				return ;
			}
		}

		d_assert (0);
	}

	void ScrollViewV::Remove(int index, bool _delete)
	{
		if (_delete)
		{
			delete mItems[index];
		}
		else
		{
			_notifyDetach(mItems[index]);
		}

		mItems.Erase(index);
	}

	void ScrollViewV::Clear()
	{
		for (int i = 0; i < mItems.Size(); ++i)
		{
			delete mItems[i];
		}
		mItems.Clear();
	}

	int ScrollViewV::GetCount()
	{
		return mItems.Size();
	}

	Widget * ScrollViewV::GetItem(int index)
	{
		return mItems[index];
	}

	const Rect & ScrollViewV::GetItemRect()
	{
		return mItemRect;
	}

	void ScrollViewV::_updateItem()
	{
		float y = mItemRect.y;
		for (int i = 0; i < mItems.Size(); ++i)
		{
			mItems[i]->SetRectPosition(0, y);
			mItems[i]->_updateTM();

			y += mItems[i]->GetRect().h;
		}
	}

	void ScrollViewV::OnUpdate(float elapsedTime)
	{
		float x0 = 0, y0 = 0;
		float x1 = 0, y1 = 0;

		for (int i = 0; i < mItems.Size(); ++i)
		{
			const Rect & rc = mItems[i]->GetRect();

			x0 = Min(x0, rc.x);
			y0 = Min(y0, rc.y);
			x1 = Max(x1, rc.x + rc.w);
			y1 = Max(y1, rc.y + rc.h);
		}

		mItemRect.w = x1 - x0;
		mItemRect.h = y1 - y0;

		if (mAnimable)
		{
			x0 = mItemRect.x;
			y0 = mItemRect.y;
			x1 = x0 + mItemRect.w;
			y1 = y0 + mItemRect.h;

			float dt = elapsedTime;
			float dy = 0;

			dt = Max(dt, 0.1f);
			dy = dt * mAnimSpeed;

			if (y0 > 0)
			{
				y0 = Max(0.0f, x0 - dy);
				y1 = y0 + mItemRect.h;
			}
			else if (y0 < 0 && y1 < mClient.h)
			{
				y1 = Min(mClient.h, y1 + dy);
				y1 = Min(mItemRect.h, y1);
				y0 = y1 - mItemRect.h;
			}

			mItemRect.x = x0;
			mItemRect.y = y0;
		}

		_updateItem();
	}

	void ScrollViewV::OnMouseDrag(float x, float y)
	{
		float dx = x - mMousePosition.x;
		float dy = y - mMousePosition.y;

		OnScroll(dy);

		mMousePosition.x = x;
		mMousePosition.y = y;
	}

	void ScrollViewV::OnMouseDown(int id, float x, float y)
	{
		if (id == InputCode::MKC_LEFT)
		{
			mMousePosition.x = x;
			mMousePosition.y = y;

			mAnimable = false;
		}
	}

	void ScrollViewV::OnMouseUp(int id, float x, float y)
	{
		if (id == InputCode::MKC_LEFT)
		{
			mMousePosition.x = x;
			mMousePosition.y = y;

			mAnimable = true;
		}
	}

	void ScrollViewV::OnTouchDown(int id, float x, float y)
	{
		mTouchPosition[id].x = x;
		mTouchPosition[id].y = y;

		mAnimable = false;
	}

	void ScrollViewV::OnTouchUp(int id, float x, float y)
	{
		mTouchPosition[id].x = x;
		mTouchPosition[id].y = y;

		mAnimable = true;
	}

	void ScrollViewV::OnTouchCancel(int id, float x, float y)
	{
		mTouchPosition[id].x = x;
		mTouchPosition[id].y = y;

		mAnimable = true;
	}

	void ScrollViewV::OnTouchMove(int id, float x, float y)
	{
		float dx = x - mTouchPosition[id].x;
		float dy = y - mTouchPosition[id].y;

		OnScroll(dy);

		mTouchPosition[id].x = x;
		mTouchPosition[id].y = y;
	}

	void ScrollViewV::OnScroll(float dy)
	{
		if (mRect.h <= 1)
			return ;

		float y0 = mItemRect.y;
		float y1 = y0 + mItemRect.y;

		if (dy < 0)
		{
			float k = 1 - Min(-y0, mRect.h - y1) / (mRect.h * 0.35f);
			dy *= Math::Clamp(k, 0.0f, 1.0f);
		}
		else
		{
			float k = 1 - y0 / (mRect.h * 0.35f);
			dy *= Math::Clamp(k, 0.0f, 1.0f);
		}

		dy = Math::Floor(dy);
		mItemRect.y += dy;

		E_Scroll(dy);
	}

}}