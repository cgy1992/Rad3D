#include "App.h"

class Demo11_NavPath : public App
{
	NavData * mData;
	NavPathFinderPtr mPathFinder;

	int mTestIndex;
	Float3 mTestPoint1;
	Float3 mTestPoint2;

	MGUI::Layout * mLayout;

public:
	Demo11_NavPath() {}
	virtual ~Demo11_NavPath() {}

	virtual void OnSetupResource()
	{
#ifdef M_PLATFORM_WIN32
		ResourceManager::Instance()->AddArchive(new FilePathArchive("../Core"));
		ResourceManager::Instance()->AddArchive(new FilePathArchive("../Sample"));
#endif

#ifdef M_PLATFORM_ANDROID
		ResourceManager::Instance()->AddArchive(new APKArchive("assets/Core"));
		ResourceManager::Instance()->AddArchive(new APKArchive("assets/Sample"));
#endif
	}

	virtual void OnInit()
	{
		MGUI::FontManager::Instance()->Load("Sample.font");

		World::Instance()->Load("Demo11/navtest.scene");

		World::Instance()->MainCamera()->SetPosition(4139, 2681, 1096);
		World::Instance()->MainCamera()->SetRotationEx(Float3(52, -44, 0));

		mData = World::Instance()->GetNavData();

		mPathFinder = new NavPathFinder(mData);

		mLayout = new MGUI::Layout(NULL);
		mLayout->SetAlign(MGUI::eAlign::STRETCH);
		mLayout->E_DoubleClick += new cListener1<Demo11_NavPath, const MGUI::ClickEvent *>(this, &Demo11_NavPath::OnDoubleClick);

		World::Instance()->E_RenderSolidEnd += new cListener0<Demo11_NavPath>(this, &Demo11_NavPath::OnDebugRender);

		mTestIndex = 0;
	}

	virtual void OnUpdate()
	{
	}

	virtual void OnShutdown()
	{
		mPathFinder = NULL;
		mData = NULL;

		World::Instance()->Unload();
	}

	virtual void OnPause()
	{
	}

	virtual void OnResume()
	{
	}

	//
	void SetTestPoint(const Float3 & point)
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

	//
	void OnDoubleClick(const MGUI::ClickEvent * e)
	{
		float x = e->x, y = e->y;

		App::Instance()->MapScreenUnit(x, y);

		if (x < 0 || y < 0 || x > 1 || y > 1)
			return ;

		float dist = 0;
		Ray ray = World::Instance()->MainCamera()->GetViewportRay(x, y);
		Float3 point;

		if (Terrain::Instance()->RayCheck(&point, ray))
		{
			SetTestPoint(point);
		}
		else if (ray.Intersect(&dist, Plane(0, 1, 0, 0)))
		{
			point = ray.orig + ray.dir * dist;
			SetTestPoint(point);
		}
	}

	//
	void OnDebugRender()
	{
		Mat4 form;
		form.MakeTranslate(0, 0.15f * METER_LEN, 0);

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
			RenderHelper::Instance()->DebugDraw(&triPoints[0], ePrimType::TRIANGLE_LIST, triPoints.Size() / 3, Float4(0, 1, 0), form);
		}

		// Path Test
		RenderSystem::Instance()->SetRenderState(eFillMode::SOLID, eCullMode::NONE);

		if (mTestIndex == 1)
		{
			Float3 p = mTestPoint1;

			Sphere sph;
			sph.center = p;
			sph.radius = 0.15f * METER_LEN;

			Float4 color = Float4(1, 1, 0); 
			RenderHelper::Instance()->DebugDrawSphere(sph, color, form);
		}

		const Array<NavPathPoint> & path = mPathFinder->GetPath();

		for (int i = 0; i < path.Size(); ++i)
		{
			Float3 p = path[i].position;

			Sphere sph;
			sph.center = p;
			sph.radius = 0.15f * METER_LEN;

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
			RenderHelper::Instance()->DebugDraw(&linePoints[0], ePrimType::LINE_STRIP, linePoints.Size() - 1, Float4(1, 1, 0), form);
		}
	}
};

void CreateApplication(App ** ppApp)
{
	*ppApp = new Demo11_NavPath;
}
