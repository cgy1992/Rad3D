#include "Module.h"

Module::Module()
{
}

Module::~Module()
{
	for (int i = 0; i < mActors.Size(); ++i)
	{
		delete mActors[i];
	}

	mActors.Clear();
}

void Module::Update(float frameTime)
{
	for (int i = 0; i < mActors.Size(); ++i)
	{
		mActors[i]->Update(frameTime);
	}
}

void Module::AddActor(Actor * obj)
{
	mActors.PushBack(obj);
}

void Module::RemoveActor(Actor * obj)
{
	for (int i = 0; i < mActors.Size(); ++i)
	{
		if (mActors[i] == obj)
		{
			delete obj;
			mActors.Erase(i);

			break;
		}
	}
}