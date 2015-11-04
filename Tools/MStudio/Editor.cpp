#include "Editor.h"
#include "DragDrop.h"
#include "MainRenderView.h"
#include "App.h"
#include "ToolUtil.h"

ImplementSingleton(Editor);

Editor::Editor()
{
	mConfig = new ConfigFile;
	mConfig->Load("MStudio.ini");

	mEditorFactory = new EditorFactory;
	mUndoRedoManager = new UndoRedoManager;

	mNodeUID = 1;
	mFlags = 0;
	mCameraMode = eCameraMode::NORMAL;

	mInputMasker = new MGUI::Layout(NULL, NULL);
	mInputMasker->SetOrder(MGUI::eOrder::TOP);
	mInputMasker->SetAlign(MGUI::eAlign::STRETCH);
	mInputMasker->SetVisible(false);

	mGizmo = new Gizmo;

	World::Instance()->E_SectionAddNode += new cListener2<Editor, WorldSection *, Node *>(this, &Editor::OnSectionAddNode);
	World::Instance()->E_SectionRemoveNode += new cListener2<Editor, WorldSection *, Node *>(this, &Editor::OnSectionRemoveNode);
	World::Instance()->E_RenderSolidEnd += new cListener0<Editor>(this, &Editor::OnSectionRender);
}

Editor::~Editor()
{
	delete mGizmo;
	delete mInputMasker;
	delete mUndoRedoManager;
	delete mEditorFactory;

	mConfig->Save("MStudio.ini");
	delete mConfig;
}

void Editor::Update()
{
	if (!IsLoaded())
		return ;

	if (MainRenderView::Instance()->IsMouseFoucsed())
	{
		Float2 pt = IMouse::Instance()->GetPosition();
		Editor::Instance()->MapScreenUnit(pt.x, pt.y);
		mGizmo->Update(pt.x, pt.y);
	}

	Node * cam = World::Instance()->MainCamera();
	float frameTime = Root::Instance()->GetFrameTime();
	float camMoveStep = frameTime * UNIT_METRES * 10;
	float camRotateStep = frameTime * 0.1f;
	float camWheelStep = frameTime * UNIT_METRES;

	if (MGUI::InputManager::Instance()->GetKeyFocusedWidget() == NULL && !mInputMasker->IsVisible())
	{
		if (IKeyboard::Instance()->KeyPressed(InputCode::KC_F1))
		{
			cam->SetPosition(Float3(0, 0, 0));
		}

		if (IKeyboard::Instance()->KeyPressed(InputCode::KC_W))
		{
			if (mCameraMode == eCameraMode::NORMAL)
			{
				cam->Translate(Float3(0, 0, 1) * camMoveStep, eTransform::LOCAL);
			}
			else if (mCameraMode == eCameraMode::NO_Y)
			{
				Float3 dir = cam->GetWorldRotation().GetDirVector();

				dir.y = 0;
				dir.Normalize();

				cam->Translate(dir * camMoveStep, eTransform::WORLD);
			}
		}
		else if (IKeyboard::Instance()->KeyPressed(InputCode::KC_S))
		{
			if (mCameraMode == eCameraMode::NORMAL)
			{
				cam->Translate(Float3(0, 0, -1) * camMoveStep, eTransform::LOCAL);
			}
			else if (mCameraMode == eCameraMode::NO_Y)
			{
				Float3 dir = cam->GetWorldRotation().GetDirVector();

				dir.y = 0;
				dir.Normalize();

				cam->Translate(-dir * camMoveStep, eTransform::WORLD);
			}
		}
		else if (IKeyboard::Instance()->KeyPressed(InputCode::KC_A))
		{
			cam->Translate(Float3(-1, 0, 0) * camMoveStep, eTransform::LOCAL);
		}
		else if (IKeyboard::Instance()->KeyPressed(InputCode::KC_D))
		{
			cam->Translate(Float3(1, 0, 0) * camMoveStep, eTransform::LOCAL);
		}
		else if (IKeyboard::Instance()->KeyUp(InputCode::KC_DELETE))
		{
			if (mFlags & FLAG_DRAG_ABLE &&
				Editor::Instance()->GetSelectNode() &&
				Util_QuestDlg(App::Instance()->_hWnd(), "Delete Node?"))
			{
				Node * pNode = Editor::Instance()->GetSelectNode();

				SetSelectNode(NULL);

				E_NodeDestroy(pNode);

				RemoveNode(pNode);
			}
		}

		// parse 
		else if (IKeyboard::Instance()->KeyPressed(InputCode::KC_LCONTROL) && IKeyboard::Instance()->KeyUp(InputCode::KC_V))
		{
			if (mSelectNodes.Size() == 1)
			{
				Node * pNode = (Node *)(GetSelectNode()->Clone());
				if (pNode)
				{
					AddNode(pNode);
				}

				SetSelectNode(pNode);
			}
		}

		// undo
		else if (IKeyboard::Instance()->KeyPressed(InputCode::KC_LCONTROL) &&
			IKeyboard::Instance()->KeyUp(InputCode::KC_Z))
		{
			UndoRedoManager::Instance()->Undo();
		}

		// redo
		else if (IKeyboard::Instance()->KeyPressed(InputCode::KC_LCONTROL) &&
			IKeyboard::Instance()->KeyUp(InputCode::KC_Y))
		{
			UndoRedoManager::Instance()->Redo();
		}
		
		// console
		else if (IKeyboard::Instance()->KeyUp(InputCode::KC_GRAVE))
		{
			Console::Instance()->SetVisible(!Console::Instance()->IsVisible());
		}
	}

	if (MGUI::InputManager::Instance()->GetMouseFocusedWidget() == MainRenderView::Instance()->GetLayout())
	{
		if (IMouse::Instance()->MouseMoved() && IMouse::Instance()->KeyPressed(InputCode::MKC_RIGHT))
		{
			Float2 pt = IMouse::Instance()->GetPositionDiff();

			if (abs(pt.y) >abs(pt.x))
			{
				cam->Pitch(pt.y * camRotateStep, eTransform::LOCAL);
			}
			else
			{
				cam->Yaw(pt.x * camRotateStep, eTransform::PARENT);
			}
		}

		if (IMouse::Instance()->MouseWheel())
		{
			cam->Translate(Float3(0, 0, camWheelStep * IMouse::Instance()->MouseWheel()), eTransform::LOCAL);
		}
	}

	E_Update();
}

void Editor::LoadProject(const char * filename)
{
	UnloadProject();

	mEditorFloder = FileHelper::GetFileDir(filename) + "/Editor";
	ResourceManager::Instance()->AddArchive(new FilePathArchive(mEditorFloder));

	xml_doc doc;
	if (doc.open_file(filename))
	{
		xml_node node = doc.first_node().first_node("MediaPath");

		while (node != NULL)
		{
			String path = node.first_attribute("value");

			ResourceManager::Instance()->AddArchive(new FilePathArchive(path));

			mMediaPathArray.PushBack(path);

			node = node.next_sibling("MediaPath");
		}

		E_LoadSetting(doc);
	}

	E_ProjectLoaded();
}

void Editor::UnloadProject()
{
	E_ProjectUnload();

	for (int i = 0; i < mMediaPathArray.Size(); ++i)
	{
		ResourceManager::Instance()->RemoveArchive(mMediaPathArray[i]);
	}
	mMediaPathArray.Clear();

	if (mEditorFloder != "")
		ResourceManager::Instance()->RemoveArchive(mEditorFloder);
}

void Editor::LoadScene(const char * scene)
{
	UnloadScene();

	// do ...
	mSceneFilename = scene;

	String filename = ResourceManager::Instance()->GetRelativeSourceName(mSceneFilename);

	if (filename != "")
	{
		FILE * fp = fopen(mSceneFilename.c_str(), "rb");
		if (fp)
		{
			fread(&mNodeUID, 1, 4, fp);
			fclose(fp);

			World::Instance()->Load(filename);
		}
	}

	E_SceneLoaded();
}

void Editor::UnloadScene()
{
	SetSelectNode(NULL);

	E_SceneUnload();

	if (IsLoaded())
	{
		World::Instance()->Unload();
	}
}

void Editor::SaveScene()
{
	if (!IsLoaded())
		return ;

	FILE * fp = fopen(mSceneFilename.c_str(), "wb");
	if (fp)
	{
		fwrite(&mNodeUID, 1, 4, fp);
		fclose(fp);

		World::Instance()->Save(mSceneFilename);
	}

	DoMessageBox("Save scene success!");
}

void Editor::SavePrefab()
{
	if (!IsLoaded())
		return ;

	const Array<Node *> & nodes = mSelectNodes;
	if (nodes.Size() > 0)
	{
		String filename = mSceneFilename;
		filename = FileHelper::RemoveExternName(filename);
		filename += ".prefab";
		Prefab::SavePrefab(nodes, filename);

		DoMessageBox("Save prefab success!");
	}
	else
	{
		DoMessageBox("No select nodes!");
	}
}

bool Editor::IsLoaded()
{
	return World::Instance()->GetFilename() != "";
}

const String & Editor::GetSceneFilename()
{
	return mSceneFilename;
}

Float3 Editor::GetHitPosition(float x, float y)
{
	Ray ray = World::Instance()->MainCamera()->GetViewportRay(x, y);

	return ray.orig + ray.dir * 20 * UNIT_METRES;
}

void Editor::SetSelectNode(Node * obj)
{
	if (GetSelectNode() != obj)
	{
		mSelectNodes.Clear();

		AddSelectNode(obj);
	}
}

void Editor::AddSelectNode(Node * obj)
{
	for (int i = 0; i < mSelectNodes.Size(); ++i)
	{
		if (mSelectNodes[i] == obj)
			return ;
	}

	if (obj != NULL)
	{
		mSelectNodes.PushFront(obj);
	}

	E_NodeSelect();
}

Node * Editor::GetSelectNode()
{
	return mSelectNodes.Size() > 0 ? mSelectNodes[0] : NULL;
}

Node * Editor::GetSelectNode(int i)
{
	return mSelectNodes[i];
}

int Editor::GetSelectNodeCount()
{
	return mSelectNodes.Size();
}

void Editor::MaskUserInput(bool mask)
{
	mInputMasker->SetVisible(mask);
}

bool Editor::IsMaskUserInput()
{
	return mInputMasker->IsVisible();
}

void Editor::MapScreenUnit(float & x, float & y)
{
	Float2 sz = MGUI::Engine::Instance()->GetInvSize();

	x *= sz.x;
	y *= sz.y;
}
void Editor::OnDragFile(const char * filename)
{
	if (mFlags & FLAG_DRAG_ABLE)
	{
		Float2 pt = IMouse::Instance()->GetPosition();
		if (MGUI::Engine::Instance()->GetWidget(pt.x, pt.y) != MainRenderView::Instance()->GetLayout())
			return ;

		Editor::Instance()->MapScreenUnit(pt.x, pt.y);

		String fullName = filename;
		fullName.Replace('/', '\\');

		String typeName = FileHelper::GetExternName(fullName);
		typeName.ToLower();

		const char * testFile = fullName.c_str();
		int length = fullName.Length();

		bool exist = false;

		while (length > 0)
		{
			if (testFile[length - 1] == '\\')
			{
				const char * tfile = testFile + length;

				exist = ResourceManager::Instance()->OpenResource(tfile) != NULL;

				if (exist)
					break;
			}

			--length;
		}

		if (!exist)
			return ;

		testFile += length;

		Node * pNode = NULL;

		Array<EntityFactory *> & factoryArray = EditorFactory::Instance()->GetEntityFactorys();
		for (int i = 0; i < factoryArray.Size() && pNode == NULL; ++i)
		{
			pNode = factoryArray[i]->NewObjectByDrag(testFile, typeName.c_str());
		}

		if (pNode != NULL)
		{
			Float3 pos = Editor::Instance()->GetHitPosition(pt.x, pt.y);

			pNode->SetPosition(pos);

			Editor::Instance()->SetSelectNode(pNode);
		}
	}
}

void Editor::OnPickNode()
{
	Float2 pt = IMouse::Instance()->GetPosition();
	Editor::Instance()->MapScreenUnit(pt.x, pt.y);

	if (pt.x <= 0 || pt.x >= 1 ||
		pt.y <= 0 || pt.y >= 1)
		return ;

	float dist = 0;
	Ray ray = World::Instance()->MainCamera()->GetViewportRay(pt.x, pt.y);
	const float ray_len = 100 * UNIT_METRES;

	Node * pSelectedShape = Editor::Instance()->RayCheck(dist, ray, ray_len, EDITOR_NODE_PICK);
	if (pSelectedShape != NULL)
	{
		SetSelectNode(pSelectedShape);
	}
}

void Editor::OnPickNodeRect(const MGUI::Rect & rc)
{
	Float3 camPos = World::Instance()->MainCamera()->GetWorldPosition();
	Mat4 vp = World::Instance()->MainCamera()->GetViewProjMatrix();
	float distSq = MAX_FLOAT;

	SetSelectNode(NULL);

	Array<Node *> nodes;
	World::Instance()->GetSectionNodes(nodes);
	for (int i = 0; i < nodes.Size(); ++i)
	{
		Node * n = nodes[i];
		if (n->GetFlags() & EDITOR_NODE_PICK)
		{
			Float3 pos = n->GetWorldPosition();

			pos *= vp;
			pos.x = (pos.x + 1) * 0.5f;
			pos.y = (1 - pos.y) * 0.5f;

			pos.x *= MGUI::Engine::Instance()->GetRect().w;
			pos.y *= MGUI::Engine::Instance()->GetRect().h;

			if (pos.x > rc.x && pos.x < rc.x + rc.w &&
				pos.y > rc.y && pos.y < rc.y + rc.h)
			{
				AddSelectNode(n);
			}
		}
	}
}

Node * Editor::RayCheck(float & dist, const Ray & ray, float length, int flags)
{
	Array<RayCheckContract> contractArray;
	World::Instance()->MainZone()->RayCheck(contractArray, ray, length, flags);

	if (contractArray.Size() == 0)
		return NULL;

	RayCheckContract::SortOp sortOp;
	Sort(&contractArray[0], contractArray.Size(), sortOp);

	Node * pSelectedNode = NULL;
	float k_len = MAX_FLOAT;

	for (int i = 0; i < contractArray.Size(); ++i)
	{
		Node * pNode = contractArray[i].node;

		if (pNode == NULL)
			continue;

		if (k_len < contractArray[i].contract_dist)
			continue;

		float c_len = MAX_FLOAT;
		if (pNode->GetShape() != NULL)
		{
			pNode->GetShape()->OnRayCheck(c_len, ray, length);
		}
		else
		{
			c_len = contractArray[i].contract_dist;
		}

		if (c_len < k_len)
		{
			pSelectedNode = pNode;
			k_len = c_len;
		}
	}

	dist = k_len;

	return pSelectedNode;
}

bool Editor::RayCheckEx(float & dist, const Ray & ray, float length, int flags)
{
	if (RayCheck(dist, ray, length, flags) != NULL)
	{
		return true;
	}
	else
	{
		Float3 point;
		if (Terrain::Instance()->RayCheck(&point, ray))
		{
			dist = point.Distance(ray.orig);

			return dist <= length;
		}
	}

	return false;
}

void Editor::AddNode(Node * node)
{
	WorldSection * section = World::Instance()->GetBestSection(node->GetWorldPosition());
	d_assert (section != NULL);

	node->SetUID(mNodeUID++);

	section->AddNode(node);
}

void Editor::RemoveNode(Node * node)
{
	d_assert (node->GetSection() != NULL);

	node->GetSection()->RemoveNode(node, true);
}

Node * Editor::GetNodeByUID(int UID)
{
	Array<Node *> nodes;
	World::Instance()->GetSectionNodes(nodes);

	for (int i = 0; i < nodes.Size(); ++i)
	{
		if (nodes[i]->GetUID() == UID)
			return nodes[i];
	}

	return NULL;
}

void Editor::OnSectionAddNode(WorldSection * section, Node * node)
{
	if (KIND_OF(Mesh, node))
	{
		Mesh * mesh = (Mesh *)node;
		node->SetShape(new PhyMesh(PhyEntity::STATIC_BODY));
	}
	else if (KIND_OF(Light, node))
	{
		node->SetLocalAabb(Aabb::Default);
		node->AddComponent(new NodeDisplayer("Light.png"));
	}
	else if (KIND_OF(Sound, node))
	{
		node->SetLocalAabb(Aabb::Default);
		node->AddComponent(new NodeDisplayer("Sound.png"));
	}

	node->SetFlags(EDITOR_NODE_FLAG);
}

void Editor::OnSectionRemoveNode(WorldSection * section, Node * node)
{
}

void Editor::OnSectionRender()
{
	const World::Info * wi = World::Instance()->GetInfo();

	WorldSection * pSection = World::Instance()->GetCurrentSection();
	if (pSection)
	{
		const Aabb & bound = pSection->GetBound();

		RenderSystem::Instance()->SetRenderState(eFillMode::SOLID, eCullMode::NONE);

		// bottom
		RectF rect;
		rect.x1 = 0;
		rect.y1 = 0;
		rect.x2 = bound.GetSize().x;
		rect.y2 = bound.GetSize().z;

		Mat4 form;
		form.MakeTranslate(bound.minimum.x, bound.minimum.y, bound.minimum.z);

		RenderHelper::Instance()->DebugDrawRectFrames(rect, wi->SectionZoneTile.x, wi->SectionZoneTile.z, Float4(0, 1, 0), form);

		// top
		rect.x1 = 0;
		rect.y1 = 0;
		rect.x2 = bound.GetSize().x;
		rect.y2 = bound.GetSize().z;

		form.MakeTranslate(bound.minimum.x, bound.maximum.y, bound.minimum.z);

		RenderHelper::Instance()->DebugDrawRectFrames(rect, wi->SectionZoneTile.x, wi->SectionZoneTile.z, Float4(0, 1, 0), form);

		// left
		rect.x1 = 0;
		rect.y1 = 0;
		rect.x2 = bound.GetSize().y;
		rect.y2 = bound.GetSize().x;

		Quat q;
		q.FromAxis(Float3(0, 0, 1), PI / 2);
		form.MakeTransform(Float3(bound.minimum.x, bound.minimum.y, bound.minimum.z), q, Float3(1, 1, 1));

		RenderHelper::Instance()->DebugDrawRectFrames(rect, wi->SectionZoneTile.y, wi->SectionZoneTile.x, Float4(0, 1, 0), form);

		// right
		rect.x1 = 0;
		rect.y1 = 0;
		rect.x2 = bound.GetSize().y;
		rect.y2 = bound.GetSize().z;

		q.FromAxis(Float3(0, 0, 1), PI / 2);
		form.MakeTransform(Float3(bound.maximum.x, bound.minimum.y, bound.minimum.z), q, Float3(1, 1, 1));

		RenderHelper::Instance()->DebugDrawRectFrames(rect, wi->SectionZoneTile.y, wi->SectionZoneTile.z, Float4(0, 1, 0), form);

		// front
		rect.x1 = 0;
		rect.y1 = 0;
		rect.x2 = bound.GetSize().x;
		rect.y2 = bound.GetSize().y;

		q.FromAxis(Float3(1, 0, 0), -PI / 2);
		form.MakeTransform(Float3(bound.minimum.x, bound.minimum.y, bound.minimum.z), q, Float3(1, 1, 1));

		RenderHelper::Instance()->DebugDrawRectFrames(rect, wi->SectionZoneTile.x, wi->SectionZoneTile.y, Float4(0, 1, 0), form);

		// back
		rect.x1 = 0;
		rect.y1 = 0;
		rect.x2 = bound.GetSize().x;
		rect.y2 = bound.GetSize().y;

		q.FromAxis(Float3(1, 0, 0), -PI / 2);
		form.MakeTransform(Float3(bound.minimum.x, bound.minimum.y, bound.maximum.z), q, Float3(1, 1, 1));

		RenderHelper::Instance()->DebugDrawRectFrames(rect, wi->SectionZoneTile.x, wi->SectionZoneTile.y, Float4(0, 1, 0), form);
	}
}