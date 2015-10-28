/*
*	NavData
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MMath2.h"
#include "MString.h"

namespace Rad {

#define NAV_MAX_POINT MAX_SHORT
#define NAV_INVALID_ID 0xFFFF
#define NAV_GRID_SIZE (5 * UNIT_METRES)
#define NAV_INTERSECT_DIST (1 * UNIT_METRES)
#define NAV_ADJUST_DIST (0.5 * UNIT_METRES)

#define NAV_EAGE_FLAG_ERROR 1
#define NAV_EAGE_FLAG_BOUND 2

#define NAV_PATH_FLAG_SMOOTH 1
#define NAV_PATH_FLAG_INVALID 2

#define NAV_MAGIC MAKE_DWORD('N', 'A', 'V', 'D')
#define NAV_VERSION 1001

	struct NavEdge
	{
		int flag;
		word pointId[2];
		word triangleId[2];

		NavEdge()
		{
			flag = 0;

			pointId[0] = pointId[1] = NAV_INVALID_ID;
			triangleId[0] = triangleId[1] = NAV_INVALID_ID; 
		}
	};

	struct NavTriangle
	{
		Float3 center;
		word pointId[3];
		word edgeId[3];
	};

	struct NavGrid
	{
		int startIndex;
		int endIndex;
	};

	struct M_ENTRY NavData
	{
		RectF Bound;

		Array<Float3> Points;
		Array<NavEdge> Edges;
		Array<NavTriangle> Triangles;

		int GridWidth, GridHeight;
		Array<NavGrid> Grids;
		Array<word> GridedTriangles;

		NavData();
		~NavData();

		bool 
			Load(const String & filename);
		void 
			Save(const String & filename);
		void 
			Clear();

		int 
			MapGridIndex(const Float3 & p) const;
		int 
			MapTriangleIndex(const Float3 & p) const;
		int 
			MapNearestTriangle(const Float3 & p) const;

		bool 
			LOSTest(const Float3 & start, const Float3 & end, uint16_t startTriId, uint16_t endTriId) const;
		bool 
			LOSTestEx(Float3 & pos, const Float3 & start, const Float3 & end, uint16_t startTriId, uint16_t endTriId) const;

		//
		bool
			Empty() const;
		void 
			Invalid();
		bool 
			Build();

		int 
			AddPoint(const Float3 & points);
		void 
			RemovePoint(int pointId);

		void 
			AddEdge(int p0, int p1);
		void 
			RemoveEdge(int edgeId);

	protected:
		bool 
			_intersects(const RectF & rect, const NavTriangle & tri) const;
		bool 
			_hasTriangle(int pa, int pb, int pc) const;
	};

}