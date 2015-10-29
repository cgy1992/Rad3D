#include "NavGizmo.h"
#include "NavGenerator.h"
#include "NavPathModule.h"
#include "Editor.h"

NavGizmo::NavGizmo()
	: mPicked(false)
	, mPickedAxis(-1)
	, mEnable(true)
{
	_initGeo_Move();
	_initGeo_Move_Render();

	mTech = ShaderFXManager::Instance()->Load("Gizmo", "Gizmo.mfx");
	d_assert (mTech != NULL);

	World::Instance()->E_RenderAlpha += new cListener0<NavGizmo>(this, &NavGizmo::Render);
}

NavGizmo::~NavGizmo()
{
	delete[] mVertex_Move;
	delete[] mIndex_Move;
	delete mRender_Move;

	mTech = NULL;
}

void NavGizmo::Render()
{
	if (!mEnable && MainFrame::Instance()->GetCurrentModule() != NavPathModule::Instance())
		return ;

	Float3 point;
	if (!NavGenerator::Instance()->GetSelectPoint(point))
		return ;

	_renderMove();
}

void NavGizmo::Update(float _x, float _y)
{
	if (!mEnable && MainFrame::Instance()->GetCurrentModule() != NavPathModule::Instance())
		return ;

	Float3 point;
	if (!NavGenerator::Instance()->GetSelectPoint(point))
		return ;

	mMouseX = _x;
	mMouseY = _y;

	Camera * cam = World::Instance()->MainCamera();

	if (IMouse::Instance()->KeyUp(InputCode::MKC_LEFT))
	{
		if (mPicked && mPickedAxis != -1)
		{
		}

		mPicked = false;
	}

	_mouseMoved_Move();
	_update_Move();
}


void NavGizmo::_initGeo_Move()
{
	mNumVertex_Move = 8 + 5;
	mNumIndex_Move = 12 * 3 + 4 * 3;

	mVertex_Move = new Float3[mNumVertex_Move];
	mIndex_Move = new short[mNumIndex_Move];

	float * vert = (float *)mVertex_Move;
	{
		const float off = 0.1f;
		const float half_w = 1 + off;
		const float half_h = 0.02f;
		const float half_d = 0.02f;
		Float3 pos;

		//front
		pos = Float3(off, half_h, -half_d);
		*vert++ = off;
		*vert++ = pos.y;
		*vert++ = pos.z;

		pos = Float3(half_w, half_h, -half_d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		pos = Float3(off, -half_h, -half_d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		pos = Float3(half_w, -half_h, -half_d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		//back
		pos = Float3(off, half_h, half_d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		pos = Float3(half_w, half_h, half_d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		pos = Float3(off, -half_h, half_d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		pos = Float3(half_w, -half_h, half_d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		//
		pos = Float3(half_w + 0.3f, 0, 0);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		pos = Float3(half_w - 0.1f,  half_h + 0.01f,  half_d + 0.01f);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		pos = Float3(half_w - 0.1f, -half_h - 0.01f,  half_d + 0.01f);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		pos = Float3(half_w - 0.1f, -half_h - 0.01f, -half_d - 0.01f);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		pos = Float3(half_w - 0.1f,  half_h + 0.01f, -half_d - 0.01f);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;
	}

	short * indices = (short *)mIndex_Move;
	{
		//front
		*indices++ = 0, *indices++ = 1, *indices++ = 2;
		*indices++ = 2, *indices++ = 1, *indices++ = 3;

		//back
		*indices++ = 5, *indices++ = 4, *indices++ = 7;
		*indices++ = 7, *indices++ = 4, *indices++ = 6;

		//left
		*indices++ = 4, *indices++ = 0, *indices++ = 6;
		*indices++ = 6, *indices++ = 0, *indices++ = 2;

		//right
		*indices++ = 1, *indices++ = 5, *indices++ = 3;
		*indices++ = 3, *indices++ = 5, *indices++ = 7;

		//top
		*indices++ = 4, *indices++ = 5, *indices++ = 0;
		*indices++ = 0, *indices++ = 5, *indices++ = 1;

		//bottom
		*indices++ = 2, *indices++ = 3, *indices++ = 6;
		*indices++ = 6, *indices++ = 3, *indices++ = 7;

		//
		*indices++ = 8, *indices++ =  9, *indices++ = 10;
		*indices++ = 8, *indices++ = 10, *indices++ = 11;
		*indices++ = 8, *indices++ = 11, *indices++ = 12;
		*indices++ = 8, *indices++ = 12, *indices++ = 9;
	}
}

void NavGizmo::_initGeo_Move_Render()
{
	mRender_Move = new RenderOp();

	int iVertexCount = mNumVertex_Move;
	int iIndexCount = mNumIndex_Move;
	int iPrimCount = iIndexCount / 3;

	mRender_Move->vertexDeclarations[0].AddElement(eVertexSemantic::POSITION, eVertexType::FLOAT3);

	VertexBufferPtr buffer = HWBufferManager::Instance()->NewVertexBuffer(sizeof(Float3), iVertexCount);

	float * verteces = (float *)buffer->Lock(eLockFlag::WRITE);
	{
		memcpy(verteces, mVertex_Move, iVertexCount * sizeof(Float3));
	}
	buffer->Unlock();

	mRender_Move->vertexBuffers[0] = buffer;

	IndexBufferPtr ibuffer = HWBufferManager::Instance()->NewIndexBuffer(iIndexCount);
	short * indices = (short *)ibuffer->Lock(eLockFlag::WRITE);
	{
		memcpy(indices, mIndex_Move, iIndexCount * sizeof (short));
	}
	ibuffer->Unlock();

	mRender_Move->indexBuffer = ibuffer;

	mRender_Move->primCount = iPrimCount;
	mRender_Move->primType = ePrimType::TRIANGLE_LIST;
}

void NavGizmo::_renderMove()
{
	Float3 point;
	if (!NavGenerator::Instance()->GetSelectPoint(point))
		return ;

	float w = _getObjSize(point);

	Float3 position = point;
	Quat orientation = Quat::Identity;
	Float3 scale = Float3(w, w, w);

	RenderSystem * render = RenderSystem::Instance();

	Mat4 matLocal, matWorld;
	Float4 colorData;

	// xAxis
	{
		matLocal = Mat4::Identity;
		matWorld.MakeTransform(position, orientation, scale);
		matWorld = matLocal * matWorld;

		if (mPickedAxis == 0)
			colorData = Float4(1, 1, 1, 1);
		else
			colorData = Float4(1, 0, 0, 1);

		mTech->GetPass(0)->SetUniform("u_Color", colorData);

		render->SetWorldTM(matWorld);
		render->Render(mTech, mRender_Move);
	}

	// yAxis
	{
		matLocal.MakeRotationZ(PI / 2);
		matWorld.MakeTransform(position, orientation, scale);
		matWorld = matLocal * matWorld;

		if (mPickedAxis == 1)
			colorData = Float4(1, 1, 1, 1);
		else
			colorData = Float4(0, 1, 0, 1);

		mTech->GetPass(0)->SetUniform("u_Color", colorData);

		render->SetWorldTM(matWorld);
		render->Render(mTech, mRender_Move);
	}

	// zAxis
	{
		matLocal.MakeRotationY(-PI / 2);
		matWorld.MakeTransform(position, orientation, scale);
		matWorld = matLocal * matWorld;

		if (mPickedAxis == 2)
			colorData = Float4(1, 1, 1, 1);
		else
			colorData = Float4(0, 0, 1, 1);

		mTech->GetPass(0)->SetUniform("u_Color", colorData);

		render->SetWorldTM(matWorld);
		render->Render(mTech, mRender_Move);
	}
}

void NavGizmo::_update_Move()
{
	Camera * cam = World::Instance()->MainCamera();

	if (mPicked)
		return ;

	Float2 pt = Float2(mMouseX, mMouseY);

	Ray ray = cam->GetViewportRay(pt.x, pt.y);

	Float3 point;
	if (!NavGenerator::Instance()->GetSelectPoint(point))
		return ;

	float w = _getObjSize(point);

	Float3 position = point;
	Float3 scale = Float3(w, w, w);

	float dx, dy, dz, dist;

	// x axis
	{
		Aabb bound = Aabb::Invalid;

		Mat4 matLocal, matWorld;

		matLocal = Mat4::Identity;
		matWorld.MakeTransform(position, Quat::Identity, scale);
		matWorld = matLocal * matWorld;

		for (int i = 0; i < mNumVertex_Move; ++i)
		{
			Float3 p = mVertex_Move[i] * matWorld;
			bound.Merge(p);
		}

		if (ray.Intersect(&dist, bound))
			dx = dist;
		else
			dx = -1;
	}

	// y axis
	{
		Aabb bound = Aabb::Invalid;

		Mat4 matLocal, matWorld;

		matLocal.MakeRotationZ(PI / 2);
		matWorld.MakeTransform(position, Quat::Identity, scale);
		matWorld = matLocal * matWorld;

		for (int i = 0; i < mNumVertex_Move; ++i)
		{
			Float3 p = mVertex_Move[i] * matWorld;
			bound.Merge(p);
		}

		if (ray.Intersect(&dist, bound))
			dy = dist;
		else
			dy = -2;
	}

	// z axis
	{
		Aabb bound = Aabb::Invalid;

		Mat4 matLocal, matWorld;

		matLocal.MakeRotationY(-PI / 2);
		matWorld.MakeTransform(position, Quat::Identity, scale);
		matWorld = matLocal * matWorld;

		for (int i = 0; i < mNumVertex_Move; ++i)
		{
			Float3 p = mVertex_Move[i] * matWorld;
			bound.Merge(p);
		}

		if (ray.Intersect(&dist, bound))
			dz = dist;
		else
			dz = -3;
	}

	mPickedAxis = -1;

	if (dx > 0)
		mPickedAxis = 0;

	if (dy > 0 && dy > dx)
		mPickedAxis = 1;

	if (dz > 0 && dz > dy && dz > dx)
		mPickedAxis = 2;

	if (mPickedAxis != -1 && IMouse::Instance()->KeyDown(InputCode::MKC_LEFT))
	{
		mPicked = true;
		mOldPosition = point;
	}
}

void NavGizmo::_mouseMoved_Move()
{
	Float3 point;
	if (!NavGenerator::Instance()->GetSelectPoint(point))
		return ;

	Camera * cam = World::Instance()->MainCamera();
	float w = _getObjSize(point);

	if (mPickedAxis != -1 && mPicked && IMouse::Instance()->MouseMoved())
	{
		Float2 pt = IMouse::Instance()->GetPositionDiff();

		Editor::Instance()->MapScreenUnit(pt.x, pt.y);
		const Mat4 & matVP = cam->GetViewProjMatrix();

		float dt = 0;

		Float3 p0 = point;
		Float3 p1;

		if (mPickedAxis == 0)
			p1 = Float3(1, 0, 0);
		else if (mPickedAxis == 1)
			p1 = Float3(0, 1, 0);
		else
			p1 = Float3(0, 0, 1);

		Float3 p2 = p0 * matVP;
		Float3 p3 = (p0 + p1) * matVP;

		p2.x = (p2.x + 1) / 2;
		p2.y = (1 - p2.y) / 2;

		p3.x = (p3.x + 1) / 2;
		p3.y = (1 - p3.y) / 2;

		Float2 d = Float2(p3.x - p2.x, p3.y - p2.y);

		float len = Math::Sqrt(d.x * d.x + d.y * d.y);

		if (len > 0.0001f)
			d.x /= len , d.y /= len;
		else
			d.x = 0 , d.y = 0;

		dt = d.x * pt.x + d.y * pt.y;

		Float3 newPos = point + p1 * dt * w * 5;

		E_PositionChanged(newPos);
	}
}

float NavGizmo::_getObjSize(const Float3 & point)
{
	Aabb box(point - Float3(15, 0, 15), point + Float3(15, 30, 15));
	Float3 size = box.GetSize();
	float w = size.x;

	w = Max(w, size.y);
	w = Max(w, size.z);
	w *= 0.1f;
	
	Camera * cam = World::Instance()->MainCamera();
	Float3 pos = point;

	pos *= cam->GetViewProjMatrix();

	pos = Float3(0.2f, 0, pos.z);

	Mat4 matProj = cam->GetProjMatrix();
	matProj.Inverse();
	pos *= matProj;

	return pos.x;
}


