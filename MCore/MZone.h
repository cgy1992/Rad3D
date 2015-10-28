/*
*	Zone
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MNode.h"
#include "MCamera.h"

namespace Rad {

	class WorldSection;

	struct RayCheckContract
	{
		struct SortOp
		{
			int operator ()(const RayCheckContract & a, const RayCheckContract & b) const
			{
				return a.contract_dist < b.contract_dist ? -1 : (a.contract_dist > b.contract_dist ? 1 : 0);
			};
		};

		Node * node;
		float contract_dist;
	};

	class M_ENTRY Zone
	{
	public:
		Zone(WorldSection * section);
		~Zone();

		void 
			Resize(Aabb aabb, int w, int h, int d, int level);

		bool 
			Contain(Node * n);
		bool 
			AddNode(Node * n);
		void 
			RemoveNode(Node * n);

		void
			AddChild(Zone * child);
		void
			RemoveChild(Zone * child, bool _delete);
		void
			RemoveAllChild(bool _delete);
		int
			GetChildCount();

		void 
			ImpVisiblityCull(Array<Node*> & visibleArray, Camera * camera);

		void 
			RayCheck(Array<RayCheckContract> & contractArray, const Ray & ray, float dist, int flags);

	protected:
		WorldSection * mSection;
		Aabb mAabb;
		Zone * mParent;
		Array<Zone *> mChildren; 

		Node::ZoneLinker * mNodeLinker;
	};

	
}