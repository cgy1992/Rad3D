/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MStudioEntry.h"

//
struct MSTUDIO_ENTRY EntityFactory
{
public:
	EntityFactory() {}
	virtual ~EntityFactory() {}

	virtual FixedString32
		GetTypeName() = 0;
	virtual Node *
		NewObject() = 0;
	virtual Node *
		NewObjectByDrag(const char * filename, const char * ext) { return NULL; }
};

//
struct MeshCreator : public EntityFactory
{
public:
	virtual ~MeshCreator() {}

	virtual FixedString32
		GetTypeName() { return "Mesh"; }
	virtual Node *
		NewObject();
	virtual Node *
		NewObjectByDrag(const char * filename, const char * ext);

protected:
};

//
struct LightCreator : public EntityFactory
{
public:
	virtual ~LightCreator() {}

	virtual FixedString32
		GetTypeName() { return "Light"; }
	virtual Node *
		NewObject();
};

//
struct ParticleCreator : public EntityFactory
{
public:
	virtual ~ParticleCreator() {}

	virtual FixedString32
		GetTypeName() { return "Particle"; }
	virtual Node *
		NewObject();
	virtual Node *
		NewObjectByDrag(const char * filename, const char * ext);
};

//
struct SoundCreator : public EntityFactory
{
public:
	virtual ~SoundCreator() {}

	virtual FixedString32
		GetTypeName() { return "Sound"; }
	virtual Node *
		NewObject();
	virtual Node *
		NewObjectByDrag(const char * filename, const char * ext);
};

//
struct PrefabCreator : public EntityFactory
{
public:
	virtual ~PrefabCreator() {}

	virtual FixedString32
		GetTypeName() { return "Prefab"; }
	virtual Node *
		NewObject();
	virtual Node *
		NewObjectByDrag(const char * filename, const char * ext);
};

//
//
class MSTUDIO_ENTRY ComponentFactory
{
public:
	ComponentFactory() {}
	virtual ~ComponentFactory() {}

	virtual FixedString32
		GetTypeName() = 0;
	virtual IComponent *
		NewObject() = 0;
};


//
//
class EditorFactory : public Singleton<EditorFactory>
{
public:
	EditorFactory();
	~EditorFactory();

	void
		AddEntityFactory(EntityFactory * factory);
	EntityFactory *
		GetEntityFactory(const FixedString32 & type);
	Array<EntityFactory *> &
		GetEntityFactorys() { return mEntityFactorys; }

	void
		AddComponentFactory(ComponentFactory * factory);
	ComponentFactory *
		GetComponentFactory(const FixedString32 & type);
	Array<ComponentFactory *>
		GetComponentFactorys() { return mComponentFactorys; }

protected:
	Array<EntityFactory *> mEntityFactorys;
	Array<ComponentFactory *> mComponentFactorys;
};