/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MStudioEntry.h"
#include "UndoRedo.h"

class UndoRedo_Move : public UndoRedo
{
public: 
	UndoRedo_Move(int UID, const Float3 & orgPosition, const Float3 & newPosition);
	virtual ~UndoRedo_Move();

	virtual void 
		Undo();
	virtual void 
		Redo();

protected:
	int mUID;
	Float3 mOrgPosition;
	Float3 mNewPosition;
};

class UndoRedo_Rotate : public UndoRedo
{
public: 
	UndoRedo_Rotate(int UID, const Quat & orgOrientation, const Quat & newOrientation);
	virtual ~UndoRedo_Rotate();

	virtual void 
		Undo();
	virtual void 
		Redo();

protected:
	int mUID;
	Quat mOrgOrientation;
	Quat mNewOrientation;
};

class UndoRedo_Scale : public UndoRedo
{
public:
	UndoRedo_Scale(int UID, Float3 orgScale, Float3 newScale);
	virtual ~UndoRedo_Scale();

	virtual void 
		Undo();
	virtual void 
		Redo();

protected:
	int mUID;
	Float3 mOrgScale;
	Float3 mNewScale;
};

