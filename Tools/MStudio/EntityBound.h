/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MStudioEntry.h"

class EntityBound
{
public:
	EntityBound();
	~EntityBound();

	void OnRender();

protected:
	void _doRenderAABB();
	void _doRenderColMesh();

	void OnShapeSelectedChanged();

protected:
	RenderOp * mRenderAABB;
	RenderOp * mRenderColMesh;
};

