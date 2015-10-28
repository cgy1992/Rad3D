#pragma once

#include "stdafx.h"

class DebugInfo : public Singleton<DebugInfo>
{
public:
	DebugInfo();
	~DebugInfo();

	void Update();

	void Show(bool _show);

protected:
	MGUI::Layout * mLayout;
	MGUI::ImageBox * mLogo;
	MGUI::TextBox * mBatch;
	MGUI::TextBox * mPrimitive;
	MGUI::TextBox * mFPS;
};
