#pragma once

#include "Module.h"

class ModuleDemo : public Module
{
public:
	ModuleDemo();
	~ModuleDemo();

	virtual void 
		Update(float frameTime);

protected:
	void 
		OnMouseDown(const MGUI::MouseEvent * e);
	void 
		OnTouchDown(const MGUI::TouchEvent * e);

protected:
	Actor * mPlayer;
};