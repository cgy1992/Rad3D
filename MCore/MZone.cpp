#include "MZone.h"
#include "MWorldSection.h"

namespace Rad {

	Zone::Zone(WorldSection * section)
		: mSection(section)
		, mParent(NULL)
		, mNodeLinker(NULL)
		, mAabb(Aabb::Infinite)
	{
	}

	Zone::~Zone()
	{
		while (mNodeLinker)
		{
			RemoveNode(mNodeLinker->GetNode());
		}

		for (int i = 0; i < mChildren.Size(); ++i)
		{
			delete mChildren[i];
		}
	}

	bool Zone::Contain(Node * pNode)
	{
		return mAabb.Contain(pNode->GetWorldAabb());
	}

	bool Zone::AddNode(Node * pNode)
	{
		d_assert (pNode->GetZone() == NULL);

		if (Contain(pNode))
		{
			int i = 0;
			for (i = 0; i < mChildren.Size(); ++i)
			{
				if (mChildren[i]->AddNode(pNode))
					return true;
			}

			pNode->_notifyAttachZone(this);

			LINKER_APPEND(mNodeLinker, pNode->GetZoneLinker());
			
			return true;
		}

		return false;
	}

	void Zone::RemoveNode(Node * pNode)
	{
		d_assert (pNode->GetZone() == this);

		LINKER_REMOVE(mNodeLinker, pNode->GetZoneLinker());

		pNode->_notifyDetachZone();
	}

	void Zone::AddChild(Zone * child)
	{
		d_assert (child->mParent == NULL);

		child->mParent = this;
		mChildren.PushBack(child);
	}

	void Zone::RemoveChild(Zone * child, bool _delete)
	{
		d_assert (child->mParent == this);

		child->mParent = NULL;
		for (int i = 0; i < mChildren.Size(); ++i)
		{
			if (mChildren[i] == child)
			{
				if (_delete)
				{
					delete mChildren[i];
				}

				mChildren.Erase(i);
				return ;
			}
		}

		d_assert (0);
	}

	void Zone::RemoveAllChild(bool _delete)
	{
		for (int i = 0; i < mChildren.Size(); ++i)
		{
			if (_delete)
			{
				delete mChildren[i];
			}
		}

		mChildren.Clear();
	}

	int Zone::GetChildCount()
	{
		return mChildren.Size();
	}

	void Zone::Resize(Aabb aabb, int w, int h, int d, int level)
	{
		d_assert (mSection != NULL && "Main zone can't resize able.");
		d_assert (w > 0 && h > 0 && d > 0);

		mAabb = aabb;

		while (mNodeLinker != NULL)
		{
			mNodeLinker->GetNode()->ChangeTm(eTmFlags::UNKNOWN);

			RemoveNode(mNodeLinker->GetNode());
		}

		mChildren.Clear();

		if (w * h * d == 1)
			return ;

		mChildren.Resize(w * h * d);

		Float3 step = aabb.GetSize() * Float3(1.0f / w, 1.0f / h, 1.0f / d);

		int index = 0;
		for (int i = 0; i < w; ++i)
		{
			for (int j = 0; j < h; ++j)
			{
				for (int k = 0; k < d; ++k)
				{
					Aabb myAabb;

					myAabb.minimum = aabb.minimum + step * Float3((float)i, (float)j, (float)k);
					myAabb.maximum = myAabb.minimum + step;

					mChildren[index] = new Zone(mSection);
					mChildren[index]->mParent = this;
					mChildren[index]->mAabb = myAabb;

					if (level - 1  > 0)
						mChildren[index]->Resize(myAabb, w, h, d, level - 1);

					++index;
				}
			}
		}
	}

	void Zone::ImpVisiblityCull(Array<Node*> & visibleArray, Camera * camera)
	{
		Camera::Visibility vb = camera->GetVisibility(mAabb);

		if (vb == Camera::VB_PARTIAL)
		{
			Node::ZoneLinker * pNodeLinker = mNodeLinker;

			while (pNodeLinker != NULL)
			{
				Node * pNode = pNodeLinker->GetNode();

				if (pNode->IsVisible() && camera->GetVisibility(pNode->GetWorldAabb()) != Camera::VB_NONE)
				{
					visibleArray.PushBack(pNode);
				}

				pNodeLinker = LINKER_NEXT(pNodeLinker);
			}

			for (int i = 0; i < mChildren.Size(); ++i)
			{
				mChildren[i]->ImpVisiblityCull(visibleArray, camera);
			}
		}
		else if (vb == Camera::VB_FULL)
		{
			Node::ZoneLinker * pNodeLinker = mNodeLinker;

			while (pNodeLinker != NULL)
			{
				Node * pNode = pNodeLinker->GetNode();

				visibleArray.PushBack(pNode);

				pNodeLinker = LINKER_NEXT(pNodeLinker);
			}

			for (int i = 0; i < mChildren.Size(); ++i)
			{
				mChildren[i]->ImpVisiblityCull(visibleArray, camera);
			}
		}
	}

	void Zone::RayCheck(Array<RayCheckContract> & contractArray, const Ray & ray, float dist, int flags)
	{
		float contract_dist = -1;
		if (!ray.Intersect(&contract_dist, mAabb) && contract_dist >= dist)
			return ;

		Node::ZoneLinker * pNodeLinker = mNodeLinker;

		while (pNodeLinker != NULL)
		{
			Node * pNode = pNodeLinker->GetNode();

			if (pNode->GetFlags() & flags)
			{
				const Aabb & aabb = pNode->GetWorldAabb();

				if (ray.Intersect(&contract_dist, aabb))
				{
					RayCheckContract info;

					info.node = pNode;
					info.contract_dist = contract_dist;

					contractArray.PushBack(info);
				}
			}

			pNodeLinker = LINKER_NEXT(pNodeLinker);
		}

		for (int i = 0; i < mChildren.Size(); ++i)
		{
			mChildren[i]->RayCheck(contractArray, ray, dist, flags);
		}
	}

}
