#pragma once

#include "Object.h"


class GmCompRotate : public GmComponent
{
public:
	GmCompRotate();
	virtual ~GmCompRotate();

	void SetTargetDir(float dir);
	float GetTargetDir();

	virtual void SetObject(GmObj * obj);
	virtual void Update(float time);

protected:
	float mTargetDir;
};