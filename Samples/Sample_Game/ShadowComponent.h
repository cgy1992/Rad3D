#pragma once

#include "stdafx.h"

class Actor;

class ShadowComponent : public IComponent
{
	static const int GRID_COUNT;

public:
	ShadowComponent();
	virtual ~ShadowComponent();

	virtual int
		Update(float time);

protected:
	void 
		OnRender();
	Actor * 
		GetActor() { return (Actor *)mOwner; }

protected:
	ShaderFX * mShaderFX;
	bool mVisible;
	RenderOp mRenderOp;
	cListener0<ShadowComponent> listenerRender;
};