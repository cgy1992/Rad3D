/*
*	NavPathFinder
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MRef.h"
#include "MLinker.h"
#include "MEvent.h"
#include "MSmartPtr.h"
#include "NavData.h"

namespace Rad {

	struct NavPathPoint
	{
		int flag;
		word pointId;
		word triangleId;
		Float3 position;
	};

	class M_ENTRY NavPathFinder
	{
		DECLARE_REF_MT();

		struct PathNode
		{
			DECLARE_LINKER(PathNode);

			word triId;
			word edgeId;
			float s, g;

			PathNode * parent;
		};

	public:
		tEvent0 E_ThreadCallback; // in multi-thread.

	public:
		NavPathFinder(const NavData * data);
		~NavPathFinder();

		void
			Release() { delete this; }

		void 
			Clear();
		void 
			Do(const Float3 & start, const Float3 & end, int max_step);

		const Array<NavPathPoint> & 
			GetPath() { return mPathPoints; }

	protected:
		PathNode * 
			AllocNode();
		bool 
			IsNear(int x, float s, float g);

		void 
			PushOpen(int triId, int edgeId, float s, float g, PathNode * p);
		PathNode * 
			PopOpen();

		void 
			PushClose(PathNode * n);

		float 
			_calcuG(int i1, int i2);

		void 
			_smoothPath(const Float3 & start, const Float3 & end);

	protected:
		const NavData * mData;

		PathNode k_end;
		PathNode * p_end;

		PathNode * mOpenLinker;
		PathNode * mCloseLinker;
		PathNode * mPathLinker;
		Allocator<PathNode, 128> mAlloc;

		Array<NavPathPoint> mPathPoints;
	};

	typedef SmartPtr<NavPathFinder> NavPathFinderPtr;

}