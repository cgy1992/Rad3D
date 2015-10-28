#include "App.h"

Mesh * gMesh = NULL;

void LookMesh(Mesh* entity)
{
	Aabb bound = entity->GetWorldAabb();

	float size = 0;
	size = Max(size, bound.GetSize().x);
	size = Max(size, bound.GetSize().y);
	size = Max(size, bound.GetSize().z);

	Node * pCamera = World::Instance()->MainCamera();
	pCamera->SetPosition(bound.GetCenter() + Float3(0, 5, -8.0f) * size);
	pCamera->SetDirection(bound.GetCenter() - pCamera->GetPosition());
}

MeshSourcePtr CreateBoxMesh(float width, float height, float depth)
{
	MeshSourcePtr pMesh = MeshManager::Instance()->NewMesh("Box");

	MeshBuffer * sm = pMesh->NewMeshBuffer();

	int iVertexCount = 4 * 6;
	int iIndexCount = 12 * 3;
	int iPrimCount = 12;

	sm->GetRenderOp()->vertexDeclarations[0].AddElement(eVertexSemantic::POSITION, eVertexType::FLOAT3);
	sm->GetRenderOp()->vertexDeclarations[0].AddElement(eVertexSemantic::NORMAL, eVertexType::FLOAT3);
	sm->GetRenderOp()->vertexDeclarations[0].AddElement(eVertexSemantic::TEXCOORD0, eVertexType::FLOAT2);

	//VertexBuffer 1
	VertexBufferPtr buffer = HWBufferManager::Instance()->NewVertexBuffer(sizeof(float) * 8, iVertexCount);

	const float half_w = width * 0.5f;
	const float half_h = height * 0.5f;
	const float half_d = depth * 0.5f;
	float * v = (float *)buffer->Lock(eLockFlag::WRITE);
	{
#define d_pos(i) *v++ = all_pos[i].x, *v++ = all_pos[i].y, *v++ = all_pos[i].z
#define d_normal(i) *v++ = all_normal[i].x, *v++ = all_normal[i].y, *v++ = all_normal[i].z
#define d_uv(i) *v++ = all_uv[i].x, *v++ = all_uv[i].y

		Float3 pos, normal;

		//front
		normal = Float3(0, 0, -1);

		Float3 all_pos[8] = {
			Float3(-half_w,  half_h, -half_d),
			Float3( half_w,  half_h, -half_d),
			Float3(-half_w, -half_h, -half_d),
			Float3( half_w, -half_h, -half_d),

			Float3(-half_w,  half_h,  half_d),
			Float3( half_w,  half_h,  half_d),
			Float3(-half_w, -half_h,  half_d),
			Float3( half_w, -half_h,  half_d),
		};

		Float3 all_normal[6] = {
			Float3( 0,  0, -1),
			Float3( 0,  0,  1),

			Float3(-1,  0,  0),
			Float3( 1,  0,  0),

			Float3( 0,  1,  0),
			Float3( 0,  -1,  0)
		};

		Float2 all_uv[4] = {
			Float2(0, 0),
			Float2(1, 0),
			Float2(0, 1),
			Float2(1, 1)
		};

		// front
		d_pos(0), d_normal(0), d_uv(0);
		d_pos(1), d_normal(0), d_uv(1);
		d_pos(2), d_normal(0), d_uv(2);
		d_pos(3), d_normal(0), d_uv(3);

		// back
		d_pos(5), d_normal(1), d_uv(0);
		d_pos(4), d_normal(1), d_uv(1);
		d_pos(7), d_normal(1), d_uv(2);
		d_pos(6), d_normal(1), d_uv(3);

		// left
		d_pos(4), d_normal(2), d_uv(0);
		d_pos(0), d_normal(2), d_uv(1);
		d_pos(6), d_normal(2), d_uv(2);
		d_pos(2), d_normal(2), d_uv(3);

		// right
		d_pos(1), d_normal(3), d_uv(0);
		d_pos(5), d_normal(3), d_uv(1);
		d_pos(3), d_normal(3), d_uv(2);
		d_pos(7), d_normal(3), d_uv(3);

		// top
		d_pos(4), d_normal(4), d_uv(0);
		d_pos(5), d_normal(4), d_uv(1);
		d_pos(0), d_normal(4), d_uv(2);
		d_pos(1), d_normal(4), d_uv(3);

		// bottom
		d_pos(2), d_normal(5), d_uv(0);
		d_pos(3), d_normal(5), d_uv(1);
		d_pos(6), d_normal(5), d_uv(2);
		d_pos(7), d_normal(5), d_uv(3);

#undef d_pos
#undef d_normal
#undef d_uv

	}
	buffer->Unlock();

	sm->GetRenderOp()->vertexBuffers[0] = buffer;

	// VertexBuffer 2
	sm->GetRenderOp()->vertexDeclarations[1].AddElement(eVertexSemantic::COLOR, eVertexType::FLOAT4);

	VertexBufferPtr buffer2 = HWBufferManager::Instance()->NewVertexBuffer(sizeof(float) * 4, iVertexCount);
	Float4 * colorData = (Float4 *)buffer2->Lock(eLockFlag::WRITE);
	{
		Float4 c = Float4(1, 0, 0, 1);

		*colorData++ = c; *colorData++ = c; *colorData++ = c; *colorData++ = c;
		*colorData++ = c; *colorData++ = c; *colorData++ = c; *colorData++ = c;
		*colorData++ = c; *colorData++ = c; *colorData++ = c; *colorData++ = c;
		*colorData++ = c; *colorData++ = c; *colorData++ = c; *colorData++ = c;
		*colorData++ = c; *colorData++ = c; *colorData++ = c; *colorData++ = c;
		*colorData++ = c; *colorData++ = c; *colorData++ = c; *colorData++ = c;
	}
	buffer2->Unlock();
	sm->GetRenderOp()->vertexBuffers[1] = buffer2;

	// IndexBuffer
	IndexBufferPtr ibuffer = HWBufferManager::Instance()->NewIndexBuffer(iIndexCount);
	short * indices = (short *)ibuffer->Lock(eLockFlag::WRITE);
	for (int i = 0; i < 6; ++i)
	{
		int idx = i * 4;

		*indices++ = idx + 0;
		*indices++ = idx + 1;
		*indices++ = idx + 2;

		*indices++ = idx + 2;
		*indices++ = idx + 1;
		*indices++ = idx + 3;
	}
	ibuffer->Unlock();

	sm->GetRenderOp()->indexBuffer = ibuffer;

	sm->GetRenderOp()->primCount = iPrimCount;
	sm->GetRenderOp()->primType = ePrimType::TRIANGLE_LIST;

	sm->GetMaterial()->maps[eMapType::DIFFUSE] = RenderHelper::Instance()->GetWhiteTexture();

	pMesh->SetAabb(Aabb(Float3(-half_w, -half_h, -half_d), Float3(half_w, half_h, half_d)));

	pMesh->EnsureLoad();

	return pMesh;
}


class Demo10_MultiVertexBuffer : public App
{
public:
	Demo10_MultiVertexBuffer() {}
	virtual ~Demo10_MultiVertexBuffer() {}

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

		ShaderFX * pShaderFX = ShaderFXManager::Instance()->Load("Demo10_MultiVertexBuffer", "Demo10/MultiVertexBuffer.mfx");

		MeshSourcePtr pMeshSource = CreateBoxMesh(100, 100, 100);

		gMesh = new Mesh;
		gMesh->SetSource(pMeshSource);
		gMesh->SetShaderFX(pShaderFX);

		LookMesh(gMesh);

		World::Instance()->MainLight()->SetDirection(Float3(-1, 0, 1));
	}

	virtual void OnUpdate()
	{
	}

	virtual void OnShutdown()
	{
		delete gMesh;
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
	*ppApp = new Demo10_MultiVertexBuffer;
}
