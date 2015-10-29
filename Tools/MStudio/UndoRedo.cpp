#include "UndoRedo.h"

ImplementSingleton(UndoRedoManager);

UndoRedoManager::UndoRedoManager()
{
}

UndoRedoManager::~UndoRedoManager()
{
	while (mRedoStack.Size())
	{
		UndoRedo * redo = mRedoStack[0];
		delete redo;
		mRedoStack.PopFront();
	}

	while (mUndoStack.Size())
	{
		UndoRedo * undo = mUndoStack[0];
		delete undo;
		mUndoStack.PopFront();
	}
}

void UndoRedoManager::Push(UndoRedo * op)
{
	mUndoStack.PushFront(op);
	
	while (mRedoStack.Size())
	{
		UndoRedo * redo = mRedoStack[0];
		delete redo;
		mRedoStack.PopFront();
	}

	if (mUndoStack.Size() > K_STACK_SIZE)
	{
		UndoRedo * undo = mUndoStack[mUndoStack.Size() - 1];
		delete undo;
		mUndoStack.PopBack();
	}
}

void UndoRedoManager::Undo()
{
	if (mUndoStack.Size())
	{
		UndoRedo * undo = mUndoStack[0];
		undo->Undo();

		mUndoStack.PopFront();
		mRedoStack.PushFront(undo);
	}
}

void UndoRedoManager::Redo()
{
	if (mRedoStack.Size())
	{
		UndoRedo * redo = mRedoStack[0];
		redo->Redo();

		mRedoStack.PopFront();
		mUndoStack.PushFront(redo);
	}
}

