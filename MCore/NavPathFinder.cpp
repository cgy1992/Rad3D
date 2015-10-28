#include "NavPathFinder.h"
#include "MUtil.h"

namespace Rad {

	inline float _NAV_BEST_POINT(const Float3 & A, const Float3 & B, const Float3 & C)
	{
		Float2 AB = Float2(A.x - B.x, A.z - B.z);
		Float2 AC = Float2(A.x - C.x, A.z - C.z);

		/*
		// quad area
		float k = Float2::Cross(AB, AC);

		return Math::Abs(k);
		*/

		return AB.LengthSq() + AC.LengthSq();
	}

	NavPathFinder::NavPathFinder(const NavData * data)
		: mData(data)
	{
		mOpenLinker = NULL;
		mCloseLinker = NULL;
		mPathLinker = NULL;

		p_end = &k_end;
		p_end->triId = NAV_INVALID_ID;
		p_end->s = p_end->g = MAX_FLOAT;
		p_end->parent = NULL;
	}

	NavPathFinder::~NavPathFinder()
	{
	} 

	void NavPathFinder::Do(const Float3 & start, const Float3 & end, int max_step)
	{
		Clear();

		int startIndex = mData->MapTriangleIndex(start);
		int endIndex = mData->MapTriangleIndex(end);

		if (startIndex == NAV_INVALID_ID)
		{
			startIndex = mData->MapNearestTriangle(start);

			if (startIndex == NAV_INVALID_ID)
			{
				return ;
			}
		}

		if (endIndex == NAV_INVALID_ID)
		{
			Float3 pos;
			if (mData->LOSTestEx(pos, start, end, startIndex, NAV_INVALID_ID))
			{
				Float3 dir = pos - start;

				if (dir.Normalize() > NAV_ADJUST_DIST)
				{
					pos -= dir * NAV_ADJUST_DIST;

					NavPathPoint point;
					point.flag = NAV_PATH_FLAG_SMOOTH | NAV_PATH_FLAG_INVALID;
					point.triangleId = NAV_INVALID_ID;
					point.pointId = NAV_INVALID_ID;
					point.position = start;
					mPathPoints.PushBack(point);

					point.position = pos;
					mPathPoints.PushBack(point);
				}
			}
		}
		else if (startIndex == endIndex)
		{
			NavPathPoint point;
			point.flag = NAV_PATH_FLAG_SMOOTH;
			point.triangleId = startIndex;
			point.pointId = NAV_INVALID_ID;
			point.position = start;
			mPathPoints.PushBack(point);

			point.flag = NAV_PATH_FLAG_SMOOTH;
			point.triangleId = endIndex;
			point.pointId = NAV_INVALID_ID;
			point.position = end;
			mPathPoints.PushBack(point);

			return ;
		}
		else
		{
			PushOpen(startIndex, NAV_INVALID_ID, 0, _calcuG(startIndex, endIndex), NULL);

			int step = 0;
			while (mOpenLinker != p_end && step < max_step)
			{
				PathNode * node = PopOpen();

				PushClose(node);

				if (node->triId == endIndex)
					break;

				float s = node->s;
				const NavTriangle & tri = mData->Triangles[node->triId];
				const NavEdge * edge[3] = { 
					&mData->Edges[tri.edgeId[0]],
					&mData->Edges[tri.edgeId[1]],
					&mData->Edges[tri.edgeId[2]]
				};

				for (int i = 0; i < 3; ++i)
				{
					if (edge[i]->triangleId[0] != NAV_INVALID_ID && edge[i]->triangleId[0] != node->triId)
					{
						s += _calcuG(node->triId, edge[i]->triangleId[0]);
						PushOpen(edge[i]->triangleId[0], tri.edgeId[i], s, s + _calcuG(edge[i]->triangleId[0], endIndex), node);
					}
					else if (edge[i]->triangleId[1] != NAV_INVALID_ID && edge[i]->triangleId[1] != node->triId)
					{
						s += _calcuG(node->triId, edge[i]->triangleId[1]);
						PushOpen(edge[i]->triangleId[1], tri.edgeId[i], s, s + _calcuG(edge[i]->triangleId[1], endIndex), node);
					}
				}

				++step;
			}

			if (step < max_step && mCloseLinker != NULL && mCloseLinker->triId == endIndex)
			{
				NavPathPoint point;
				point.flag = 0;
				point.triangleId = startIndex;
				point.pointId = NAV_INVALID_ID;
				point.position = start;
				mPathPoints.PushBack(point);

				PathNode * path = mCloseLinker;
				while (path != NULL)
				{
					LINKER_REMOVE(mCloseLinker, path);

					LINKER_APPEND(mPathLinker, path);

					path = path->parent;
				}

				PathNode * first = mPathLinker;
				PathNode * second = LINKER_NEXT(mPathLinker);
				first->edgeId = second->edgeId;

				path = mPathLinker;
				while (path != NULL)
				{
					const NavEdge & edge = mData->Edges[path->edgeId];
					const Float3 & A = mData->Points[edge.pointId[0]];
					const Float3 & B = mData->Points[edge.pointId[1]];

					float da = _NAV_BEST_POINT(A, start, end);
					float db = _NAV_BEST_POINT(B, start, end);

					point.triangleId = path->triId;

					if (da < db)
					{
						point.pointId = edge.pointId[0];
						point.position = A;
					}
					else
					{
						point.pointId = edge.pointId[1];
						point.position = B;
					}

					const NavTriangle & tri = mData->Triangles[path->triId];
					int ei = 0;
					Float3 ep[3];
					for (int i = 0; i < 3; ++i)
					{
						const NavEdge & e = mData->Edges[tri.edgeId[i]];
						if (e.pointId[0] == point.pointId)
						{
							ep[ei++] = mData->Points[e.pointId[1]];
						}
						else if (e.pointId[1] == point.pointId)
						{
							ep[ei++] = mData->Points[e.pointId[0]];
						}
					}

					d_assert (ei == 2);

					Float3 dir1 = ep[0] - point.position;
					Float3 dir2 = ep[1] - point.position;
					util::FastNormalize3(dir1);
					util::FastNormalize3(dir2);

					Float3 dir = (dir1 + dir2) * 0.5f;
					point.position += dir * NAV_ADJUST_DIST;

					mPathPoints.PushBack(point);

					path = LINKER_NEXT(path);
				}

				point.triangleId = endIndex;
				point.pointId = NAV_INVALID_ID;
				point.position = end;
				mPathPoints.PushBack(point);

				_smoothPath(start, end);
			}
		}
	}

	void NavPathFinder::_smoothPath(const Float3 & start, const Float3 & end)
	{
		int checkIndex = 0;
		int nextIndex = 1;
		while (nextIndex < mPathPoints.Size())
		{
			for (int i = nextIndex; i < mPathPoints.Size(); ++i)
			{
				if (mData->LOSTest(mPathPoints[checkIndex].position, mPathPoints[i].position, 
					mPathPoints[checkIndex].triangleId, mPathPoints[i].triangleId))
					nextIndex = i;
			}

			mPathPoints[checkIndex].flag |= NAV_PATH_FLAG_SMOOTH;
			mPathPoints[nextIndex].flag |= NAV_PATH_FLAG_SMOOTH;

			checkIndex = nextIndex;
			nextIndex += 1;
		}
	}

	void NavPathFinder::Clear()
	{
		mPathPoints.Clear();
		mAlloc.Clear();
		mOpenLinker = NULL;
		mCloseLinker = NULL;
		mPathLinker = NULL;
		p_end->__link_node.prev = NULL;
		p_end->__link_node.next = NULL;

		LINKER_APPEND(mOpenLinker, p_end);
	}

	NavPathFinder::PathNode * NavPathFinder::AllocNode()
	{
		PathNode * node = mAlloc.Alloc();
		node->__link_node.prev = NULL;
		node->__link_node.next = NULL;
		node->parent = NULL;

		return node;
	}

	bool NavPathFinder::IsNear(int index, float s, float g)
	{
		PathNode * node = mCloseLinker;
		while (node != NULL)
		{
			if (node->triId == index)
			{
				if (g >= node->g)
				{
					return false;
				}
				else
				{
					LINKER_REMOVE(mCloseLinker, node);
					mAlloc.Free(node);
					break;
				}
			}

			node = LINKER_NEXT(node);
		}

		return true;
	}

	void NavPathFinder::PushOpen(int triId, int edgeId, float s, float g, PathNode * p)
	{
		if (!IsNear(triId, s, g))
			return ;

		PathNode * node = mOpenLinker;
		while (node != NULL)
		{
			if (node->triId == triId)
			{
				if (node->g <= g)
					return ;

				LINKER_REMOVE(mOpenLinker, node);

				break;
			}

			node = LINKER_NEXT(node);
		}

		if (node == NULL)
		{
			node = AllocNode();
			node->triId = triId;
		}

		node->parent = p;
		node->edgeId = edgeId;
		node->s = s;
		node->g = g;

		d_assert (p == NULL || p->parent != node);

		PathNode * at = mOpenLinker;
		while (at != NULL)
		{
			if (at->g >= node->g)
			{
				LINKER_INSERT(mOpenLinker, at, node);

				return ;
			}

			at = LINKER_NEXT(at);
		}

		d_assert (0);
	}

	NavPathFinder::PathNode * NavPathFinder::PopOpen()
	{
		d_assert (mOpenLinker != p_end);

		PathNode * node = mOpenLinker;

		LINKER_REMOVE(mOpenLinker, node);

		return node;
	}

	void NavPathFinder::PushClose(PathNode * n)
	{
		LINKER_APPEND(mCloseLinker, n);
	}

	float NavPathFinder::_calcuG(int i1, int i2)
	{
		const Float3 & p1 = mData->Triangles[i1].center;
		const Float3 & p2 = mData->Triangles[i2].center;

		return p1.DistanceSq(p2);
	}

}