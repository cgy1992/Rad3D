#include "Gizmo.h"
#include "BaseOperator.h"
#include "Editor.h"

ImplementSingleton(Gizmo);

Gizmo::Gizmo()
	: mPicked(false)
	, mPickedAxis(-1)
{
	_initGeo_Move();
	_initGeo_Move_Render();

	_initGeo_Rotate();
	_initGeo_Rotate_Render();

	_initGeo_Scale();
	_initGeo_Scale_Render();

	mTech = ShaderFXManager::Instance()->Load("Gizmo", "Shader/Gizmo.mfx");
	d_assert (mTech != NULL);

	World::Instance()->E_RenderAlpha += new cListener0<Gizmo>(this, &Gizmo::OnRender);

	mOperator = OP_MOVE;
}

Gizmo::~Gizmo()
{
	delete[] mVertex_Move;
	delete[] mIndex_Move;
	delete mRender_Move;

	delete[] mVertex_Rotate;
	delete[] mIndex_Rotate;
	delete mRender_Rotate;

	delete[] mVertex_Scale;
	delete[] mIndex_Scale;
	delete mRender_Scale;

	mTech = NULL;
}

void Gizmo::OnRender()
{
	int op = GetOperator();

	if (op == OP_MOVE)
		_renderMove();
	//else if (op == OP_ROTATE)
	//_renderRotate();
	else if (op == OP_SCALE)
		_renderScale();
}

void Gizmo::Update(float _x, float _y)
{
	mMouseX = _x;
	mMouseY = _y;

	Camera * cam = World::Instance()->MainCamera();

	if (IMouse::Instance()->KeyUp(InputCode::MKC_LEFT))
	{
		if (mPicked && mPickedAxis != -1)
		{
			Node * obj = Editor::Instance()->GetSelectNode();
			d_assert (obj);

			Float3 pos = obj->GetPosition();
			Quat ort = obj->GetRotation();
			Float3 scl = obj->GetScale();

			if (mOperator == OP_MOVE)
			{
				if (pos != mOldPosition)
				{
					UndoRedo_Move * op = new UndoRedo_Move(obj->GetUID(), mOldPosition, pos);
					UndoRedoManager::Instance()->Push(op);
				}
			}
			/*else if (op == OP_ROTATE)
			{
				if (ort != mOldRotation)
				{
					UndoRedo_Rotate * op = new UndoRedo_Rotate(obj->GetUID(), mOldOrientation, ort);
					UndoRedoManager::Instance()->Push(op);
				}
			}*/
			else if (mOperator == OP_SCALE)
			{
				if (scl != mOldScale)
				{
					UndoRedo_Scale * op = new UndoRedo_Scale(obj->GetUID(), mOldScale, scl);
					UndoRedoManager::Instance()->Push(op);
				}
			}
		}

		mPicked = false;
	}

	if (mOperator == OP_MOVE)
	{
		_mouseMoved_Move();
		_update_Move();
	}
	/*else if (mOperator == OP_ROTATE)
	{
		_mouseMoved_Rotate();
		_update_Rotate();
	}*/
	else if (mOperator == OP_SCALE)
	{
		_mouseMoved_Scale();
		_update_Scale();
	}
}


void Gizmo::_initGeo_Move()
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

void Gizmo::_initGeo_Move_Render()
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

void Gizmo::_initGeo_Rotate()
{
	int segments = 20;

	mNumVertex_Rotate = (segments + 1) * 4;
	mNumIndex_Rotate = segments * 8 * 3;
	
	mVertex_Rotate = new Float3[mNumVertex_Rotate];
	mIndex_Rotate = new short[mNumIndex_Rotate];

	memset(mIndex_Rotate, 0, mNumIndex_Rotate * 2);

	const float w = 0.025f;
	for (int i = 0; i < segments + 1; ++i)
	{
		float rads = i / float(segments) * PI * 2;

		float cosine = Math::Cos(rads);
		float sine = Math::Sin(rads);

		Float3 dir = Float3(cosine, 0, sine);

		mVertex_Rotate[i * 4 + 0] = Float3(0, -w, 0) + dir * (1 - w);
		mVertex_Rotate[i * 4 + 1] = Float3(0, -w, 0) + dir * (1 + w);
		mVertex_Rotate[i * 4 + 2] = Float3(0, +w, 0) + dir * (1 - w);
		mVertex_Rotate[i * 4 + 3] = Float3(0, +w, 0) + dir * (1 + w);
	}

	for (int i = 0; i < segments; ++i)
	{
		int s = i * 4;
		int n = s + 4;
		int idx = i * 24;

		// bottom
		mIndex_Rotate[idx++] = s; 
		mIndex_Rotate[idx++] = s + 1; 
		mIndex_Rotate[idx++] = n;

		mIndex_Rotate[idx++] = n;
		mIndex_Rotate[idx++] = s + 1;
		mIndex_Rotate[idx++] = n + 1;

		// top
		mIndex_Rotate[idx++] = s + 2;
		mIndex_Rotate[idx++] = s + 3;
		mIndex_Rotate[idx++] = n + 2;

		mIndex_Rotate[idx++] = n + 2;
		mIndex_Rotate[idx++] = s + 3;
		mIndex_Rotate[idx++] = n + 3;

		// left
		mIndex_Rotate[idx++] = s;
		mIndex_Rotate[idx++] = n;
		mIndex_Rotate[idx++] = s + 2;

		mIndex_Rotate[idx++] = s + 2;
		mIndex_Rotate[idx++] = n;
		mIndex_Rotate[idx++] = n + 2;

		// right
		mIndex_Rotate[idx++] = s + 1;
		mIndex_Rotate[idx++] = n + 1;
		mIndex_Rotate[idx++] = s + 3;

		mIndex_Rotate[idx++] = s + 3;
		mIndex_Rotate[idx++] = n + 1;
		mIndex_Rotate[idx++] = n + 3;
	}
}

void Gizmo::_initGeo_Rotate_Render()
{
	mRender_Rotate = new RenderOp();

	int iVertexCount = mNumVertex_Rotate;
	int iIndexCount = mNumIndex_Rotate;
	int iPrimCount = iIndexCount / 3;

	mRender_Rotate->vertexDeclarations[0].AddElement(eVertexSemantic::POSITION, eVertexType::FLOAT3);

	VertexBufferPtr buffer = HWBufferManager::Instance()->NewVertexBuffer(sizeof (Float3), iVertexCount);

	float * verteces;
	verteces = (float *)buffer->Lock(eLockFlag::WRITE);
	{
		memcpy(verteces, mVertex_Rotate, iVertexCount * sizeof (Float3));
	}
	buffer->Unlock();

	mRender_Rotate->vertexBuffers[0] = buffer;

	IndexBufferPtr ibuffer = HWBufferManager::Instance()->NewIndexBuffer(iIndexCount);
	short * indices;
	indices = (short *)ibuffer->Lock(eLockFlag::WRITE);
	{
		memcpy(indices, mIndex_Rotate, iIndexCount * sizeof (short));
	}
	ibuffer->Unlock();

	mRender_Rotate->indexBuffer = ibuffer;

	mRender_Rotate->primCount = iPrimCount;
	mRender_Rotate->primType = ePrimType::TRIANGLE_LIST;
}

void Gizmo::_initGeo_Scale()
{
	mNumVertex_Scale = 8;
	mNumIndex_Scale = 36;

	mVertex_Scale = new Float3[mNumVertex_Scale];
	mIndex_Scale = new short[mNumIndex_Scale];

	float * vert = (float *)mVertex_Scale;
	{
		const float w = 0.02f;
		const float h = 0.02f;
		const float d = 0.02f;
		Float3 pos;

		//front
		pos = Float3(1 - w, h, -d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		pos = Float3(1 + w, h, -d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		pos = Float3(1 - w, -h, -d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		pos = Float3(1 + w, -h, -d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		//back
		pos = Float3(1 - w, h, d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		pos = Float3(1 + w, h, d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		pos = Float3(1 - w, -h, d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		pos = Float3(1 + w, -h, d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;
	}

	short * indices = (short *)mIndex_Scale;
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
	}
}

void Gizmo::_initGeo_Scale_Render()
{
	mRender_Scale = new RenderOp();

	int iVertexCount = mNumVertex_Scale;
	int iIndexCount = mNumIndex_Scale;
	int iPrimCount = iIndexCount / 3;

	mRender_Scale->vertexDeclarations[0].AddElement(eVertexSemantic::POSITION, eVertexType::FLOAT3);

	VertexBufferPtr buffer = HWBufferManager::Instance()->NewVertexBuffer(sizeof (Float3), iVertexCount);

	float * verteces;
	verteces = (float *)buffer->Lock(eLockFlag::WRITE);
	{
		memcpy(verteces, mVertex_Scale, iVertexCount * sizeof(Float3));
	}
	buffer->Unlock();

	mRender_Scale->vertexBuffers[0] = buffer;

	IndexBufferPtr ibuffer = HWBufferManager::Instance()->NewIndexBuffer(iIndexCount);
	short * indices;
	indices = (short *)ibuffer->Lock(eLockFlag::WRITE);
	{
		memcpy(indices, mIndex_Scale, iIndexCount * sizeof (short));
	}
	ibuffer->Unlock();

	mRender_Scale->indexBuffer = ibuffer;

	mRender_Scale->primCount = iPrimCount;
	mRender_Scale->primType = ePrimType::TRIANGLE_LIST;
}

void Gizmo::_renderMove()
{
	if (Editor::Instance()->GetSelectNode() == NULL)
		return ;

	Node * obj = Editor::Instance()->GetSelectNode();
	float w = _getObjSize(obj);

	Float3 position = obj->GetPosition();
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

void Gizmo::_renderRotate()
{
	if (Editor::Instance()->GetSelectNode() == 0)
		return ;

	Node * obj = Editor::Instance()->GetSelectNode();
	float w = _getObjSize(obj);

	Float3 position = obj->GetPosition();
	Quat orientation = Quat::Identity;
	Float3 scale = Float3(w, w, w);

	RenderSystem * render = RenderSystem::Instance();

	Mat4 matLocal, matWorld;
	Float4 colorData;

	// xAxis
	{
		matLocal.MakeRotationZ(PI / 2);
		matWorld.MakeTransform(position, orientation, scale);
		matWorld = matLocal * matWorld;

		if (mPickedAxis == 0)
			colorData = Float4(1, 1, 1, 1);
		else
			colorData = Float4(1, 0, 0, 1);

		mTech->GetPass(0)->SetUniform("u_Color", colorData);

		render->SetWorldTM(matWorld);
		render->Render(mTech, mRender_Rotate);
	}

	// yAxis
	{
		matLocal = Mat4::Identity;
		matWorld.MakeTransform(position, orientation, scale);
		matWorld = matLocal * matWorld;

		if (mPickedAxis == 1)
			colorData = Float4(1, 1, 1, 1);
		else
			colorData = Float4(0, 1, 0, 1);

		mTech->GetPass(0)->SetUniform("u_Color", colorData);

		render->SetWorldTM(matWorld);
		render->Render(mTech, mRender_Rotate);
	}

	// zAxis
	{
		matLocal.MakeRotationX(PI / 2);
		matWorld.MakeTransform(position, orientation, scale);
		matWorld = matLocal * matWorld;

		if (mPickedAxis == 2)
			colorData = Float4(1, 1, 1, 1);
		else
			colorData = Float4(0, 0, 1, 1);

		mTech->GetPass(0)->SetUniform("u_Color", colorData);

		render->SetWorldTM(matWorld);
		render->Render(mTech, mRender_Rotate);
	}
}

void Gizmo::_renderScale()
{
	if (Editor::Instance()->GetSelectNode() == 0)
		return ;

	Node * obj = Editor::Instance()->GetSelectNode();
	float w = _getObjSize(obj);

	Float3 position = obj->GetPosition();
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
		render->Render(mTech, mRender_Scale);
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
		render->Render(mTech, mRender_Scale);
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
		render->Render(mTech, mRender_Scale);
	}
}

void Gizmo::_update_Move()
{
	Camera * cam = World::Instance()->MainCamera();

	if (mPicked)
		return ;

	Float2 pt = Float2(mMouseX, mMouseY);

	Ray ray = cam->GetViewportRay(pt.x, pt.y);

	if (Editor::Instance()->GetSelectNode() == 0)
		return ;

	Node * obj = Editor::Instance()->GetSelectNode();
	float w = _getObjSize(obj);

	Float3 position = obj->GetPosition();
	Quat rotation = obj->GetRotation();
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
			bound.Merge(mVertex_Move[i] * matWorld);
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
			bound.Merge(mVertex_Move[i] * matWorld);
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
			bound.Merge(mVertex_Move[i] * matWorld);
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
		mOldPosition = obj->GetPosition();
	}
}

void Gizmo::_mouseMoved_Move()
{
	Camera * cam = World::Instance()->MainCamera();

	Node * obj = Editor::Instance()->GetSelectNode();

	if (obj == 0)
		return ;

	float w = _getObjSize(obj);

	if (mPickedAxis != -1 && mPicked && IMouse::Instance()->MouseMoved())
	{
		Float2 pt = IMouse::Instance()->GetPositionDiff();
		Editor::Instance()->MapScreenUnit(pt.x, pt.y);

		const Mat4 & matVP = cam->GetViewProjMatrix();

		float dt = 0;

		Float3 p0 = obj->GetPosition();
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

		if (len > 0.000001f)
			d.x /= len , d.y /= len;
		else
			d.x = 0 , d.y = 0;

		dt = d.x * pt.x + d.y * pt.y;

		Float3 target = obj->GetPosition() + p1 * dt * w * 5;

		obj->SetPosition(target);

		Editor::Instance()->E_NodePositionChanged();

	}
}

void Gizmo::_update_Rotate()
{
	Camera * cam = World::Instance()->MainCamera();

	if (mPicked)
		return ;

	Float2 pt = Float2(mMouseX, mMouseY);

	Ray ray = cam->GetViewportRay(pt.x, pt.y);

	if (Editor::Instance()->GetSelectNode() == 0)
		return ;

	Node * obj = Editor::Instance()->GetSelectNode();

	float w = _getObjSize(obj);

	Float3 position = obj->GetPosition();
	Quat orientation = Quat::Identity;
	Float3 scale = Float3(w, w, w);

	float dx, dy, dz, dist;

	// x axis
	{
		Mat4 matLocal, matWorld;

		matLocal.MakeRotationZ(PI / 2);
		matWorld.MakeTransform(position, Quat::Identity, scale);
		matWorld = matLocal * matWorld;

		if (ray.Intersect(&dist, mVertex_Rotate, mIndex_Rotate, mNumIndex_Rotate / 3, matWorld))
			dx = dist;
		else
			dx = -1;
	}

	// y axis
	{
		Mat4 matLocal, matWorld;

		matLocal.MakeRotationY(-PI / 2);
		matWorld.MakeTransform(position, Quat::Identity, scale);
		matWorld = matLocal * matWorld;

		if (ray.Intersect(&dist, mVertex_Rotate, mIndex_Rotate, mNumIndex_Rotate / 3, matWorld))
			dy = dist;
		else
			dy = -2;
	}

	// z axis
	{
		Mat4 matLocal, matWorld;

		matLocal.MakeRotationX(PI / 2);
		matWorld.MakeTransform(position, Quat::Identity, scale);
		matWorld = matLocal * matWorld;

		if (ray.Intersect(&dist, mVertex_Rotate, mIndex_Rotate, mNumIndex_Rotate / 3, matWorld))
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
		mOldRotation = obj->GetRotation();
	}
}

void Gizmo::_mouseMoved_Rotate()
{
	Camera * cam = World::Instance()->MainCamera();

	if (Editor::Instance()->GetSelectNode() == 0)
		return ;

	Node * obj = Editor::Instance()->GetSelectNode();
	Float3 pos = obj->GetPosition();

	float w = _getObjSize(obj);

	if (mPickedAxis != -1 && mPicked && IMouse::Instance()->MouseMoved())
	{
		Float2 pt = IMouse::Instance()->GetPositionDiff();
		Editor::Instance()->MapScreenUnit(pt.x, pt.y);

		const Mat4 & matVP = cam->GetViewProjMatrix();

		float dt = 0;

		Float3 p0 = pos;
		Float3 p1, axis;

		if (mPickedAxis == 0)
		{
			p1 = Float3(0, 0, 1);
			axis = Float3(1, 0, 0);
		}
		else if (mPickedAxis == 1)
		{
			p1 = Float3(-1, 0, 0);
			axis = Float3(0, 1, 0);
		}
		else
		{
			p1 = Float3(0, -1, 0);
			axis = Float3(0, 0, 1);
		}

		Float3 p2 = p0 * matVP;
		Float3 p3 = (p0 + p1) * matVP;

		p2.x = (p2.x + 1) / 2;
		p2.y = (1 - p2.y) / 2;

		p3.x = (p3.x + 1) / 2;
		p3.y = (1 - p3.y) / 2;

		Float2 d = Float2(p3.x - p2.x, p3.y - p2.y);

		d.Normalize();

		dt = d.x * pt.x + d.y * pt.y;

		Float3 ort = obj->GetRotation().ToEulerAngle();

		ort += axis * Math::RadianToDegree(dt * PI);

		obj->SetRotationEx(ort);

		Editor::Instance()->E_NodeRotationChanged();
	}
}

void Gizmo::_update_Scale()
{
	Camera * cam = World::Instance()->MainCamera();

	if (mPicked)
		return ;

	Float2 pt = Float2(mMouseX, mMouseY);

	Ray ray = cam->GetViewportRay(pt.x, pt.y);

	if (Editor::Instance()->GetSelectNode() == 0)
		return ;

	Node * obj = Editor::Instance()->GetSelectNode();

	float w = _getObjSize(obj);

	Float3 position = obj->GetPosition();
	Float3 scale = Float3(w, w, w);

	float dx, dy, dz, dist;

	// x axis
	{
		Aabb bound = Aabb::Invalid;

		Mat4 matLocal, matWorld;

		matLocal = Mat4::Identity;
		matWorld.MakeTransform(position, Quat::Identity, scale);
		matWorld = matLocal * matWorld;

		for (int i = 0; i < mNumVertex_Scale; ++i)
		{
			bound.Merge(mVertex_Scale[i] * matWorld);
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

		for (int i = 0; i < mNumVertex_Scale; ++i)
		{
			bound.Merge(mVertex_Scale[i] * matWorld);
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

		for (int i = 0; i < mNumVertex_Scale; ++i)
		{
			bound.Merge(mVertex_Scale[i] * matWorld);
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
		mOldScale = obj->GetScale();
	}
}

void Gizmo::_mouseMoved_Scale()
{
	Camera * cam = World::Instance()->MainCamera();

	if (Editor::Instance()->GetSelectNode() == 0)
		return ;

	Node * obj = Editor::Instance()->GetSelectNode();

	if (mPickedAxis != -1 && mPicked && IMouse::Instance()->MouseMoved())
	{
		Float2 pt = IMouse::Instance()->GetPositionDiff();
		Editor::Instance()->MapScreenUnit(pt.x, pt.y);

		const Mat4 & matVP = cam->GetViewProjMatrix();

		float dt = 0;

		Float3 p0 = obj->GetPosition();
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
		d.Normalize();

		dt = d.x * pt.x + d.y * pt.y;

		Float3 scale = obj->GetScale();
		scale *= Float3(1, 1, 1) + dt * p1;
		obj->SetScale(scale);

		Editor::Instance()->E_NodeScaleChanged();
	}
}

float Gizmo::_getObjSize(Node * obj)
{
	Aabb box = obj->GetBound();
	Float3 size = box.GetSize();
	float w = size.x;

	w = Max(w, size.y);
	w = Max(w, size.z);
	w *= 0.5f;
	
	Camera * cam = World::Instance()->MainCamera();
	Float3 pos = obj->GetPosition();

	pos *= cam->GetViewProjMatrix();

	pos = Float3(0.3f, 0, pos.z);

	Mat4 matProj = cam->GetProjMatrix();
	matProj.Inverse();
	pos *= matProj;

	return pos.x;
}
