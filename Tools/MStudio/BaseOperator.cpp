#include "BaseOperator.h"
#include "Editor.h"

UndoRedo_Move::UndoRedo_Move(int UID, const Float3 & orgPosition, const Float3 & newPosition)
	: mUID(UID)
	, mOrgPosition(orgPosition)
	, mNewPosition(newPosition)
{
}

UndoRedo_Move::~UndoRedo_Move()
{
}

void UndoRedo_Move::Undo()
{
	Node * obj = Editor::Instance()->GetNodeByUID(mUID);

	if (obj)
	{
		obj->SetPosition(mOrgPosition);
	}
}

void UndoRedo_Move::Redo()
{
	Node * obj = Editor::Instance()->GetNodeByUID(mUID);

	if (obj)
	{
		obj->SetPosition(mNewPosition);
	}
}



UndoRedo_Rotate::UndoRedo_Rotate(int UID, const Quat & orgOrientation, const Quat & newOrientation)
	: mUID(UID)
	, mOrgOrientation(orgOrientation)
	, mNewOrientation(newOrientation)
{
}

UndoRedo_Rotate::~UndoRedo_Rotate()
{
}

void UndoRedo_Rotate::Undo()
{
	Node * obj = Editor::Instance()->GetNodeByUID(mUID);

	if (obj)
	{
		obj->SetRotation(mOrgOrientation);
	}
}

void UndoRedo_Rotate::Redo()
{
	Node * obj = Editor::Instance()->GetNodeByUID(mUID);

	if (obj)
	{
		obj->SetRotation(mNewOrientation);
	}
}




UndoRedo_Scale::UndoRedo_Scale(int UID, Float3 orgScale, Float3 newScale)
	: mUID(UID)
	, mOrgScale(orgScale)
	, mNewScale(newScale)
{
}

UndoRedo_Scale::~UndoRedo_Scale()
{
}

void UndoRedo_Scale::Undo()
{
	Node * obj = Editor::Instance()->GetNodeByUID(mUID);

	if (obj)
	{
		obj->SetScale(mOrgScale);
	}
}

void UndoRedo_Scale::Redo()
{
	Node * obj = Editor::Instance()->GetNodeByUID(mUID);

	if (obj)
	{
		obj->SetScale(mNewScale);
	}
}


