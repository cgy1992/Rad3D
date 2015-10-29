/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "stdafx.h"

class RectEd : public MGUI::Widget
{
public:
	tEvent1<MGUI::Rect> evRectChanged;

public:
	RectEd(MGUI::Widget * _parent);
	~RectEd();

	void SetEditRect(const MGUI::Rect & _rect);
	const MGUI::Rect & GetEditRect();

	void SetEditClient(const MGUI::ClientRect & client);

protected:
	void _updateRect();
	void _updateClient();

protected:
	virtual void OnMouseLostFocus(MGUI::Widget* _new);
	virtual void OnMouseSetFocus(MGUI::Widget* _old);
	virtual void OnMouseDrag(float _x, float _y);
	virtual void OnMouseDown(int _id, float _x, float _y);
	virtual void OnMouseUp(int _id, float _x, float _y);

	void OnLeftFocus_(const MGUI::FocusEvent * e);
	void OnRightFocus_(const MGUI::FocusEvent * e);
	void OnTopFocus_(const MGUI::FocusEvent * e);
	void OnBottomFocus_(const MGUI::FocusEvent * e);

	void OnLeftDrag_(const MGUI::MouseEvent * e);
	void OnRightDrag_(const MGUI::MouseEvent * e);
	void OnTopDrag_(const MGUI::MouseEvent * e);
	void OnBottomDrag_(const MGUI::MouseEvent * e);

protected:

	MGUI::Widget * mLeft;
	MGUI::Widget * mRight;
	MGUI::Widget * mTop;
	MGUI::Widget * mBottom;

	MGUI::Widget * mLeftCl;
	MGUI::Widget * mRightCl;
	MGUI::Widget * mTopCl;
	MGUI::Widget * mBottomCl;

	MGUI::Widget * mLeftLn;
	MGUI::Widget * mRightLn;
	MGUI::Widget * mTopLn;
	MGUI::Widget * mBottomLn;

	float mX, mY;
	MGUI::ClientRect mClient;
};

