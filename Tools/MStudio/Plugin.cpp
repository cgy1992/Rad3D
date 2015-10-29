#include "Plugin.h"

ImplementSingleton(PluginManager);

PluginManager::PluginManager()
{
}

PluginManager::~PluginManager()
{
}

void PluginManager::AddPlugin(Plugin * plugin)
{
	for (int i = 0; i < mPlugins.Size(); ++i)
	{
		if (mPlugins[i] == plugin)
			return ;
	}

	mPlugins.PushBack(plugin);
}

void PluginManager::InitAll()
{
	for (int i = 0; i < mPlugins.Size(); ++i)
	{
		mPlugins[i]->Init();
	}
}

void PluginManager::ShutdownAll()
{
	for (int i = 0; i < mPlugins.Size(); ++i)
	{
		mPlugins[i]->Shutdown();
	}
	mPlugins.Clear();
}