#include <fstream>
#include "App.h"
#include "RenderViewer.h"
#include "CameraController.h"
#include "MMeshSerializer.h"
#include "MAnimationSerializer.h"
#include "SkeletonDisplayer.h"
#include "ToolScaleMesh.h"
#include "MUtil.h"

Mesh * gMesh = NULL;
ParticleSystem * gParticle = NULL;
Node * gNode = NULL;
String gMeshFilePath = "";
float gSize = 0;

class MeshViewer : public App
{
	RenderViewer * mRenderViewer;
	CameraController * mCameraController;
	MGUI::Layout * mLayout;
	MGUI::ListBox * mListBox;
	SkeletonDisplayer * mSkeletonDisplayer;
	bool mShowSkeleton;
	ToolScaleMesh * mToolScaleMesh;

public:
	MeshViewer() {}
	virtual ~MeshViewer() {}

	virtual void OnSetupResource()
	{
		ResourceManager::Instance()->AddArchive(new FilePathArchive("../Core"));
		ResourceManager::Instance()->AddArchive(new FilePathArchive("../Core/Editor"));

		ResourceManager::Instance()->AddArchive(new FilePathArchive("../MeshViewer"));

		ConfigFile config;
		if (config.Load("MeshViewer.ini"))
		{
			rml_node * node = config.first_node("Media");
			while (node != NULL)
			{
				String path = node->value();

				ResourceManager::Instance()->AddArchive(new FilePathArchive(path));

				node = node->next_sibling("Media");
			}
		}
	}

	virtual void OnInit()
	{
		CharSet::SetCharSet("gb2312");

		MGUI::FontManager::Instance()->Load("Editor.font");
		MGUI::LookFeelManager::Instance()->Load("Editor.lookfeel");

		World::Instance()->MainRenderContext()->SetColorClear(eClearMode::ALL, Float4(0.3f, 0.3f, 0.3f));

		gNode = new Node;
		gNode->Yaw(PI);

		mRenderViewer = new RenderViewer;
		mCameraController = new CameraController;
		mSkeletonDisplayer = new SkeletonDisplayer;
		mToolScaleMesh = new ToolScaleMesh;

		mLayout = MGUI::Layout::Load("MeshViewer.layout", NULL); d_assert(mLayout);
		MGUI::Button * bnDelete = (MGUI::Button *)mLayout->GetChild("Remove");
		MGUI::Button * bnSave = (MGUI::Button *)mLayout->GetChild("Save");
		MGUI::Button * bnStop = (MGUI::Button *)mLayout->GetChild("Stop");
		MGUI::Button * bnDump = (MGUI::Button *)mLayout->GetChild("DumpSkeleton");
		MGUI::Button * bnShow = (MGUI::Button *)mLayout->GetChild("ShowSkeleton");
		MGUI::Button * bnScale = (MGUI::Button *)mLayout->GetChild("ScaleMesh");
		mListBox = (MGUI::ListBox *)mLayout->GetChild("List");

		bnSave->E_Click += new cListener1<MeshViewer, const MGUI::ClickEvent *>(this, &MeshViewer::OnSave);
		bnDelete->E_Click += new cListener1<MeshViewer, const MGUI::ClickEvent *>(this, &MeshViewer::OnDelete);
		bnDump->E_Click += new cListener1<MeshViewer, const MGUI::ClickEvent *>(this, &MeshViewer::OnDumpSkeleton);
		bnShow->E_Click += new cListener1<MeshViewer, const MGUI::ClickEvent *>(this, &MeshViewer::OnShowSkeleton);
		bnStop->E_Click += new cListener1<MeshViewer, const MGUI::ClickEvent *>(this, &MeshViewer::OnStop);
		bnScale->E_Click += new cListener1<MeshViewer, const MGUI::ClickEvent *>(this, &MeshViewer::OnScaleMesh);
		mListBox->E_SelectChanged += new cListener2<MeshViewer, const MGUI::Event *, int>(this, &MeshViewer::OnAnimChanged);
	}

	virtual void OnShutdown()
	{
		delete mToolScaleMesh;
		delete mSkeletonDisplayer;
		delete mCameraController;
		delete mRenderViewer;

		safe_delete(gMesh);
		safe_delete(gParticle);
		safe_delete(gNode);
	}

	void OnSave(const MGUI::ClickEvent * e)
	{
		if (gMesh == NULL || gMesh->GetSource() == NULL)
			return ;

		String filename = Util_GetSaveFile("", "mesh(*.mesh)\0*.mesh\0");
		if (filename != "")
		{
			MeshSerializer::Save(gMesh->GetSource().c_ptr(), filename);
		}
	}

	void OnDelete(const MGUI::ClickEvent * e)
	{
		int index = mListBox->GetSelectIndex();
		if (index < 0)
			return ;

		if (gMesh == NULL || gMesh->GetSkeleton() == NULL)
			return ;

		gMesh->StopAnimation();

		String name;
		name.FromUnicode(mListBox->GetText(index).c_str());
		gMesh->GetSource()->DeleteAnimation(name.c_str());

		OnMeshChanged();
	}

	void OnStop(const MGUI::ClickEvent * e)
	{
		if (gMesh == NULL || gMesh->GetSkeleton() == NULL)
			return ;

		gMesh->StopAnimation();
	}

	void OnDumpSkeleton(const MGUI::ClickEvent * e)
	{
		if (gMesh == NULL || gMesh->GetSkeleton() == NULL)
			return ;

		std::ofstream file;
		file.open("MeshViewer.dump");

		if (!file.is_open())
			return ;

		xml_doc doc;

		gMesh->GetSkeleton()->SaveXml(gMesh->GetSource(), doc.append_node("Skeleton"));

		doc.print(file);
		
		file.close();
	}

	void OnShowSkeleton(const MGUI::ClickEvent * e)
	{
		if (gMesh == NULL || gMesh->GetSkeleton() == NULL)
			return ;

		if (mSkeletonDisplayer->GetSkeleton() != NULL)
		{
			mSkeletonDisplayer->Attach(NULL);

			gMesh->SetOpacity(1);
		}
		else
		{
			mSkeletonDisplayer->Attach(gMesh);

			gMesh->SetOpacity(0.3f);
		}
	}

	void OnScaleMesh(const MGUI::ClickEvent * e)
	{
		if (gMesh != NULL && gMesh->GetSource() != NULL)
		{
			mToolScaleMesh->Show(gMesh->GetSource());
		}
	}

	void OnAnimChanged(const MGUI::Event * e, int index)
	{
		if (index < 0 || gMesh == NULL)
			return ;

		String name;
		name.FromUnicode(mListBox->GetText(index).c_str());

		gMesh->PlayAnimation(name.c_str());
	}

	void OnMeshChanged()
	{
		mListBox->Clear();

		if (gMesh == NULL || gMesh->GetSource() == NULL)
			return ;

		for (int i = 0; i < gMesh->GetSource()->GetAnimationCount(); ++i)
		{
			Animation * anim = gMesh->GetSource()->GetAnimation(i);

			mListBox->Append(anim->GetName().c_wstr());
		}
	}

	virtual void OnUpdate()
	{
		float frameTime = Root::Instance()->GetFrameTime();
		float camWheelStep = frameTime * gSize * 0.2f;

		if (mRenderViewer->IsMouseFocused())
		{
			float frameTime = Root::Instance()->GetFrameTime();

			if (IMouse::Instance()->MouseWheel())
			{
				mCameraController->Scroll(camWheelStep * IMouse::Instance()->MouseWheel());
			}

			if (IMouse::Instance()->MouseMoved() && IMouse::Instance()->KeyPressed(InputCode::MKC_RIGHT))
			{
				Float2 pt = IMouse::Instance()->GetPositionDiff();

				float degree = pt.y * frameTime * 12.0f;

				mCameraController->Pitch(degree);

				degree = pt.x * frameTime * 12.0f;

				mCameraController->Yaw(degree);
			}
		}

		mCameraController->ForceUpdate();
	}

	virtual void OnPause()
	{
	}

	virtual void OnResume()
	{
	}

	virtual void OnResize(int w, int h)
	{
		mRenderViewer->Layout();
	}

	virtual void OnDragFile(const char * filename)
	{
		String fileExt = FileHelper::GetExternName(filename);
		fileExt.ToLower();

		if (fileExt == "anim")
		{
			if (gMesh == NULL || gMesh->GetSource() == NULL)
				return ;

			DataStreamPtr stream = ResourceManager::Instance()->OpenFile(filename);
			if (stream == NULL)
				return ;

			String animName = FileHelper::GetBaseName(filename);
			animName = FileHelper::RemoveExternName(animName);

			if (gMesh->GetSource()->GetAnimation(animName.c_str()) != NULL)
				return ;

			Animation * anim = gMesh->GetSource()->NewAnimation(animName.c_str());
			AnimationSerializer::Load(anim, stream);

			OnMeshChanged();

			return ;
		}

		safe_delete(gMesh);
		safe_delete(gParticle);
		mSkeletonDisplayer->Attach(NULL);

		if (gMeshFilePath != "")
			ResourceManager::Instance()->RemoveArchive(gMeshFilePath);

		OnMeshChanged();

		String file = ResourceManager::Instance()->GetRelativeSourceName(filename);
		if (file == "")
		{
			gMeshFilePath = FileHelper::GetFileDir(filename);

			if (gMeshFilePath == "")
				return ;

			file = FileHelper::GetBaseName(filename);
			ResourceManager::Instance()->AddArchive(new FilePathArchive(gMeshFilePath));
		}

		Aabb bound = Aabb::Identiy;

		if (fileExt == "mesh")
		{
			MeshSourcePtr pSource = MeshManager::Instance()->LoadMesh(file, -1);

			gMesh = new Mesh();
			gMesh->SetSource(pSource);
			gNode->Attach(gMesh);

			bound = gMesh->GetWorldAabb();
		}
		else if (fileExt == "particle")
		{
			PS_SourcePtr pSource = PS_Manager::Instance()->LoadSource(file, -1);

			gParticle = new ParticleSystem;
			gParticle->SetSource(pSource);
			gNode->Attach(gParticle);

			bound = gParticle->GetWorldAabb();
		}

		gSize = 0;
		gSize = Max(gSize, bound.GetSize().x);
		gSize = Max(gSize, bound.GetSize().y);
		gSize = Max(gSize, bound.GetSize().z);

		if (gSize == 0)
			gSize = 100.0f;

		mCameraController->SetLookAt(bound.GetCenter());
		mCameraController->SetScroll(gSize * 5.0f);
		mCameraController->SetYaw(0);

		OnMeshChanged();
	}
};

void CreateApplication(App ** ppApp)
{
	*ppApp = new MeshViewer;
}
