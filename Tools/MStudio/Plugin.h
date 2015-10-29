#pragma once

#include "MStudioEntry.h"

class MSTUDIO_ENTRY Plugin
{
public:
	Plugin() {}
	virtual ~Plugin() {}

	virtual void Init() = 0;
	virtual void Shutdown() = 0;
};

class MSTUDIO_ENTRY PluginManager : public Singleton<PluginManager>
{
public:
	PluginManager();
	~PluginManager();

	void AddPlugin(Plugin * plugin);

	void InitAll();
	void ShutdownAll();

protected:
	Array<Plugin *> mPlugins;
};