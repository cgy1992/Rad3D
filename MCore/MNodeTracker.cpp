#include "MNodeTracker.h"
#include "MFunctional.h"

namespace Rad {

	DF_PROPERTY_BEGIN(NodeTracker)
		DF_PROPERTY_EX(NodeTracker, mKeyController, "Animation", "KeyFrame", "PT_KeyController", PT_UserData)
		DF_PROPERTY(NodeTracker, mLooped, "", "Looped", PT_Bool)
		DF_PROPERTY(NodeTracker, mPosition, "", "Position", PT_Float)
	DF_PROPERTY_END()

	ImplementRTTI(NodeTracker, IComponent);

	NodeTracker::NodeTracker()
		: mLooped(false)
		, mPosition(0)
	{
	}

	NodeTracker::~NodeTracker()
	{
	}

	void NodeTracker::Serialize(Serializer & sl)
	{
		IComponent::Serialize(sl);

		if (sl.IsOut())
		{
			OSerializer & OS = (OSerializer &)sl;

			OS << 0x1001;
			mKeyController.Serialize(sl);

			OS << 0;
		}
		else
		{
			ISerializer & IS = (ISerializer &)sl;

			int ckId = 0;
			while (IS.Read(&ckId, sizeof(int)) && ckId != 0)
			{
				if (ckId == 0x1001)
				{
					mKeyController.Serialize(sl);
				}
			}
		}
	}

	void NodeTracker::Attach(ComponentOwner * owner)
	{
		d_assert (KIND_OF(Node, owner));

		IComponent::Attach(owner);
	}

	int NodeTracker::Update(float elapsedTime)
	{
		float length = mKeyController.Size() > 0 ? mKeyController[mKeyController.Size() - 1].time : 0;
		if (length <= 0)
			return 0;

		mPosition += elapsedTime;

		if (mPosition >= length)
		{
			if (mLooped)
			{
				while (mPosition > length)
					mPosition -= length;
			}
			else
			{
				mPosition = length;
			}
		}

		KF_Trasnform kf;
		if (mKeyController.GetValue(kf, mPosition, true))
		{
			Node * node = (Node *)mOwner;
			node->SetPosition(kf.position);
			node->SetRotation(kf.rotation);
			node->SetScale(kf.scale);
		}

		return 0;
	}

	void NodeTracker::SetLooped(bool looped)
	{
		mLooped = looped;
	}

	void NodeTracker::SetPosition(float position)
	{
		mPosition = position;
	}

}

