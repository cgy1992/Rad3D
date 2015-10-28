#include "stdafx.h"

void Game_SetupResource()
{
#ifdef MW_PLATFORM_WIN32
	ResourceManager::Instance()->AddArchive(new FilePathArchive("../Sample"));
	FileHelper::SetCachePath("Cache");
#endif

#ifdef MW_PLATFORM_ANDROID
	ResourceManager::Instance()->AddArchive(new APKArchive("assets/Sample"));
#endif
}



void Game_Init()
{
	
}

void Game_Update()
{
}

void Game_Shutdown()
{
	
}

void Game_Pasue()
{
}

void Game_Resume()
{
}