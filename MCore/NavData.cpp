#include "NavData.h"
#include "MSet.h"
#include "MLog.h"
#include "MResourceManager.h"
#include "MSerializer.h"

namespace Rad {

	NavData::NavData()
		: Bound(0, 0, 0, 0)
		, GridWidth(0)
		, GridHeight(0)
	{
	}

	NavData::~NavData()
	{
	}

	bool NavData::_intersects(const RectF & rect, const NavTriangle & tri) const
	{
		const Float3 & A = Points[tri.pointId[0]];
		const Float3 & B = Points[tri.pointId[1]];
		const Float3 & C = Points[tri.pointId[2]];

		if (A.x <= rect.x1 && B.x <= rect.x1 && C.x <= rect.x1)
			return false;

		if (A.z <= rect.y1 && B.z <= rect.y1 && C.z <= rect.y1)
			return false;

		if (A.x >= rect.x2 && B.x >= rect.x2 && C.x >= rect.x2)
			return false;

		if (A.z >= rect.y2 && B.z >= rect.y2 && C.z >= rect.y2)
			return false;

		return true;
	}

	bool NavData::_hasTriangle(int pa, int pb, int pc) const
	{
		FixedSet<int, 3> t_set1, t_set2;
		t_set1.Insert(pa);
		t_set1.Insert(pb);
		t_set1.Insert(pc);

		for (int i = 0; i < Triangles.Size(); ++i)
		{
			t_set2.Clear();
			t_set2.Insert(Triangles[i].pointId[0]);
			t_set2.Insert(Triangles[i].pointId[1]);
			t_set2.Insert(Triangles[i].pointId[2]);

			if (t_set1[0] == t_set2[0] && t_set1[1] == t_set2[1] && t_set1[2] == t_set2[2])
			{
				return true;
			}
		}

		return false;
	};

	bool NavData::Load(const String & filename)
	{
		d_assert (Points.Size() == 0);

		DataStreamPtr stream = ResourceManager::Instance()->OpenResource(filename);
		if (stream == NULL)
		{
			d_log("!: NavData load failed, '%s'.", filename.c_str());
			return false;
		}

		ISerializerD IS(stream);

		int magic, version;
		IS >> magic;
		IS >> version;

		d_assert (magic == NAV_MAGIC && version == NAV_VERSION);

		IS >> Bound.x1;
		IS >> Bound.y1;
		IS >> Bound.x2;
		IS >> Bound.y2;

		int numPoints = 0;
		IS >> numPoints;
		if (numPoints > 0)
		{
			Points.Resize(numPoints);
			IS.Read(&Points[0], sizeof(Float3) * numPoints);
		}

		int numEdges = 0;
		IS >> numEdges;
		if (numEdges > 0)
		{
			Edges.Resize(numEdges);
			IS.Read(&Edges[0], sizeof(NavEdge) * numEdges);
		}

		int numTris = 0;
		IS >> numTris;
		if (numTris > 0)
		{
			Triangles.Resize(numTris);
			IS.Read(&Triangles[0], sizeof(NavTriangle) * numTris);
		}

		IS >> GridWidth;
		IS >> GridHeight;
		if (GridWidth * GridHeight > 0)
		{
			Grids.Resize(GridWidth * GridHeight);
			IS.Read(&Grids[0], sizeof(NavGrid) * GridWidth * GridHeight);
		}

		int numGridedTris;
		IS >> numGridedTris;
		if (numGridedTris > 0)
		{
			GridedTriangles.Resize(numGridedTris);
			IS.Read(&GridedTriangles[0], sizeof(uint16_t) * numGridedTris);
		}

		return true;
	}

	void NavData::Save(const String & filename)
	{
		FILE * fp = fopen(filename.c_str(), "wb");
		if (!fp)
		{
			d_log("!: NavData save failed, '%s'.", filename.c_str());
			return ;
		}

		OSerializerF OS(fp, true);

		OS << (int)(NAV_MAGIC);
		OS << (int)(NAV_VERSION);
		
		OS << Bound.x1;
		OS << Bound.y1;
		OS << Bound.x2;
		OS << Bound.y2;

		OS << Points.Size();
		if (Points.Size() > 0)
		{
			OS.Write(&Points[0], sizeof(Float3) * Points.Size());
		}

		OS << Edges.Size();
		if (Edges.Size() > 0)
		{
			OS.Write(&Edges[0], sizeof(NavEdge) * Edges.Size());
		}

		OS << Triangles.Size();
		if (Triangles.Size() > 0)
		{
			OS.Write(&Triangles[0], sizeof(NavTriangle) * Triangles.Size());
		}

		OS << GridWidth;
		OS << GridHeight;
		if (Grids.Size() > 0)
		{
			OS.Write(&Grids[0], sizeof(NavGrid) * Grids.Size());
		}

		OS << GridedTriangles.Size();
		if (GridedTriangles.Size() > 0)
		{
			OS.Write(&GridedTriangles[0], sizeof(uint16_t) * GridedTriangles.Size());
		}
	}

	void NavData::Clear()
	{
		Points.Clear();
		Edges.Clear();
		Triangles.Clear();

		GridWidth = GridHeight = 0;
		Grids.Clear();
		GridedTriangles.Clear();
	}

	bool NavData::Empty() const
	{
		return Points.Empty();
	}

	void NavData::Invalid()
	{
		Triangles.Clear();

		GridWidth = GridHeight = 0;
		Grids.Clear();
		GridedTriangles.Clear();
	}

	bool NavData::Build()
	{
		bool is_error = false;

		// Triangulate
		Triangles.Clear();

		for (int i = 0; i < Edges.Size(); ++i)
		{
			Edges[i].flag = 0;
			Edges[i].triangleId[0] = Edges[i].triangleId[1] = NAV_INVALID_ID;
		}

		FixedSet<int, 32> t_set;
		for (int i = 0; i < Edges.Size(); ++i)
		{
			NavEdge & e1 = Edges[i];

			for (int j = 0; j < Edges.Size(); ++j)
			{
				NavEdge & e2 = Edges[j];

				t_set.Clear();
				t_set.Insert(e1.pointId[0]);
				t_set.Insert(e1.pointId[1]);
				t_set.Insert(e2.pointId[0]);
				t_set.Insert(e2.pointId[1]);

				if (t_set.Size() != 3)
					continue ;

				for (int k = 0; k < Edges.Size(); ++k)
				{
					if (k == i || k == j)
						continue ;

					NavEdge & e3 = Edges[k];

					t_set.Clear();
					t_set.Insert(e1.pointId[0]);
					t_set.Insert(e1.pointId[1]);
					t_set.Insert(e2.pointId[0]);
					t_set.Insert(e2.pointId[1]);
					t_set.Insert(e3.pointId[0]);
					t_set.Insert(e3.pointId[1]);

					if (t_set.Size() == 3)
					{
						NavTriangle tri;
						tri.pointId[0] = t_set[0];
						tri.pointId[1] = t_set[1];
						tri.pointId[2] = t_set[2];
						tri.edgeId[0] = i;
						tri.edgeId[1] = j;
						tri.edgeId[2] = k;

						// ensure clockwise
						const Float3 & A = Points[tri.pointId[0]];
						const Float3 & B = Points[tri.pointId[1]];
						const Float3 & C = Points[tri.pointId[2]];
						Float3 Y = Float3::Cross(B - A, C - A);
						if (Y.y < 0)
						{
							Swap(tri.pointId[0], tri.pointId[1]);
						}

						tri.center = (A + B + C) / 3;

						/*
						tri.bound.x1 = A.x;
						tri.bound.y1 = A.z;
						tri.bound.x2 = A.x;
						tri.bound.y2 = A.z;

						tri.bound.x1 = Min(B.x, tri.bound.x1);
						tri.bound.y1 = Min(B.y, tri.bound.y1);
						tri.bound.x2 = Max(B.x, tri.bound.x2);
						tri.bound.y2 = Max(B.y, tri.bound.y2);

						tri.bound.x1 = Min(C.x, tri.bound.x1);
						tri.bound.y1 = Min(C.y, tri.bound.y1);
						tri.bound.x2 = Max(C.x, tri.bound.x2);
						tri.bound.y2 = Max(C.y, tri.bound.y2);
						*/

						if (!_hasTriangle(tri.pointId[0], tri.pointId[1], tri.pointId[2]))
						{
							Triangles.PushBack(tri);
							int index = Triangles.Size() - 1;

							NavEdge * edge[3] = { &e1, &e2, &e3 };
							for (int e = 0; e < 3; ++e)
							{
								if (edge[e]->triangleId[0] == NAV_INVALID_ID)
								{
									edge[e]->triangleId[0] = index;
								}
								else if (edge[e]->triangleId[1] == NAV_INVALID_ID)
								{
									edge[e]->triangleId[1] = index;
								}
								else
								{
									edge[e]->flag = NAV_EAGE_FLAG_ERROR;
									is_error = true;
								}
							}
						}
					}
				}
			}
		}

		for (int i = 0; i < Edges.Size(); ++i)
		{
			if (Edges[i].triangleId[0] == NAV_INVALID_ID || Edges[i].triangleId[1] == NAV_INVALID_ID)
			{
				Edges[i].flag = NAV_EAGE_FLAG_BOUND;
			}
		}

		// Init Bound
		if (Points.Size() > 0)
		{
			Bound.x1 = Points[0].x;
			Bound.y1 = Points[0].z;
			Bound.x2 = Points[0].x;
			Bound.y2 = Points[0].z;

			for (int i = 1; i < Points.Size(); ++i)
			{
				Bound.x1 = Min(Bound.x1, Points[i].x);
				Bound.y1 = Min(Bound.y1, Points[i].z);
				Bound.x2 = Max(Bound.x2, Points[i].x);
				Bound.y2 = Max(Bound.y2, Points[i].z);
			}
		}
		else
		{
			Bound = RectF(0, 0, 0, 0);
		}

		// Init Grids
		Grids.Clear();
		float width = Bound.Width();
		float height = Bound.Height();

		GridWidth = (int)(width / NAV_GRID_SIZE) + 1;
		GridHeight = (int)(height / NAV_GRID_SIZE) + 1;

		Bound.x2 = Bound.x1 + GridWidth * NAV_GRID_SIZE;
		Bound.y2 = Bound.y1 + GridHeight * NAV_GRID_SIZE;

		Grids.Resize(GridWidth * GridHeight);

		int gindex = 0;
		for (int j = 0 ; j < GridHeight; ++j)
		{
			for (int i = 0; i < GridWidth; ++i)
			{
				NavGrid & grid = Grids[gindex++];
				grid.startIndex = GridedTriangles.Size();

				RectF rect;
				rect.x1 = Bound.x1 + i * NAV_GRID_SIZE;
				rect.y1 = Bound.y1 + j * NAV_GRID_SIZE;
				rect.x2 = rect.x1 + NAV_GRID_SIZE;
				rect.y2 = rect.y1 + NAV_GRID_SIZE;

				for (int t = 0; t < Triangles.Size(); ++t)
				{
					if (_intersects(rect, Triangles[t]))
					{
						GridedTriangles.PushBack(t);
					}
				}

				grid.endIndex = GridedTriangles.Size();
			}
		}

		return is_error;
	}

	int NavData::AddPoint(const Float3 & point)
	{
		Points.PushBack(point);

		return Points.Size() - 1;
	}

	void NavData::RemovePoint(int pointId)
	{
		for (int i = 0; i < Edges.Size(); ++i)
		{
			if (Edges[i].pointId[0] == pointId || 
				Edges[i].pointId[1] == pointId)
				Edges.Erase(i--);
		}

		for (int i = 0; i < Edges.Size(); ++i)
		{
			NavEdge & e = Edges[i];

			if (e.pointId[0] > pointId)
				e.pointId[0] -= 1;

			if (e.pointId[1] > pointId)
				e.pointId[1] -= 1;
		}

		Points.Erase(pointId);
	}

	void NavData::AddEdge(int p0, int p1)
	{
		d_assert (p0 != NAV_INVALID_ID && p1 != NAV_INVALID_ID && p0 != p1);

		NavEdge e;
		e.pointId[0] = p0;
		e.pointId[1] = p1;

		FixedSet<int, 2> t_set1;
		t_set1.Insert(p0);
		t_set1.Insert(p1);

		for (int i = 0; i < Edges.Size(); ++i)
		{
			FixedSet<int, 2> t_set2;
			t_set2.Insert(Edges[i].pointId[0]);
			t_set2.Insert(Edges[i].pointId[1]);

			if (t_set1[0] == t_set2[0] && t_set1[1] == t_set2[1])
				return ;
		}

		Edges.PushBack(e);
	}

	void NavData::RemoveEdge(int edgeId)
	{
		Edges.Erase(edgeId);
		Invalid();
	}

	int NavData::MapGridIndex(const Float3 & p) const
	{
		float x = p.x - Bound.x1;
		float y = p.z - Bound.y1;

		Int2 r;
		r.x = (int)(x / NAV_GRID_SIZE);
		r.y = (int)(y / NAV_GRID_SIZE);

		if (r.x < 0 || r.x > GridWidth - 1 ||
			r.y < 0 || r.y > GridHeight - 1)
			return NAV_INVALID_ID;

		return r.y * GridWidth + r.x;
	}

	int NavData::MapTriangleIndex(const Float3 & p) const
	{
		int gridIndex = MapGridIndex(p);
		if (gridIndex != NAV_INVALID_ID)
		{
			const NavGrid & grid = Grids[gridIndex];

			for (int i = grid.startIndex; i < grid.endIndex; ++i)
			{
				uint16_t triId = GridedTriangles[i];
				const NavTriangle & tri = Triangles[triId];

				const Float3 & A = Points[tri.pointId[0]];
				const Float3 & B = Points[tri.pointId[1]];
				const Float3 & C = Points[tri.pointId[2]];
				const Float3 & P = p;

				/*
				Float3 AB(B - A), AC(C - A), AP(P - A);

				float abc = AB.x * AC.z - AB.z * AC.x;
				float abp = AB.x * AP.z - AB.z * AP.x;
				float apc = AP.x * AC.z - AP.z * AC.x;
				float pbc = abc - abp - apc; 

				float delta = fabs(abc) - fabs(abp) - fabs(apc) - fabs(pbc);

				if (fabs(delta) > EPSILON_E5)
					continue;
				*/

				Ray ray;
				ray.orig = P + Float3(0, NAV_ADJUST_DIST, 0);
				ray.dir = Float3(0, -1, 0);

				if (ray.Intersect(NULL, A, B, C))
					return triId;
			}
		}

		return NAV_INVALID_ID;
	}

	int NavData::MapNearestTriangle(const Float3 & p) const
	{
		float c_delta = MAX_FLOAT;
		int triId = NAV_INVALID_ID;
		int gridIndex = MapGridIndex(p);

		if (gridIndex != NAV_INVALID_ID)
		{
			const NavGrid & grid = Grids[gridIndex];

			for (int i = grid.startIndex; i < grid.endIndex; ++i)
			{
				const NavTriangle & tri = Triangles[GridedTriangles[i]];

				const Float3 & A = Points[tri.pointId[0]];
				const Float3 & B = Points[tri.pointId[1]];
				const Float3 & C = Points[tri.pointId[2]];
				const Float3 & P = p;

				Float3 AB(B - A), AC(C - A), AP(P - A);

				float abc = AB.x * AC.z - AB.z * AC.x;
				float abp = AB.x * AP.z - AB.z * AP.x;
				float apc = AP.x * AC.z - AP.z * AC.x;
				float pbc = abc - abp - apc; 

				float delta = fabs(fabs(abc) - fabs(abp) - fabs(apc) - fabs(pbc));
				if (delta < c_delta)
				{
					c_delta = delta;
					triId = GridedTriangles[i];
				}
			}
		}

		return triId;
	}

	inline bool NavLineIntersect(float & r, float & s, const Float3 & A, const Float3 & B, const Float3 & C, const Float3 & D)
	{
		float denominator = (B.x-A.x)*(D.z-C.z) - (B.z-A.z)*(D.x-C.x);

		if (Math::Abs(denominator) < EPSILON_E4)
			return false;

		float numeratorR = (A.z-C.z)*(D.x-C.x) - (A.x-C.x)*(D.z-C.z);
		float numeratorS = (A.z-C.z)*(B.x-A.x) - (A.x-C.x)*(B.z-A.z);

		r = numeratorR / denominator;
		s = numeratorS / denominator;

		if (r >= 0 && r <= 1 && s >= 0 && s <= 1)
			return true;
		else
			return false;
	}

	bool NavData::LOSTest(const Float3 & start, const Float3 & end, uint16_t startTriId, uint16_t endTriId) const
	{
		float r, s;
		int step = 0;
		uint16_t lastEdgeId = NAV_INVALID_ID;
		while (startTriId != NAV_INVALID_ID && startTriId != endTriId && step++ < 100)
		{
			const NavTriangle * t = &Triangles[startTriId];

			for (int i = 0; i < 3; ++i)
			{
				if (lastEdgeId != t->edgeId[i])
				{
					const NavEdge * e = &Edges[t->edgeId[i]];

					const Float3 & A = Points[e->pointId[0]];
					const Float3 & B = Points[e->pointId[1]];

					if (NavLineIntersect(r, s, A, B, start, end))
					{
						startTriId = e->triangleId[0] == startTriId ? e->triangleId[1] : e->triangleId[0];
						lastEdgeId = t->edgeId[i];

						break;
					}
				}
			}
		}

		return startTriId == endTriId;
	}
	
	bool NavData::LOSTestEx(Float3 & pos, const Float3 & start, const Float3 & end, uint16_t startTriId, uint16_t endTriId) const
	{
		float r, s;
		int step = 0;
		uint16_t lastEdgeId = NAV_INVALID_ID;
		while (startTriId != NAV_INVALID_ID && startTriId != endTriId && step++ < 100)
		{
			const NavTriangle * t = &Triangles[startTriId];

			for (int i = 0; i < 3; ++i)
			{
				if (lastEdgeId != t->edgeId[i])
				{
					const NavEdge * e = &Edges[t->edgeId[i]];

					const Float3 & A = Points[e->pointId[0]];
					const Float3 & B = Points[e->pointId[1]];

					if (NavLineIntersect(r, s, A, B, start, end))
					{
						startTriId = e->triangleId[0] == startTriId ? e->triangleId[1] : e->triangleId[0];
						lastEdgeId = t->edgeId[i];

						break;
					}
				}
			}
		}

		if (lastEdgeId != NAV_INVALID_ID)
		{
			const NavEdge * e = &Edges[lastEdgeId];

			const Float3 & A = Points[e->pointId[0]];
			const Float3 & B = Points[e->pointId[1]];

			pos.x = A.x+r*(B.x-A.x);
			pos.y = A.y+r*(B.y-A.y);
			pos.z = A.z+r*(B.z-A.z);
		}

		return startTriId == endTriId;
	}
}