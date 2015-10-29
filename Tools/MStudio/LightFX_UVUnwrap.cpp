#include "LightFX_UVUnwrap.h"
#include "App.h"
#include <d3d9.h>
#include <d3dx9.h>

namespace Rad {

	LPDIRECT3D9 gDirect3D = NULL;
	LPDIRECT3DDEVICE9 gDevice = NULL;

	void FX_Unwarp_Init()
	{
		gDirect3D = Direct3DCreate9(D3D_SDK_VERSION);

		D3DPRESENT_PARAMETERS param;
		memset(&param, 0, sizeof (D3DPRESENT_PARAMETERS));

		param.hDeviceWindow				    = App::Instance()->_hWnd();
		param.Windowed					    = TRUE;
		param.SwapEffect					= D3DSWAPEFFECT_DISCARD;
		param.BackBufferCount				= 1;
		param.BackBufferWidth				= 16;
		param.BackBufferHeight			    = 16;
		param.EnableAutoDepthStencil		= FALSE;
		param.Flags                         = 0;

		param.PresentationInterval			= D3DPRESENT_INTERVAL_DEFAULT;
		param.BackBufferFormat		        = D3DFMT_UNKNOWN;
		param.FullScreen_RefreshRateInHz	= 0;

		param.MultiSampleType               = D3DMULTISAMPLE_NONE;
		param.MultiSampleQuality            = 0;


		HRESULT hr;
		hr = gDirect3D->CreateDevice(0, D3DDEVTYPE_NULLREF,
			NULL, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &param, &gDevice);

		d_assert (hr == D3D_OK);
	}

	void FX_Unwarp_Shutdown()
	{
		if (gDevice)
		{
			gDevice->Release();
			gDevice = NULL;
		}

		if (gDirect3D)
		{
			gDirect3D->Release();
			gDevice = NULL;
		}
	}

	HRESULT __stdcall UVAtlasCallback(FLOAT fPercentDone, LPVOID lpUserContext)
	{
		return S_OK;
	}

	bool FX_Unwarp(FX_Mesh * fxmesh)
	{
		// Convert Mesh
		D3DVERTEXELEMENT9 vertexFormat[] = {
			{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
			{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
			{0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
			{0, 32, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
			{0, 40, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2},

			D3DDECL_END(),
		};

		LPD3DXMESH mesh = NULL;
		D3DXCreateMesh(fxmesh->NumOfTriangles(), fxmesh->NumOfVertices(), 0, vertexFormat, gDevice, &mesh);
		if (mesh == NULL)
		{
			return false;
		}

		int vertexCount = mesh->GetNumVertices();
		int faceCount = mesh->GetNumFaces();

		FX_Vertex * fx_v;
		FX_Triangle * fx_t;
		FX_Material * fx_m;
		fxmesh->Lock(&fx_v, &fx_t, &fx_m);

		FX_Vertex * vtx;
		mesh->LockVertexBuffer(0, (void**)&vtx);
		for (int i = 0; i < vertexCount; ++i)
		{
			*vtx++ = fx_v[i];
		}
		mesh->UnlockVertexBuffer();

		unsigned short * idx;
		mesh->LockIndexBuffer(0, (void**)&idx);
		for (int i = 0; i < faceCount; ++i)
		{
			*idx++ = fx_t[i].Index0;
			*idx++ = fx_t[i].Index1;
			*idx++ = fx_t[i].Index2;
		}
		mesh->UnlockIndexBuffer();

		DWORD * attri;
		mesh->LockAttributeBuffer(0, &attri);
		for (int i = 0; i < faceCount; ++i)
		{
			attri[i] = fx_t[i].MaterialId;
		}
		mesh->UnlockAttributeBuffer();

		fxmesh->Unlock();

		const UINT MaxCharts = 0;
		const float AllowedStretch = 0.25f;
		const float GutterSize = 2.0f;
		float MaxStretch;
		UINT NumCharts;
		const UINT TextureWidth = fxmesh->GetLightingMapWidth();
		const UINT TextureHeight = fxmesh->GetLightingMapHeight();

		DWORD *pAdjacency = new DWORD[faceCount * 3];
		mesh->GenerateAdjacency(0, pAdjacency);

		LPD3DXBUFFER FacePartitioningBuffer = NULL, VertexRemapArrayBuffer = NULL;

		// Create UV Atlas.
		LPD3DXMESH n_mesh = NULL;
		D3DXUVAtlasCreate(
			mesh,
			MaxCharts,
			AllowedStretch,
			TextureWidth,
			TextureHeight,
			GutterSize,
			1,
			pAdjacency,
			NULL,
			NULL,
			UVAtlasCallback,
			0.0001f,
			NULL,
			D3DXUVATLAS_DEFAULT,
			&n_mesh,
			&FacePartitioningBuffer,
			&VertexRemapArrayBuffer,
			&MaxStretch,
			&NumCharts
			);

		delete[] pAdjacency;
		FacePartitioningBuffer->Release();
		VertexRemapArrayBuffer->Release();

		mesh->Release();

		// Fetch Result.
		vertexCount = n_mesh->GetNumVertices();
		faceCount = n_mesh->GetNumFaces();

		fxmesh->Alloc(vertexCount, faceCount, -1);
		fxmesh->Lock(&fx_v, &fx_t, &fx_m);

		n_mesh->LockVertexBuffer(0, (void**)&vtx);
		for (int i = 0; i < vertexCount; ++i)
		{
			fx_v[i] = *vtx++;
		}
		n_mesh->UnlockVertexBuffer();

		n_mesh->LockIndexBuffer(0, (void**)&idx);
		for (int i = 0; i < faceCount; ++i)
		{
			fx_t[i].Index0 = *idx++;
			fx_t[i].Index1 = *idx++;
			fx_t[i].Index2 = *idx++;
		}
		n_mesh->UnlockIndexBuffer();

		n_mesh->LockAttributeBuffer(0, &attri);
		for (int i = 0; i < faceCount; ++i)
		{
			fx_t[i].MaterialId = attri[i];
		}
		n_mesh->UnlockAttributeBuffer();

		fxmesh->Unlock();

		n_mesh->Release();

		return true;
	}


	FX_UVUnwrapThread::FX_UVUnwrapThread(const Array<FX_Mesh *> & arr)
		: Thread(false)
	{
		mMeshes = arr;
	}

	FX_UVUnwrapThread::~FX_UVUnwrapThread()
	{
	}

	void FX_UVUnwrapThread::Run()
	{
		FX_Unwarp_Init();

		for (int i = 0; i < mMeshes.Size(); ++i)
		{
			E_Progress((int)(i / (float)mMeshes.Size() * 100.0f));

			FX_Unwarp(mMeshes[i]);
		}

		FX_Unwarp_Shutdown();

		E_Progress(100);
	}
}