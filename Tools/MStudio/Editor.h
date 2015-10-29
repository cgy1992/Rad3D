/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MStudioEntry.h"
#include "App.h"
#include "UndoRedo.h"
#include "MainFrame.h"
#include "Gizmo.h"
#include "EditorFactory.h"

#define FLAG_DRAG_ABLE 0x0001
#define FLAG_PICK_ABLE 0x0002
#define FLAG_ALL 0xFFFF

struct eCameraMode {
	enum {
		NORMAL,
		NO_Y,
	};
};

class MSTUDIO_ENTRY Editor : public Singleton<Editor>
{
public:
	tEvent0 E_Init;
	tEvent0 E_Shutdown;
	tEvent0 E_Update;

	tEvent0 E_ProjectLoaded;
	tEvent0 E_ProjectUnload;
	tEvent1<xml_doc &> E_LoadSetting;

	tEvent0 E_SceneLoaded;
	tEvent0 E_SceneUnload;

	tEvent1<Node *> E_NodeCreate;
	tEvent1<Node *> E_NodeDestroy;
	tEvent0 E_NodeSelect;
	tEvent0 E_NodePositionChanged;
	tEvent0 E_NodeRotationChanged;
	tEvent0 E_NodeScaleChanged;

public:
	Editor();
	virtual ~Editor();

	void
		Update();

	void 
		LoadProject(const char * filename);
	void 
		UnloadProject();

	void 
		LoadScene(const char * scene);
	void 
		UnloadScene();
	void 
		SaveScene();
	void 
		SavePrefab();

	bool 
		IsLoaded();

	ConfigFile *
		GetConfig() { return mConfig; }

	const String &
		GetSceneFilename();

	Float3 
		GetHitPosition(float x, float y);

	void 
		SetSelectNode(Node * obj);
	void
		AddSelectNode(Node * obj);
	Node * 
		GetSelectNode();
	Node *
		GetSelectNode(int i);
	int
		GetSelectNodeCount();

	void 
		MaskUserInput(bool mask);
	bool 
		IsMaskUserInput();

	void
		SetFlags(int flag) { mFlags |= flag; }
	void
		ClearFlags(int flag) { mFlags &= ~flag; }
	int
		GetFlags() { return mFlags; }

	void
		SetCameraMode(int type) { mCameraMode = type; }
	int
		GetCameraMode() { return mCameraMode; }

	void
		MapScreenUnit(float & x, float & y);

	void
		OnDragFile(const char * filename);
	void
		OnPickNode();
	void
		OnPickNodeRect(const MGUI::Rect & rc);

	Node *
		RayCheck(float & dist, const Ray & ray, float length, int flags);

	bool 
		RayCheckEx(float & dist, const Ray & ray, float length, int flags);

	void
		AddNode(Node * node);
	void
		RemoveNode(Node * node);
	Node *
		GetNodeByUID(int UID);

protected:
	void
		OnSectionAddNode(WorldSection * section, Node * node);
	void
		OnSectionRemoveNode(WorldSection * section, Node * node);
	void
		OnSectionRender();

protected:
	String mEditorFloder;
	String mSceneFilename;
	Array<String> mMediaPathArray;

	ConfigFile * mConfig;
	MGUI::Layout * mInputMasker;
	EditorFactory * mEditorFactory;
	UndoRedoManager * mUndoRedoManager;

	int mNodeUID;
	int mFlags;
	int mCameraMode;
	Gizmo * mGizmo;
	Array<Node *> mSelectNodes;
};

