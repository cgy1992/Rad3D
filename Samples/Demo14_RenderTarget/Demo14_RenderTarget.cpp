#include "App.h"
#include "MImage.h"

Mesh * gMesh = NULL;
Camera * gRTCamera = NULL;
RenderContext * gRenderContext = NULL;

void LookMesh(Mesh * entity)
{
	Aabb bound = entity->GetWorldAabb();

	float size = 0;
	size = Max(size, bound.GetSize().x);
	size = Max(size, bound.GetSize().y);
	size = Max(size, bound.GetSize().z);

	Node * pCamera = World::Instance()->MainCamera();
	pCamera->SetPosition(bound.GetCenter() + Float3(0, 1, -2) * size);
	pCamera->SetDirection(bound.GetCenter() - pCamera->GetPosition());
}

class Demo14_RenderTarget : public App
{
public:
	Demo14_RenderTarget() {}
	virtual ~Demo14_RenderTarget() {}

	virtual void OnSetupResource()
	{
#ifdef M_PLATFORM_WIN32
		ResourceManager::Instance()->AddArchive(new FilePathArchive("../Core"));
		ResourceManager::Instance()->AddArchive(new FilePathArchive("../Sample"));
#endif

#ifdef M_PLATFORM_ANDROID
		ResourceManager::Instance()->AddArchive(new APKArchive("Core"));
		ResourceManager::Instance()->AddArchive(new APKArchive("Sample"));
#endif
	}

	virtual void OnInit()
	{
		MGUI::FontManager::Instance()->Load("Sample.font");

		// Setup Main Light
		World::Instance()->MainLight()->SetDirection(Float3(-1, 0, 1));

		// Create Mesh
		MeshSourcePtr pMeshSource = MeshManager::Instance()->LoadMesh("Mesh/fox.mesh");

		gMesh = new Mesh;
		gMesh->SetSource(pMeshSource);
		gMesh->LoadAnimation("Idle1", "Mesh/fox_idle1.anim");
		gMesh->SetLighting(true);

		gMesh->PlayAnimation("Idle1");

		LookMesh(gMesh);

		// Setup RenderTarget
		const int RT_SIZE = 256;

		RenderTargetPtr pRenderTarget = HWBufferManager::Instance()->NewRenderTarget(RT_SIZE, RT_SIZE, eSizeAlign::NONE, ePixelFormat::R8G8B8);
		DepthBufferPtr pDepthBuffer = HWBufferManager::Instance()->NewDepthBuffer(RT_SIZE, RT_SIZE, eSizeAlign::NONE, ePixelFormat::D24);

		gRenderContext = World::Instance()->NewRenderContext(8, "RenderTarget1");
		gRenderContext->SetColorClear(eClearMode::ALL, Float4(0.3f, 0.3f, 0.3f));
		gRenderContext->SetRenderTarget(pRenderTarget);
		gRenderContext->SetDepthBuffer(pDepthBuffer);
		gRenderContext->SetVisibleCuller(new VisibleCullerStandard);
		gRenderContext->SetShaderProvider(new ShaderProviderStandard);
		gRenderContext->SetRenderPipeline(new RenderPipelineStandard);

		gRTCamera = new Camera();
		Node * pMainCameraNode = World::Instance()->MainCamera();
		gRTCamera->SetPosition(pMainCameraNode->GetPosition());
		gRTCamera->SetRotation(pMainCameraNode->GetRotation());
		gRenderContext->SetCamera(gRTCamera);

		// Create GUI
		MGUI::Layout * pLayout = new MGUI::Layout(NULL, NULL);
		pLayout->SetAlign(MGUI::eAlign::STRETCH);

		MGUI::ImageBox * pImageBox = new MGUI::ImageBox(NULL, pLayout);
		pImageBox->SetAlign(MGUI::eAlign::LEFT | MGUI::eAlign::TOP);
		pImageBox->SetSkinAlignedEx(pRenderTarget->GetTexture());

		pImageBox->E_MouseDoubleClick += new cListener1<Demo14_RenderTarget, const MGUI::MouseEvent *>(this, &Demo14_RenderTarget::OnClick);
	}

	void OnClick(const MGUI::MouseEvent *)
	{
		byte * pixels = new byte[256 * 256 * 4];

		gRenderContext->ReadPixelData(pixels, 0, 0, 256, 256);

		OSerializerF OS("Demo14_RenderTarget.png");
		if (OS.GetFile() != NULL)
		{
			PNG_Save(OS, pixels, 256, 256, ePixelFormat::R8G8B8A8);
		}

		delete[] pixels;
	}

	virtual void OnUpdate()
	{
		float time = Root::Instance()->GetFrameTime();

		gMesh->Yaw(time * 0.5f);
	}

	virtual void OnShutdown()
	{
		delete gMesh;
		delete gRTCamera;
		World::Instance()->DeleteRenderContext(4);
	}

	virtual void OnPause()
	{
	}

	virtual void OnResume()
	{
	}
};

void CreateApplication(App ** ppApp)
{
	*ppApp = new Demo14_RenderTarget;
}
