#include "MNode.h"
#include "MWorld.h"

namespace Rad{

	DF_PROPERTY_BEGIN(Node)
		DF_PROPERTY_EX(Node, mUID, "", "UID", "Static", PT_Int)
		DF_PROPERTY(Node, mName, "", "Name", PT_FixedString)
		DF_PROPERTY_EX(Node, mPosition, "", "Position", "None", PT_Float3)
		DF_PROPERTY_EX(Node, mRotation, "", "Rotation", "None", PT_Float4)
		DF_PROPERTY_EX(Node, mScale, "", "Scale", "None", PT_Float3)
		DF_PROPERTY(Node, mOpacity, "", "Opacity", PT_Float)
		DF_PROPERTY(Node, mLighting, "", "Lighting", PT_Bool)
		DF_PROPERTY(Node, mCastShadow, "", "CastShadow", PT_Bool)
		DF_PROPERTY(Node, mReceiveShadow, "", "ReceiveShadow", PT_Bool)
		DF_PROPERTY(Node, mVisible, "", "Visible", PT_Bool)
	DF_PROPERTY_END();

	//
	ImplementRTTI(Node, ComponentOwner);

	Node::Node(const FixedString32 & name, bool _inworld)
		: mParent(NULL)
		, mZone(NULL)
		, mZoneLinker(this)
		, mSection(NULL)
		, mFlags(0)
		, mTmFlags(eTmFlags::ALL)
		, mTmChangeFlags(eTmFlags::ALL)
		, mUID(0)
		, mName(name)
		, mPosition(Float3::Zero)
		, mRotation(Quat::Identity)
		, mScale(1, 1, 1)
		, mRenderContexId(-1)
		, mOpacity(1)
		, mLighting(true)
		, mCastShadow(false)
		, mReceiveShadow(false)
		, mVisible(true)
		, mWorldPosition(Float3::Zero)
		, mWorldRotation(Quat::Identity)
		, mWorldScale(1, 1, 1)
		, mWorldOpacity(1)
		, mLocalAABB(Aabb::Zero)
		, mWorldAABB(Aabb::Zero)
		, mLocalAABBValid(true)
		, mInheritRotation(true)
		, mInheritScale(true)
		, mInWorld(_inworld)
		, mShape(NULL)
	{
		if (mInWorld)
			LINKER_APPEND(World::Instance()->mNodeLinker, this);

		if (World::Instance()->GetCurrentRenderContext() != NULL)
			mRenderContexId = World::Instance()->GetCurrentRenderContext()->GetId();
	}

	Node::~Node()
	{
		safe_delete(mShape);

		if (mZone)
			mZone->RemoveNode(this);

		d_assert(mZone == NULL);

		for (int i = 0; i < mChildren.Size(); ++i)
			mChildren[i]->_notifyDetach();

		if (mParent)
			mParent->Detach(this);

		if (mInWorld)
			LINKER_REMOVE(World::Instance()->mNodeLinker, this);
	}

	IObject * Node::Clone()
	{
		Node * node = new Node;

		node->Copy(this);

		return node;
	}

	void Node::Attach(Node * child)
	{
		d_assert(child->GetParent() == NULL && child != this);
		
		mChildren.PushBack(child);

		child->_notifyAttach(this);
	}

	void Node::Detach(Node * child)
	{
		d_assert(child->GetParent() == this);

		for (int i = 0; i < mChildren.Size(); ++i)
		{
			if (mChildren[i]== child)
			{
				child->_notifyDetach();

				mChildren.Erase(i);

				return ;
			}
		}

		d_assert(0);
	}

	void Node::_notifyAttach(Node * parent)
	{
		d_assert(mParent == NULL);
		
		mParent = parent;

		ChangeTm(eTmFlags::ALL);
	}

	void Node::_notifyDetach()
	{
		mParent = NULL;

		ChangeTm(eTmFlags::ALL);
	}

	int	Node::GetChildCount()
	{
		return mChildren.Size();
	}

	Node * Node::GetChild(int index)
	{
		return mChildren[index];
	}

	void Node::_notifyAttachZone(Zone * zone)
	{
		d_assert (mZone == NULL);

		mZone = zone;
	}

	void Node::_notifyDetachZone()
	{
		d_assert (mZone != NULL);

		mZone = NULL;
	}

	void Node::_notifyAttachSection(WorldSection * section)
	{
		d_assert (mInWorld && "node must be in world");
		d_assert (mSection == NULL);

		mSection = section;
	}

	void Node::_notifyDetachSection()
	{
		d_assert (mInWorld && "node must be in world");
		d_assert (mSection != NULL);

		mSection = NULL;
	}

	void Node::SetInWorld(bool inworld)
	{
		d_assert (mSection == NULL && "node must be not in section");

		if (mInWorld != inworld)
		{
			if (mInWorld)
			{
				if (mZone != NULL)
					mZone->RemoveNode(this);

				LINKER_REMOVE(World::Instance()->mNodeLinker, this);
				mInWorld = false;
			}
			else
			{
				LINKER_APPEND(World::Instance()->mNodeLinker, this);
				mInWorld = true;
			}
		}
	}

	void Node::SetUID(int uid)
	{
		mUID = uid;
	}

	void Node::SetName(const FixedString32 & name)
	{
		mName = name;
	}

	void Node::SetPosition(const Float3 & rk)
	{
		if (mPosition != rk)
		{
			mPosition = rk;

			ChangeTm(eTmFlags::TRANSLATE);
		}
	}

	void Node::SetPosition(float x, float y, float z)
	{
		SetPosition(Float3(x, y, z));
	}

	void Node::SetRotation(const Quat & rk)
	{
		if (mRotation != rk)
		{
			mRotation = rk;

			ChangeTm(eTmFlags::ROTATE);
		}
	}

	void Node::SetRotation(float x, float y, float z, float w)
	{
		SetRotation(Quat(x, y, z, w));
	}

	void Node::SetScale(const Float3 & rk)
	{
		if (mScale != rk)
		{
			mScale = rk;

			ChangeTm(eTmFlags::SCALE);
		}
	}

	void Node::SetScale(float x, float y, float z)
	{
		SetScale(Float3(x, y, z));
	}

	void Node::SetRotationEx(const Float3 & angles)
	{
		Quat q;

		q.FromEulerAngle(angles);

		SetRotation(q);
	}

	void Node::SetDirection(const Float3 & rk)
	{
		Float3 d = rk;
		Float3 r = GetRotation().GetRightVector();
		Float3 u = GetRotation().GetUpVector();
		
		d.Normalize();

		float du = d.Dot(u);
		if (fabs(du) > 0.999f)
		{
			u = Float3::Cross(d, r);
			r = Float3::Cross(u, d);
		}
		else
		{
			r = Float3::Cross(u, d);
			u = Float3::Cross(d, r);
		}

		r.Normalize();
		u.Normalize();

		Quat q;
		q.FromAxis(r, u, d);

		SetRotation(q);
	}

	void Node::SetVisible(bool visible)
	{
		mVisible = visible;
	}

	void Node::SetOpacity(float opacity)
	{
		mOpacity = opacity;

		ChangeTm(eTmFlags::UNKNOWN);
	}

	void Node::SetLighting(bool b)
	{
		mLighting = b;
	}

	void Node::SetCastShadow(bool b)
	{
		mCastShadow = b;
	}

	void Node::SetReceiveShadow(bool b)
	{
		mReceiveShadow = b;
	}

	bool Node::IsVisible() const
	{
		bool visible = mVisible;

		if (mParent)
			visible &= mParent->IsVisible();

		return visible;
	}

	void Node::SetTmFlags(int flags)
	{
		mTmFlags = flags;
	}

	void Node::ChangeTm(int flags)
	{
		mTmChangeFlags |= flags;

		for (int i = 0; i < mChildren.Size(); ++i)
			mChildren[i]->ChangeTm(flags);
	}

	void Node::Translate(const Float3 & vTrans, eTransform ts)
	{
		if (mTmFlags & eTmFlags::TRANSLATE)
		{
			Float3 position;

			switch (ts)
			{
			case eTransform::WORLD:
				if (mParent)
				{
					Quat q = mParent->GetWorldRotation();
					q.Inverse();

					Float3 t = vTrans * q;

					position = mPosition + t / mParent->GetWorldScale();
				}
				else
				{
					position = mPosition + vTrans;
				}
				break;

			case eTransform::PARENT:
				position = mPosition + vTrans;
				break;

			case eTransform::LOCAL:
				position = mPosition + vTrans * mRotation;
				break;
			}

			SetPosition(position);
		}
	}

	void Node::Rotate(const Quat & qn, eTransform ts)
	{
		if (mTmFlags & eTmFlags::ROTATE)
		{
			Quat rotation;

			switch (ts)
			{
			case eTransform::WORLD:
				if (mParent)
				{
					Quat qp, iqp;

					qp = mParent->GetWorldRotation();
					iqp = qp; iqp.Inverse();

					rotation = mRotation * iqp * qn * qp;
				}
				else
				{
					rotation = qn * mRotation;
				}
				break;

			case eTransform::PARENT:
				rotation = qn * mRotation;
				break;

			case eTransform::LOCAL:
				rotation = mRotation * qn;
				break;
			}

			SetRotation(rotation);
		}
	}

	void Node::Scale(const Float3 & v)
	{
		if (mTmFlags & eTmFlags::SCALE)
		{
			SetScale(mScale * v);
		}
	}

	void Node::Pitch(float x, eTransform ts)
	{
		Quat q;
		q.FromAxis(Float3(1, 0, 0), x);
		Rotate(q, ts);
	}

	void Node::Yaw(float x, eTransform ts)
	{
		Quat q;
		q.FromAxis(Float3(0, 1, 0), x);
		Rotate(q, ts);
	}

	void Node::Roll(float x, eTransform ts)
	{
		Quat q;
		q.FromAxis(Float3(0, 0, 1), x);
		Rotate(q, ts);
	}

	void Node::Reflect(const Plane & plane)
	{
		Float3 position = mPosition;
		Quat rotation = mRotation;

		Mat4 matReflection;
		matReflection.MakeReflection(plane);

		Float3 xaixs, yaxis, zaxis;
		rotation.ToAxis(xaixs, yaxis, zaxis);

		Float3 raxis = zaxis;
		raxis.TransformA(matReflection);

		Quat q;
		q.FromDir(zaxis, raxis, yaxis);

		position.TransformA(matReflection);
		rotation = q * rotation;

		SetPosition(position);
		SetRotation(rotation);
	}

	const Float3 & Node::GetWorldPosition()
	{
		if (mTmChangeFlags != 0)
			_updateTM();

		return mWorldPosition;
	}

	const Quat & Node::GetWorldRotation()
	{
		if (mTmChangeFlags != 0)
			_updateTM();

		return mWorldRotation;
	}

	const Float3 & Node::GetWorldScale()
	{
		if (mTmChangeFlags != 0)
			_updateTM();

		return mWorldScale;
	}

	void Node::SetWorldAabb(const Aabb & aabb)
	{
		mWorldAABB = aabb;
		mLocalAABBValid = false;
	}

	const Aabb & Node::GetWorldAabb()
	{
		if (mTmChangeFlags != 0)
			_updateTM();

		return mWorldAABB;
	}

	Aabb Node::GetBound(int depth)
	{
		if (mTmChangeFlags != 0)
			_updateTM();

		Aabb bound = mWorldAABB;

		if (depth > 0)
		{
			for (int i = 0; i < mChildren.Size(); ++i)
				bound.Merge(mChildren[i]->GetBound(depth - 1));
		}
		
		return bound;
	}

	const Mat4 & Node::GetWorldTM()
	{
		if (mTmChangeFlags != 0)
			_updateTM();

		return mWorldTM;
	}

	float Node::GetWorldOpacity()
	{
		if (mTmChangeFlags != 0)
			_updateTM();

		return mWorldOpacity;
	}

	void Node::SetLocalAabb(const Aabb & aabb)
	{
		mLocalAABB = aabb;
		mLocalAABBValid = true;

		ChangeTm(eTmFlags::UNKNOWN);
	}

	const Aabb & Node::GetLocalAabb()
	{
		return mLocalAABB;
	}

	void Node::SetInheritRotation(bool b)
	{
		mInheritRotation = b;

		ChangeTm(eTmFlags::ALL);
	}

	void Node::SetInheritScale(bool b)
	{
		mInheritScale = b;

		ChangeTm(eTmFlags::ALL);
	}

	void Node::_updateTM()
	{
		if (mTmChangeFlags == 0)
			return ;

		int flags = mTmChangeFlags;

		if (flags & (eTmFlags::TRANSLATE | eTmFlags::ROTATE | eTmFlags::SCALE))
		{
			if (mParent)
			{
				if (mParent->mTmChangeFlags != 0)
					mParent->_updateTM();

				const Quat & parentRotation = mParent->GetWorldRotation();
				if (mInheritRotation)
					mWorldRotation = parentRotation * mRotation;
				else
					mWorldRotation = mRotation;

				const Float3 & parentScale = mParent->GetWorldScale();
				if (mInheritScale)
					mWorldScale = mScale * parentScale;
				else
					mWorldScale = mScale;

				const Float3 & parentPosition = mParent->GetWorldPosition();
				mWorldPosition = (mPosition * parentScale) * parentRotation + parentPosition;
			}
			else
			{
				mWorldPosition = mPosition;
				mWorldRotation = mRotation;
				mWorldScale = mScale;
			}

			mWorldTM.MakeTransform(mWorldPosition, mWorldRotation, mWorldScale);
		}

		if (mLocalAABBValid)
		{
			mWorldAABB = mLocalAABB;
			mWorldAABB.Transform(mWorldTM);
		}

		if (mParent != NULL)
			mWorldOpacity = mOpacity * mParent->GetWorldOpacity();
		else
			mWorldOpacity = mOpacity;
		
		mTmChangeFlags = 0;

		if (mShape != NULL)
			mShape->OnChangeTm(flags);

		if (mZone != NULL && !mZone->Contain(this))
			mZone->RemoveNode(this);
	}

	void Node::Update(float elapsedTime)
	{
		if (mShape != NULL)
			mShape->OnUpdate(elapsedTime);

		UpdateComponent(elapsedTime);
		
		if (mTmChangeFlags != 0)
			_updateTM();

		if (mZone == NULL && mInWorld)
			World::Instance()->MainZone()->AddNode(this);
	}

	void Node::SetShape(Shape * shape)
	{
		safe_delete(mShape);

		mShape = shape;
		mShape->OnAttach(this);
	}
}