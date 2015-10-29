#include "NavGenerator.h"
#include "CameraController.h"
#include "MessageBoxDialog.h"
#include "Editor.h"

ImplementSingleton(NavGenerator);

NavGenerator::NavGenerator()
{
	mSelectIndex = -1;
	mEditMode = TRUE;
	mTestIndex = 0;

	mData = World::Instance()->GetNavData();
	mPathFinder = new NavPathFinder(mData);

	Editor::Instance()->E_SceneLoaded += new cListener0<NavGenerator>(this, &NavGenerator::OnSceneLoaded);
	Editor::Instance()->E_SceneUnload += new cListener0<NavGenerator>(this, &NavGenerator::OnSceneUnload);
}

NavGenerator::~NavGenerator()
{
}

void NavGenerator::AddPoint(const Float3 & point, bool no_edge)
{
	int pointId = mData->AddPoint(point);

	if (mSelectIndex != -1 && !no_edge)
	{
		mData->AddEdge(mSelectIndex, pointId);
	}

	mSelectIndex = pointId;
}

void NavGenerator::Connect(int index)
{
	if (mSelectIndex != -1 && mSelectIndex != index)
	{
		mData->AddEdge(mSelectIndex, index);
	}
}

void NavGenerator::DeleteCurrentPoint()
{
	if (mSelectIndex != -1)
	{
		mData->RemovePoint(mSelectIndex);

		if (mSelectIndex >= mData->Points.Size())
		{
			mSelectIndex = mData->Points.Size() - 1;
		}
	}

	mPathFinder->Clear();
}

int NavGenerator::PickPoint(const Ray & ray)
{
	float dist;
	for (int i = 0; i < mData->Points.Size(); ++i)
	{
		const Float3 & p = mData->Points[i];

		Sphere sph;
		sph.center = p + Float3(0, 0.15f * UNIT_METRES, 0);
		sph.radius = 0.15f * UNIT_METRES;

		if (ray.Intersect(&dist, sph))
		{
			return i;
		}
	}

	return -1;
}

void NavGenerator::SelectPoint(int index)
{
	mSelectIndex = index;
}

void NavGenerator::DeleteEdge(const Ray & ray)
{
	for (int i = 0; i < mData->Edges.Size(); ++i)
	{
		const Float3 & pa = mData->Points[mData->Edges[i].pointId[0]];
		const Float3 & pb = mData->Points[mData->Edges[i].pointId[1]];

		Float3 dir = pb - pa;
		float length = dir.Normalize();
		if (length < EPSILON_E4)
			continue;

		Obb obb;
		obb.center = (pa + pb) / 2;
		obb.center.y += 0.15f * UNIT_METRES;
		
		obb.axis[0] = dir;
		obb.axis[1] = Float3(0, 1, 0);
		obb.axis[2] = Float3::Cross(obb.axis[0], obb.axis[1]);
		
		obb.extent = Float3(length * 0.5f, 0.15f * UNIT_METRES, 0.15f * UNIT_METRES);

		float dist = 0;
		if (ray.Intersect(&dist, obb))
		{
			mData->RemoveEdge(i);
			mPathFinder->Clear();
			return ;
		}
	}
}

void NavGenerator::LookAtError()
{
	for (int i = 0; i < mData->Edges.Size(); ++i)
	{
		const NavEdge & e = mData->Edges[i];
		if ((e.flag & 0x01) == 1)
		{
			Float3 center = (mData->Points[e.pointId[0]] + mData->Points[e.pointId[1]]) / 2;

			CameraController cc;
			cc.SetLookAt(center);
			cc.ForceUpdate();

			break;
		}
	}
}

void NavGenerator::OnDebugRender()
{
	Mat4 form;
	form.MakeTranslate(0, 0.15f * UNIT_METRES, 0);

	if (mEditMode)
	{
		RenderSystem::Instance()->SetRenderState(eFillMode::SOLID, eCullMode::NONE);

		for (int i = 0; i < mData->Points.Size(); ++i)
		{
			Float3 p = mData->Points[i];

			Sphere sph;
			sph.center = p;
			sph.radius = 0.15f * UNIT_METRES;

			if (World::Instance()->MainCamera()->GetVisibility(sph) == Camera::VB_NONE)
				continue ;

			Float4 color = mSelectIndex != i ? Float4(0, 1, 0) : Float4(1, 1, 0); 
			RenderHelper::Instance()->DebugDrawSphere(sph, color, form);
		}

		// normals
		Array<Float3> linePoints;
		for (int i = 0; i < mData->Edges.Size(); ++i)
		{
			const NavEdge & e = mData->Edges[i];

			if (e.flag == 0)
			{
				linePoints.PushBack(mData->Points[mData->Edges[i].pointId[0]]);
				linePoints.PushBack(mData->Points[mData->Edges[i].pointId[1]]);
			}
		}

		if (linePoints.Size() > 1)
		{
			RenderHelper::Instance()->DebugDrawLines(&linePoints[0], linePoints.Size() / 2, Float4(0, 1, 0), form, false);
		}

		// errors
		linePoints.Clear();
		for (int i = 0; i < mData->Edges.Size(); ++i)
		{
			const NavEdge & e = mData->Edges[i];

			if (e.flag == NAV_EAGE_FLAG_ERROR)
			{
				linePoints.PushBack(mData->Points[mData->Edges[i].pointId[0]]);
				linePoints.PushBack(mData->Points[mData->Edges[i].pointId[1]]);
			}
		}

		if (linePoints.Size() > 1)
		{
			RenderHelper::Instance()->DebugDrawLines(&linePoints[0], linePoints.Size() / 2, Float4(1, 0, 0), form, false);
		}

		// warnings
		linePoints.Clear();
		for (int i = 0; i < mData->Edges.Size(); ++i)
		{
			const NavEdge & e = mData->Edges[i];

			if (e.flag == NAV_EAGE_FLAG_BOUND)
			{
				linePoints.PushBack(mData->Points[mData->Edges[i].pointId[0]]);
				linePoints.PushBack(mData->Points[mData->Edges[i].pointId[1]]);
			}
		}

		if (linePoints.Size() > 1)
		{
			RenderHelper::Instance()->DebugDrawLines(&linePoints[0], linePoints.Size() / 2, Float4(1, 1, 0), form, false);
		}
	}
	else
	{
		// Nav Triangle
		RenderSystem::Instance()->SetRenderState(eFillMode::FRAME, eCullMode::NONE);

		Array<Float3> triPoints;
		for (int i = 0; i < mData->Triangles.Size(); ++i)
		{
			triPoints.PushBack(mData->Points[mData->Triangles[i].pointId[0]]);
			triPoints.PushBack(mData->Points[mData->Triangles[i].pointId[1]]);
			triPoints.PushBack(mData->Points[mData->Triangles[i].pointId[2]]);
		}

		if (triPoints.Size() > 0)
		{
			RenderHelper::Instance()->DebugDrawTriangles(&triPoints[0], triPoints.Size() / 3, Float4(0, 1, 0), form);
		}


		// Path Test
		RenderSystem::Instance()->SetRenderState(eFillMode::SOLID, eCullMode::NONE);

		if (mTestIndex == 1)
		{
			Float3 p = mTestPoint1;

			Sphere sph;
			sph.center = p;
			sph.radius = 0.15f * UNIT_METRES;

			RenderHelper::Instance()->DebugDrawSphere(sph, Float4(1, 1, 0), form);
		}

		const Array<NavPathPoint> & path = mPathFinder->GetPath();

		for (int i = 0; i < path.Size(); ++i)
		{
			Float3 p = path[i].position;

			Sphere sph;
			sph.center = p;
			sph.radius = 0.15f * UNIT_METRES;

			Float4 color = (path[i].flag & NAV_PATH_FLAG_SMOOTH) ? Float4(0, 1, 0) : Float4(1, 1, 0); 
			RenderHelper::Instance()->DebugDrawSphere(sph, color, form);
		}

		Array<Float3> linePoints;
		for (int i = 0; i < path.Size(); ++i)
		{
			if (path[i].flag & NAV_PATH_FLAG_SMOOTH)
			{
				linePoints.PushBack(path[i].position);
			}
		}

		if (linePoints.Size() > 1)
		{
			RenderSystem::Instance()->SetRenderState(eFillMode::SOLID, eCullMode::NONE);
			RenderHelper::Instance()->DebugDrawLines(&linePoints[0], linePoints.Size() - 1, Float4(1, 1, 0), form, true);
		}
	}
}

bool NavGenerator::GetSelectPoint(Float3 & point)
{
	if (mEditMode && mSelectIndex != -1)
	{
		point = mData->Points[mSelectIndex];
		return true;
	}
	else
	{
		return false;
	}
}

void NavGenerator::SetSelectPoint(const Float3 & point)
{
	if (mEditMode && mSelectIndex != -1)
	{
		mData->Points[mSelectIndex] = point;
	}
}

void NavGenerator::SetEditable(int em)
{
	mEditMode = em;
}

void NavGenerator::Build()
{
	if (mData->Build())
	{
		DoMessageBox("Build Error!", MessageBoxType::Error);
	}
	else
	{
		String msg;
		msg.Format("Point: %d, Edge: %d, Triangle: %d", mData->Points.Size(), mData->Edges.Size(), mData->Triangles.Size());
		DoMessageBox(msg, MessageBoxType::Info);
	}
}

void NavGenerator::SetTestPoint(const Float3 & point)
{
	if (mTestIndex == 2)
	{
		mTestIndex = 0;
	}

	if (mTestIndex == 0)
	{
		mPathFinder->Clear();
		mTestPoint1 = point;
	}
	else if (mTestIndex == 1)
	{
		mTestPoint2 = point;

		mPathFinder->Do(mTestPoint1, mTestPoint2, 4096);
	}

	mTestIndex += 1;
}

void NavGenerator::OnSceneLoaded()
{
	mTestIndex = 0;
	mSelectIndex = -1;
}

void NavGenerator::OnSceneUnload()
{
	mSelectIndex = -1;
	mTestIndex = 0;
}
