#include "MGUI_Panel.h"
#include "MGUI_Helper.h"

namespace Rad { namespace MGUI {

	ImplementRTTI(Panel, Widget);

	Panel::Panel(const LookFeel * _lookfeel, Widget * _parent)
		: Widget(_lookfeel, _parent)
		, mHScrollBar(NULL)
		, mVScrollBar(NULL)
	{
		mClientWidget = new Widget(NULL, this);
		mClientWidget->SetPickFlag(ePickFlag::CHILD);

		const LookFeel * hScrollBar = NULL;
		const LookFeel * vScrollBar = NULL;
		
		if (_lookfeel != NULL)
		{
			hScrollBar = _lookfeel->GetChild("ScrollBarH");
			vScrollBar = _lookfeel->GetChild("ScrollBarV");
		}

		if (hScrollBar != NULL)
		{
			SetHScrollBar(hScrollBar);
		}

		if (vScrollBar != NULL)
		{
			SetVScrollBar(vScrollBar);
		}
	}

	Panel::~Panel()
	{
	}

	void Panel::SetHScrollBar(const LookFeel * _lookfeel)
	{
		safe_delete (mHScrollBar);

		if (_lookfeel != NULL)
		{
			mHScrollBar = new ScrollBarH(_lookfeel, this);
			mHScrollBar->SetOrder(eOrder::TEXT + 1);

			mHScrollBar->E_PositionChanged += new cListener2<Panel, const Event *, float>(this, &Panel::OnHScroll);
		}
	}

	void Panel::SetVScrollBar(const LookFeel * _lookfeel)
	{
		safe_delete (mVScrollBar);

		if (_lookfeel != NULL)
		{
			mVScrollBar = new ScrollBarV(_lookfeel, this);
			mVScrollBar->SetOrder(eOrder::TEXT + 1);

			mVScrollBar->E_PositionChanged += new cListener2<Panel, const Event *, float>(this, &Panel::OnVScroll);
		}
	}

	void Panel::_updateClient()
	{
		Rect clientRect;

		clientRect.x = mClientWidget->GetRect().x;
		clientRect.y = mClientWidget->GetRect().y;
		clientRect.w = mVScrollBar ? mClient.w - mVScrollBar->GetRect().w : mClient.w;
		clientRect.h = mHScrollBar ? mClient.h - mHScrollBar->GetRect().h : mClient.h;

		for (int i = 0; i < mClientWidget->GetChildCount(); ++i)
		{
			Widget * child = mClientWidget->GetChild(i);

			if (!child->IsVisible())
				continue ;

			clientRect.w = Max(clientRect.w, child->GetRect().x + child->GetRect().w);
			clientRect.h = Max(clientRect.h, child->GetRect().y + child->GetRect().h);
		}

		mClientWidget->SetRect(clientRect);
	}

	void Panel::_updateVScroll()
	{
		if (mVScrollBar == NULL)
			return ;

		Rect parentRect = this->mClient;
		parentRect.x = 0;
		parentRect.y = 0;

		Rect rect;
		rect = Helper::Instance()->GetAlignRect(mVScrollBar->GetRect(),
			parentRect, eAlign::RIGHT | eAlign::V_STRETCH);

		mVScrollBar->SetRect(rect);
	}

	void Panel::_updateHScroll()
	{
		if (mHScrollBar == NULL)
			return ;

		Rect parentRect = this->mClient;
		parentRect.x = 0;
		parentRect.y = 0;

		Rect rect;
		rect = Helper::Instance()->GetAlignRect(mHScrollBar->GetRect(),
			parentRect, eAlign::RIGHT | eAlign::V_STRETCH);

		if (mVScrollBar && mVScrollBar->IsVisible())
		{
			rect.w -= mVScrollBar->GetRect().w;
		}

		mHScrollBar->SetRect(rect);
	}

	void Panel::OnUpdate(float elapsedTime)
	{
		_updateClient();

		_updateVScroll();

		float dy = mClientWidget->GetRect().h - mClient.h;
		if (mVScrollBar && dy > 0)
		{
			mVScrollBar->SetRange(0, dy);
			mVScrollBar->SetVisible(true);
		}
		else if (mVScrollBar)
		{
			mVScrollBar->SetVisible(false);
			OnVScroll(NULL, 0);
		}

		_updateHScroll();

		float dx = mClientWidget->GetRect().w - mClient.w;
		if (mHScrollBar && dx > 0)
		{
			mHScrollBar->SetRange(0, dx);
			mHScrollBar->SetVisible(true);
		}
		else if (mHScrollBar)
		{
			mHScrollBar->SetVisible(false);
			OnHScroll(NULL, 0);
		}
	}

	void Panel::OnHScroll(const Event *, float _position)
	{
		float x, y;
		mClientWidget->GetRectPosition(x, y);

		mClientWidget->SetRectPosition(-_position, y);
	}

	void Panel::OnVScroll(const Event *, float _position)
	{
		float x, y;
		mClientWidget->GetRectPosition(x, y);

		mClientWidget->SetRectPosition(x, -_position);
	}

}}