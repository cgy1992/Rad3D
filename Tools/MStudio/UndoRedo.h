/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MStudioEntry.h"

class UndoRedo
{
public:
	UndoRedo() {}
	virtual ~UndoRedo() {}

	virtual void Undo() = 0;
	virtual void Redo() = 0;
};

class UndoRedoManager : public Singleton<UndoRedoManager>
{
	static const int K_STACK_SIZE = 10;

public:
	UndoRedoManager();
	~UndoRedoManager();

	void Push(UndoRedo * op);

	void Undo();
	void Redo();

protected:
	Array<UndoRedo *> mUndoStack;
	Array<UndoRedo *> mRedoStack;
};