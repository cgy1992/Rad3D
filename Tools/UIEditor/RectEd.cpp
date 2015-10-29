#include "RectEd.h"
#include "AllLookFeel.h"
#include "MGUI_Cursor.h"


RectEd::RectEd(MGUI::Widget * _parent)
		: MGUI::Widget(NULL, _parent)
{
	mLeft = new MGUI::Widget(AllLookFeel::Instance()->GetWhite(), this);
	mTop = new MGUI::Widget(AllLookFeel::Instance()->GetWhite(), this);
	mRight = new MGUI::Widget(AllLookFeel::Instance()->GetWhite(), this);
	mBottom = new MGUI::Widget(AllLookFeel::Instance()->GetWhite(), this);

	mLeft->SetPickFlag(MGUI::ePickFlag::NONE);
	mTop->SetPickFlag(MGUI::ePickFlag::NONE);
	mRight->SetPickFlag(MGUI::ePickFlag::NONE);
	mBottom->SetPickFlag(MGUI::ePickFlag::NONE);

	mLeft->SetColor(Float4(0, 0, 1, 0.8f));
	mTop->SetColor(Float4(0, 0, 1, 0.8f));
	mRight->SetColor(Float4(0, 0, 1, 0.8f));
	mBottom->SetColor(Float4(0, 0, 1, 0.8f));

	mLeftCl = new MGUI::Widget(AllLookFeel::Instance()->GetWhite(), this);
	mTopCl = new MGUI::Widget(AllLookFeel::Instance()->GetWhite(), this);
	mRightCl = new MGUI::Widget(AllLookFeel::Instance()->GetWhite(), this);
	mBottomCl = new MGUI::Widget(AllLookFeel::Instance()->GetWhite(), this);

	mLeftCl->SetPickFlag(MGUI::ePickFlag::NONE);
	mTopCl->SetPickFlag(MGUI::ePickFlag::NONE);
	mRightCl->SetPickFlag(MGUI::ePickFlag::NONE);
	mBottomCl->SetPickFlag(MGUI::ePickFlag::NONE);

	mLeftCl->SetColor(Float4(0, 1, 0, 0.8f));
	mTopCl->SetColor(Float4(0, 1, 0, 0.8f));
	mRightCl->SetColor(Float4(0, 1, 0, 0.8f));
	mBottomCl->SetColor(Float4(0, 1, 0, 0.8f));

	mLeftLn = new MGUI::Widget(NULL, this);
	mRightLn = new MGUI::Widget(NULL, this);
	mTopLn = new MGUI::Widget(NULL, this);
	mBottomLn = new MGUI::Widget(NULL, this);

	mLeftLn->SetOrder(1);
	mRightLn->SetOrder(1);
	mTopLn->SetOrder(1);
	mBottomLn->SetOrder(1);

	mLeftLn->E_MouseLostFocus += new cListener1<RectEd, const MGUI::FocusEvent *>(this, &RectEd::OnLeftFocus_);
	mRightLn->E_MouseLostFocus += new cListener1<RectEd, const MGUI::FocusEvent *>(this, &RectEd::OnRightFocus_);
	mTopLn->E_MouseLostFocus += new cListener1<RectEd, const MGUI::FocusEvent *>(this, &RectEd::OnTopFocus_);
	mBottomLn->E_MouseLostFocus += new cListener1<RectEd, const MGUI::FocusEvent *>(this, &RectEd::OnBottomFocus_);

	mLeftLn->E_MouseSetFocus += new cListener1<RectEd, const MGUI::FocusEvent *>(this, &RectEd::OnLeftFocus_);
	mRightLn->E_MouseSetFocus += new cListener1<RectEd, const MGUI::FocusEvent *>(this, &RectEd::OnRightFocus_);
	mTopLn->E_MouseSetFocus += new cListener1<RectEd, const MGUI::FocusEvent *>(this, &RectEd::OnTopFocus_);
	mBottomLn->E_MouseSetFocus += new cListener1<RectEd, const MGUI::FocusEvent *>(this, &RectEd::OnBottomFocus_);

	mLeftLn->E_MouseDrag += new cListener1<RectEd, const MGUI::MouseEvent *>(this, &RectEd::OnLeftDrag_);
	mRightLn->E_MouseDrag += new cListener1<RectEd, const MGUI::MouseEvent *>(this, &RectEd::OnRightDrag_);
	mTopLn->E_MouseDrag += new cListener1<RectEd, const MGUI::MouseEvent *>(this, &RectEd::OnTopDrag_);
	mBottomLn->E_MouseDrag += new cListener1<RectEd, const MGUI::MouseEvent *>(this, &RectEd::OnBottomDrag_);
}

RectEd::~RectEd()
{
}

void RectEd::SetEditRect(const MGUI::Rect & _rect)
{
	if (GetRect() == _rect)
		return ;

	SetRect(_rect);
	_updateRect();
	_updateClient();

	evRectChanged(_rect);
}

void RectEd::SetEditClient(const MGUI::ClientRect & client)
{
	mClient = client;
	_updateClient();
}

const MGUI::Rect & RectEd::GetEditRect()
{
	return GetRect();
}

void RectEd::_updateRect()
{
	const MGUI::Rect & _rect = GetRect();
	MGUI::Rect rcLeft, rcTop, rcRight, rcBottom;

	float dx = _rect.w, dy = _rect.h;

	{
		rcLeft.x = 0;
		rcLeft.y = 0;
		rcLeft.w = 1;
		rcLeft.h = dy;

		rcTop.x = 0;
		rcTop.y = 0;
		rcTop.w = dx;
		rcTop.h = 1;

		rcRight.x = dx - 1;
		rcRight.y = 0;
		rcRight.w = 1;
		rcRight.h = dy;

		rcBottom.x = 0;
		rcBottom.y = dy - 1;
		rcBottom.w = dx;
		rcBottom.h = 1;

		mLeft->SetRect(rcLeft);
		mTop->SetRect(rcTop);
		mRight->SetRect(rcRight);
		mBottom->SetRect(rcBottom);
	}

	{
		rcLeft.x = 0;
		rcLeft.y = 0;
		rcLeft.w = 5;
		rcLeft.h = dy;

		rcTop.x = 0;
		rcTop.y = 0;
		rcTop.w = dx;
		rcTop.h = 5;

		rcRight.x = dx - 5;
		rcRight.y = 0;
		rcRight.w = 5;
		rcRight.h = dy;

		rcBottom.x = 0;
		rcBottom.y = dy - 5;
		rcBottom.w = dx;
		rcBottom.h = 5;

		mLeftLn->SetRect(rcLeft);
		mTopLn->SetRect(rcTop);
		mRightLn->SetRect(rcRight);
		mBottomLn->SetRect(rcBottom);
	}
}

void RectEd::_updateClient()
{
	MGUI::Rect _rect = GetRect();
	MGUI::Rect rcLeft, rcTop, rcRight, rcBottom;

	float x0 = mClient.dx0;
	float y0 = mClient.dy0;
	float x1 = _rect.w - mClient.dx1;
	float y1 = _rect.h - mClient.dy1;

	rcLeft.x = x0;
	rcLeft.y = y0;
	rcLeft.w = 1;
	rcLeft.h = y1 - y0;

	rcTop.x = x0;
	rcTop.y = y0;
	rcTop.w = x1 - x0;
	rcTop.h = 1;

	rcRight.x = x1 - 1;
	rcRight.y = y0;
	rcRight.w = 1;
	rcRight.h = y1 - y0;

	rcBottom.x = x0;
	rcBottom.y = y1 - 1;
	rcBottom.w = x1 - x0;
	rcBottom.h = 1;

	mLeftCl->SetRect(rcLeft);
	mTopCl->SetRect(rcTop);
	mRightCl->SetRect(rcRight);
	mBottomCl->SetRect(rcBottom);

	if (mClient == MGUI::ClientRect())
	{
		mLeftCl->SetVisible(false);
		mTopCl->SetVisible(false);
		mRightCl->SetVisible(false);
		mBottomCl->SetVisible(false);
	}
	else
	{
		mLeftCl->SetVisible(true);
		mTopCl->SetVisible(true);
		mRightCl->SetVisible(true);
		mBottomCl->SetVisible(true);
	}

}

void RectEd::OnMouseLostFocus(MGUI::Widget* _new)
{
	MGUI::Cursor::Instance()->Unbind();
}

void RectEd::OnMouseSetFocus(MGUI::Widget* _old)
{
	MGUI::Cursor::Instance()->Bind(MGUI::Cursor::kSize);
}

void RectEd::OnMouseDrag(float _x, float _y)
{
	if (MGUI::InputManager::Instance()->IsMouseCaptured())
	{
		float dx = _x - mX;
		float dy = _y - mY;

		MGUI::Rect rect = GetEditRect();

		rect.x += dx;
		rect.y += dy;

		mX = _x; mY = _y;

		SetEditRect(rect);
	}
}

void RectEd::OnMouseDown(int _id, float _x, float _y)
{
	if (_id == InputCode::MKC_LEFT)
	{
		mX = _x;
		mY = _y;
	}
}

void RectEd::OnMouseUp(int _id, float _x, float _y)
{
	if (_id == InputCode::MKC_LEFT)
	{
		mX = _x;
		mY = _y;
	}
}

void RectEd::OnLeftFocus_(const MGUI::FocusEvent * e)
{
	if (e->addr == &e->sender->E_MouseSetFocus)
	{
		MGUI::Cursor::Instance()->Bind(MGUI::Cursor::kSizeWE);
	}
	else
	{
		MGUI::Cursor::Instance()->Unbind();
	}
}

void RectEd::OnRightFocus_(const MGUI::FocusEvent * e)
{
	if (e->addr == &e->sender->E_MouseSetFocus)
	{
		MGUI::Cursor::Instance()->Bind(MGUI::Cursor::kSizeWE);
	}
	else
	{
		MGUI::Cursor::Instance()->Unbind();
	}
}

void RectEd::OnTopFocus_(const MGUI::FocusEvent * e)
{
	if (e->addr == &e->sender->E_MouseSetFocus)
	{
		MGUI::Cursor::Instance()->Bind(MGUI::Cursor::kSizeNS);
	}
	else
	{
		MGUI::Cursor::Instance()->Unbind();
	}
}

void RectEd::OnBottomFocus_(const MGUI::FocusEvent * e)
{
	if (e->addr == &e->sender->E_MouseSetFocus)
	{
		MGUI::Cursor::Instance()->Bind(MGUI::Cursor::kSizeNS);
	}
	else
	{
		MGUI::Cursor::Instance()->Unbind();
	}
}

void RectEd::OnLeftDrag_(const MGUI::MouseEvent * e)
{
	MGUI::Rect clRect = GetParent()->GetAbsClient();
	MGUI::Rect myRect = GetRect();

	float newX =  e->x - clRect.x;

	newX = Max(newX, 0.0f);
	newX = Min(newX, myRect.x + myRect.w);

	myRect.w = myRect.x + myRect.w - newX;
	myRect.x = newX;

	SetEditRect(myRect);
}

void RectEd::OnRightDrag_(const MGUI::MouseEvent * e)
{
	MGUI::Rect clRect = GetParent()->GetAbsClient();
	MGUI::Rect myRect = GetRect();

	float newX = (float) e->x - clRect.x;

	newX = Min(newX, clRect.w);
	newX = Max(myRect.x, newX);

	myRect.w = newX - myRect.x;

	SetEditRect(myRect);
}

void RectEd::OnTopDrag_(const MGUI::MouseEvent * e)
{
	MGUI::Rect clRect = GetParent()->GetAbsClient();
	MGUI::Rect myRect = GetRect();

	float newY =  e->y - clRect.y;

	newY = Max(newY, 0.0f);
	newY = Min(newY, myRect.y + myRect.h);

	myRect.h = myRect.y + myRect.h - newY;
	myRect.y = newY;

	SetEditRect(myRect);
}

void RectEd::OnBottomDrag_(const MGUI::MouseEvent * e)
{
	MGUI::Rect clRect = GetParent()->GetAbsClient();
	MGUI::Rect myRect = GetRect();

	float newY = e->y - clRect.y;

	newY = Min(newY, clRect.h);
	newY = Max(myRect.y, newY);

	myRect.h = newY - myRect.y;

	SetEditRect(myRect);
}

