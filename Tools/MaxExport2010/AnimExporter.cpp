#include "stdafx.h"
#include "AnimExporter.h"
#include "ExportConfig.h"
#include "MMeshSerializer.h"
#include "MAnimationSerializer.h"

AnimExporter::AnimExporter(IGameScene * gs)
	: MeshExporter(gs)
{
}

AnimExporter::~AnimExporter()
{
}

void AnimExporter::Export()
{
	mRoot = new Root;
	mRenderSystem = new NullRenderSystem;
	mWorld = new World;

	try 
	{
		mGameScene = GetIGameInterface();
		IGameConversionManager* cm = GetConversionManager();
		cm->SetCoordSystem(IGameConversionManager::IGAME_D3D);
		mGameScene->InitialiseIGame(ExportConfig::Instance()->IsExportSelected());
		mGameScene->SetStaticFrame(0);

		int nodeCount = mGameScene->GetTopLevelNodeCount();
		if (nodeCount == 0)
		{
			MessageBox(GetActiveWindow(), "No nodes available!", "Error", MB_OK);
			goto __end;
		}

		mMeshSource = MeshManager::Instance()->NewMesh("MaxExporter");

		// extract skeleton
		for (int i = 0; i < nodeCount; ++i)
		{
			IGameNode* node = mGameScene->GetTopLevelNode(i);

			if (node->IsNodeHidden())
				continue ;

			ExtractSkeleton(node);
		}

		if (mMeshSource->GetAnimationCount() == 0)
		{
			MessageBox(GetActiveWindow(), "No animation!", "Error", MB_ICONEXCLAMATION);
			goto __end;
		}

		// save animation
		AnimationSerializer::Save(mMeshSource->GetAnimation(0), ExportConfig::Instance()->GetExportFilename());
	}
	catch (...) 
	{
		MessageBox(GetActiveWindow(), "Error!", "Error", MB_ICONEXCLAMATION);
	}

__end:
	mMeshSource = NULL;

	delete mWorld;
	delete mRenderSystem;
	delete mRoot;

	if (mGameScene)
	{
		mGameScene->ReleaseIGame();
		mGameScene = NULL;
	}
}