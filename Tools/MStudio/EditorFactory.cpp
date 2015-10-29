#include "EditorFactory.h"
#include "Editor.h"

// Mesh
Node * MeshCreator::NewObject()
{
	Mesh * mesh = new Mesh;

	mesh->SetFilename("Editor.mesh");

	Editor::Instance()->AddNode(mesh);

	return mesh;
}

Node * MeshCreator::NewObjectByDrag(const char * filename, const char * ext)
{
	if (stricmp(ext, "mesh") == 0)
	{
		Mesh * mesh = new Mesh;

		mesh->SetFilename(filename);

		Editor::Instance()->AddNode(mesh);

		return mesh;
	}

	return NULL;
}

// Light
Node * LightCreator::NewObject()
{
	Light * light = new Light;

	Editor::Instance()->AddNode(light);

	return light;
}

// Particle
Node * ParticleCreator::NewObject()
{
	ParticleSystem * particle = new ParticleSystem;

	particle->SetFilename("Editor.particle");

	Editor::Instance()->AddNode(particle);

	return particle;
}

Node * ParticleCreator::NewObjectByDrag(const char * filename, const char * ext)
{
	if (stricmp(ext, "particle") == 0)
	{
		ParticleSystem * particle = new ParticleSystem;

		particle->SetFilename(filename);

		Editor::Instance()->AddNode(particle);

		return particle;
	}

	return NULL;
}

// Sound
Node * SoundCreator::NewObject()
{
	Sound * sound = new Sound;

	Editor::Instance()->AddNode(sound);

	return sound;
}

Node * SoundCreator::NewObjectByDrag(const char * filename, const char * ext)
{
	if (stricmp(ext, "wav") == 0)
	{
		Sound * sound = new Sound;

		sound->SetFilename(filename);

		Editor::Instance()->AddNode(sound);

		return sound;
	}

	return NULL;
}

// Prefab
Node * PrefabCreator::NewObject()
{
	Prefab * prefab = new Prefab;

	Editor::Instance()->AddNode(prefab);

	return prefab;
}

Node * PrefabCreator::NewObjectByDrag(const char * filename, const char * ext)
{
	if (stricmp(ext, "prefab") == 0)
	{
		Prefab * prefab = new Prefab;
		prefab->SetFilename(filename);

		Editor::Instance()->AddNode(prefab);

		return prefab;
	}

	return NULL;
}

//
//
ImplementSingleton(EditorFactory);

EditorFactory::EditorFactory()
{
	AddEntityFactory(new MeshCreator);
	AddEntityFactory(new ParticleCreator);
	AddEntityFactory(new LightCreator);
	AddEntityFactory(new SoundCreator);
	AddEntityFactory(new PrefabCreator);
}

EditorFactory::~EditorFactory()
{
	for (int i = 0; i < mEntityFactorys.Size(); ++i)
	{
		delete mEntityFactorys[i];
	}

	for (int i = 0; i < mComponentFactorys.Size(); ++i)
	{
		delete mComponentFactorys[i];
	}
}

void EditorFactory::AddEntityFactory(EntityFactory * factory)
{
	d_assert (GetEntityFactory(factory->GetTypeName()) == NULL);

	mEntityFactorys.PushBack(factory);
}

EntityFactory * EditorFactory::GetEntityFactory(const FixedString32 & type)
{
	for (int i = 0; i < mEntityFactorys.Size(); ++i)
	{
		if (mEntityFactorys[i]->GetTypeName() == type)
		{
			return mEntityFactorys[i];
		}
	}
	
	return NULL;
}

void EditorFactory::AddComponentFactory(ComponentFactory * factory)
{
	d_assert (GetComponentFactory(factory->GetTypeName()) == NULL);

	mComponentFactorys.PushBack(factory);
}

ComponentFactory * EditorFactory::GetComponentFactory(const FixedString32 & type)
{
	for (int i = 0; i < mComponentFactorys.Size(); ++i)
	{
		if (mComponentFactorys[i]->GetTypeName() == type)
		{
			return mComponentFactorys[i];
		}
	}

	return NULL;
}