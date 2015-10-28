#include "MGUI_GridBox.h"
#include "MGUI_Helper.h"

namespace Rad { namespace MGUI {

	ImplementRTTI(GridBox, Widget);

	GridBox::GridBox(const LookFeel * _lookfeel, Widget * _parent)
		: Widget(NULL, _parent)
		, mColumn(1)
		, mAutoResizeType(ALL)
		, mGridAlign(eAlign::CENTER)
		, mGridWidth(0)
		, mGridHeight(0)
	{
	}

	GridBox::~GridBox()
	{
		Clear();
	}

	void GridBox::Update(float elapsedTime)
	{
		if (mVisible)
		{
			float x_base = 0, y_base = 0;

			Rect autoRect;

			for (int i = 0; i < mGridedWidgets.Size(); ++i)
			{
				Widget * widget = mGridedWidgets[i];

				if (x_base >= mColumn * mGridWidth)
				{
					x_base = 0;
					y_base += mGridHeight + 1;
				}

				Rect rect;

				rect.x = x_base;
				rect.y = y_base;
				rect.w = mGridWidth;
				rect.h = mGridHeight;

				rect = Helper::Instance()->GetAlignRect(widget->GetRect(), rect, mGridAlign);

				x_base += mGridWidth + 1;

				autoRect.w = Max(autoRect.w, rect.x + rect.w);
				autoRect.h = Max(autoRect.h, rect.y + rect.h);

				widget->SetRect(rect);
			}

			int type = mAutoResizeType;
			Rect myRect = GetRect();

			if (type & WIDTH)
				myRect.w = autoRect.w;

			if (type & HEIGHT)
				myRect.h = autoRect.h;

			if (type != NONE)
				SetRect(myRect);
		}

		Widget::Update(elapsedTime);
	}

	void GridBox::SetAutoResizeType(eAutoResizeType type)
	{
		mAutoResizeType = type;
	}

	GridBox::eAutoResizeType GridBox::GetAutoResizeType()
	{
		return mAutoResizeType;
	}

	void GridBox::SetGridSize(float w, float h)
	{
		mGridWidth = w;
		mGridHeight = h;
	}

	void GridBox::GetGridSize(float & w, float & h)
	{
		w = mGridWidth;
		h = mGridHeight;
	}

	void GridBox::SetGridAlign(eAlign align)
	{
		mGridAlign = align;
	}

	eAlign GridBox::GetGridAlign()
	{
		return mGridAlign;
	}

	int GridBox::GetGridWidgetCount()
	{
		return mGridedWidgets.Size();
	}

	Widget * GridBox::GetGridWidget(int i)
	{
		return mGridedWidgets[i];
	}

	void GridBox::SetColumn(int col)
	{
		d_assert (col > 0);

		mColumn = col;
	}

	int GridBox::GetColumn()
	{
		return mColumn;
	}

	void GridBox::Append(Widget * widget)
	{
		d_assert (widget->GetParent() == NULL);

		widget->SetAlign(eAlign::NONE);

		_notifyAttach(widget);

		mGridedWidgets.PushBack(widget);
	}

	void GridBox::Remove(Widget * widget)
	{
		d_assert (widget->GetParent() == this);

		for (int i = 0; i < mGridedWidgets.Size(); ++i)
		{
			if (mGridedWidgets[i] == widget)
			{
				mGridedWidgets.Erase(i);
				delete widget;
				return ;
			}
		}

		d_assert (0);
	}

	void GridBox::Clear()
	{
		for (int i = 0; i < mGridedWidgets.Size(); ++i)
			delete mGridedWidgets[i];

		mGridedWidgets.Clear();
	}

}}

