/*
*	KD-Tree
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MMath.h"
#include "MArray.h"
#include "MAllocator.h"

namespace Rad {

	template <class T>
	class KDTree
	{
		DECLARE_ALLOC();

	public:
		enum eAxis {
			N_AXIS = 0,
			X_AXIS,
			Y_AXIS,
			Z_AXIS,
		};

		struct Node
		{
			int axis;
			Aabb aabb;
			Array<T> elems;

			Node * child[2];

			Node()
			{
				aabb = Aabb::Zero;
				axis = N_AXIS;
				child[0] = NULL;
				child[1] = NULL;
			}
		};

	public:
		KDTree()
			: mRoot(NULL)
			, mAabb(Aabb::Zero)
		{
		}

		~KDTree()
		{
			Clear();
		}

		void Clear()
		{
			mAlloc.Clear();
			mRoot = NULL;
		}

		void Build(const Aabb & aabb, int depth)
		{
			Clear();

			mRoot = mAlloc.Alloc();

			mRoot->aabb = aabb;
			mRoot->axis = N_AXIS;
			mRoot->child[0] = NULL;
			mRoot->child[1] = NULL;

			_build(mRoot, depth);
		}

		Node * GetRoot()
		{
			return mRoot;
		}

	protected:
		void _build(Node * parent, int depth)
		{
			if (depth < 1)
				return ;

			const Aabb & aabb = parent->aabb;
			Float3 size = aabb.GetSize();
			Float3 split = Float3(1, 1, 1);

			if (size.x >= size.y && size.x >= size.z)
			{
				parent->axis = X_AXIS;
				split.x = 0.5f;
			}
			else if (size.y >= size.x && size.y >= size.z)
			{
				parent->axis = Y_AXIS;
				split.y = 0.5f;
			}
			else if (size.z >= size.x && size.z >= size.y)
			{
				parent->axis = Z_AXIS;
				split.z = 0.5f;
			}

			d_assert (parent->axis != N_AXIS);

			parent->child[0] = mAlloc.Alloc();
			parent->child[1] = mAlloc.Alloc();

			parent->child[0]->aabb = Aabb(aabb.minimum, aabb.minimum + split * size);
			parent->child[1]->aabb = Aabb(aabb.minimum + split * size, aabb.maximum);

			_build(parent->child[0], depth - 1);
			_build(parent->child[1], depth - 1);
		}

	protected:
		Aabb mAabb;
		Node * mRoot;

		Allocator<Node, 64> mAlloc;
	};

}