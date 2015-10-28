#include "MGUI_Node.h"
#include "MGUI_Engine.h"

namespace Rad { namespace MGUI {

	Node::Node(Node * parent)
		: mParent(NULL)
		, mColor(1, 1, 1, 1)
		, mAbsColor(1, 1, 1, 1)
		, mOrder(0)
		, mAbsOrder(0)
		, mRotation(0)
		, mAbsRotation(0)
		, mScale(1)
		, mAbsScale(1)
		, mUserData(NULL)
		, mVisible(true)
		, mDockable(false)
		, mInheritAlpha(true)
		, mInheritColor(false)
		, mAlign(eAlign::NONE)
		, mUpdateFlags(UPDATE_ALL)
	{
		if (parent)
			parent->_notifyAttach(this);
	}

	Node::~Node()
	{
		if (mParent)
			mParent->_notifyDetach(this);

		while (mChildren.Size() > 0)
			delete mChildren[mChildren.Size() - 1];
	}

	void Node::SetName(const FixedString32 & _name)
	{
		mName = _name;
	}

	void Node::SetAlign(eAlign align)
	{
		if (mAlign != align)
		{
			mAlign = align;
			_needUpdate(UPDATE_RECT);
		}
	}

	void Node::SetColor(const Float4 & color)
	{
		if (mColor != color)
		{
			mColor = color;
			_needUpdate(UPDATE_COLOR);
		}
	}

	void Node::SetAlpha(float a)
	{
		if (mColor.a != a)
		{
			mColor.a = a;
			_needUpdate(UPDATE_COLOR);
		}
	}

	void Node::SetOrder(int order)
	{
		if (mOrder != order)
		{
			mOrder = order;

			if (mParent)
				mParent->_sortChild();

			_needUpdate(UPDATE_ORDER);
		}
	}

#if 0
	void Node::SetRotation(float rotation)
	{
		if (mRotation != rotation)
		{
			mRotation = rotation;
			_needUpdate(UPDATE_RECT);
		}
	}
#endif

	void Node::SetScale(float scale)
	{
		if (mScale != scale)
		{
			mScale = scale;
			_needUpdate(UPDATE_RECT);
		}
	}

	void Node::SetRect(const Rect & rect)
	{
		if (mRect != rect)
		{
			mRect = rect;
			_needUpdate(UPDATE_RECT);
		}
	}

	void Node::SetRect(float x, float y, float w, float h)
	{
		SetRect(Rect(x, y, w, h));
	}

	const Rect & Node::GetRect()
	{
		_updateTM();
		return mRect;
	}

	void Node::Move(float x, float y)
	{
		mRect.x += x, mRect.y += y;
		_needUpdate(UPDATE_RECT);
	}

	void Node::SetRectPosition(float x, float y)
	{
		if (mRect.x != x || mRect.y != y)
		{
			mRect.x = x; mRect.y = y;
			_needUpdate(UPDATE_RECT);
		}
	}

	void Node::GetRectPosition(float & x, float & y)
	{
		_updateTM();
		x = mRect.x; y = mRect.y;
	}

	void Node::SetClientRect(const ClientRect & _rect)
	{
		mClientRect = _rect;
		_needUpdate(UPDATE_RECT);
	}

	const Rect & Node::GetClient()
	{
		_updateTM();
		return mClient;
	}

	const Rect & Node::GetAbsRect()
	{
		_updateTM();
		return mAbsRect;
	}

	const Rect & Node::GetAbsClient()
	{
		_updateTM();
		return mAbsClient;
	}

	const Float4 & Node::GetAbsColor()
	{
		_updateTM();
		return mAbsColor;
	}

	int Node::GetAbsOrder()
	{
		_updateTM();
		return mAbsOrder;
	}

	float Node::GetAbsScale()
	{
		_updateTM();
		return mAbsScale;
	}

	void Node::_notifyAttach(Node * _child)
	{
		d_assert (_child->mParent == NULL);

		int index = 0;
		for (int i = mChildren.Size(); i > 0; --i)
		{
			if (_child->GetOrder() >= mChildren[i - 1]->GetOrder())
			{
				index = i;
				break;
			}
		}

		mChildren.Insert(index, _child);
		_child->mParent = this;
	}

	void Node::_notifyDetach(Node * _child)
	{
		d_assert (_child->mParent != NULL);

		if (_child->IsDockable())
			_needUpdate(UPDATE_RECT);

		int index = -1;
		for (int i = 0; i < mChildren.Size(); ++i)
		{
			if (mChildren[i] == _child)
			{
				index = i;
				break;
			}
		}

		d_assert (index != -1);

		mChildren.Erase(index);
		_child->mParent = NULL;
	}

	struct MGUI_ENTRY NodeSorter
	{
		int operator()(Node * a, Node * b)
		{
			int r1 = a->GetOrder(), r2 = b->GetOrder();

			return r1 < r2 ? -1 : (r1 > r2 ? 1 : 0);
		}
	};

	void Node::_sortChild()
	{
		NodeSorter sorter;

		Sort(&mChildren[0], mChildren.Size(), sorter);

		for (int i = 0; i < mChildren.Size(); ++i)
		{
			mChildren[i]->_needUpdate(UPDATE_RECT);
		}
	}

	void Node::_needUpdate(int flags)
	{
		if ((mUpdateFlags & flags) != flags)
		{
			mUpdateFlags |= flags;

			for (int i = 0; i < mChildren.Size(); ++i)
				mChildren[i]->_needUpdate(flags);
		}
	}

	void Node::_updateAlign()
	{
		if (mParent == NULL)
			return ;

		Rect client = mParent->mClient;
		client.x = 0, client.y = 0;
		
		if (mDockable)
		{
			int i = 0;
			float x0 = 0, y0 = 0;
			float x1 = client.w, y1 = client.h;

			for (i = 0; i < mParent->mChildren.Size(); ++i)
			{
				if (mParent->mChildren[i] == this)
					break;

				if (mParent->mChildren[i]->IsDockable() && mParent->mChildren[i]->IsVisible())
				{
					Node * _child = mParent->mChildren[i];

					if (_child->mAlign._value & eAlign::LEFT)
					{
						x0 = Max(x0, _child->mRect.x + _child->mRect.w);
					}
					else if (_child->mAlign._value & eAlign::RIGHT)
					{
						x1 = Min(x1, _child->mRect.x);
					}

					if (_child->mAlign._value & eAlign::TOP)
					{
						y0 = Max(y0, _child->mRect.y + _child->mRect.h);
					}
					else if (_child->mAlign._value & eAlign::RIGHT)
					{
						y1 = Min(y1, _child->mRect.y);
					}
				}
			}

			client.x = x0;
			client.y = y0;
			client.w = x1 - x0;
			client.h = y1 - y0;

			for (i = i + 1; i < mParent->mChildren.Size(); ++i)
			{
				if (mParent->mChildren[i]->IsDockable())
				{
					mParent->mChildren[i]->_needUpdate(UPDATE_RECT);
				}
			}
		}

		if (mAlign._value & eAlign::LEFT)
		{
			mRect.x = client.x;
		}
		else if (mAlign._value & eAlign::RIGHT)
		{
			mRect.x = client.w - mRect.w;
		}

		if (mAlign._value & eAlign::TOP)
		{
			mRect.y = client.y;
		}
		else if (mAlign._value & eAlign::BOTTOM)
		{
			mRect.y = client.h - mRect.h;
		}

		if (mAlign._value & eAlign::H_CENTER)
		{
			mRect.x = (client.w - mRect.w) / 2;
		}

		if (mAlign._value & eAlign::V_CENTER)
		{
			mRect.y = (client.h - mRect.h) / 2;
		}

		if (mAlign._value & eAlign::H_STRETCH)
		{
			mRect.x = client.x;
			mRect.w = client.w;
		}

		if (mAlign._value & eAlign::V_STRETCH)
		{
			mRect.y = client.y;
			mRect.h = client.h;
		}
	}

	void Node::_updateTM()
	{
		if (mUpdateFlags == 0)
			return ;

		if (mParent)
			mParent->_updateTM();

		if (mUpdateFlags & UPDATE_RECT)
		{
			_updateAlign();

			if (mParent != NULL)
			{
				mAbsRect.x = mParent->mAbsClient.x + mRect.x;
				mAbsRect.y = mParent->mAbsClient.y + mRect.y;
				mAbsRect.w = mRect.w;
				mAbsRect.h = mRect.h;

				mAbsRotation = mParent->mAbsRotation + mRotation;
				mAbsScale = mParent->mAbsScale * mScale;
			}
			else
			{
				mAbsRect = mRect;
				mAbsRotation = mRotation;
				mAbsScale = mScale;
			}

			if (mAbsScale != 1)
			{
				float ox = mAbsRect.x + mAbsRect.w / 2;
				float oy = mAbsRect.y + mAbsRect.h / 2;

				Helper::Instance()->GetAlignPoint(ox, oy, mAbsRect, eAlign::CENTER);

				float x0 = mAbsRect.x;
				float y0 = mAbsRect.y;
				float x1 = mAbsRect.x + mAbsRect.w;
				float y1 = mAbsRect.y + mAbsRect.h;

				x0 -= ox; y0 -= oy;
				x1 -= ox; y1 -= oy;

				x0 *= mAbsScale; y0 *= mAbsScale;
				x1 *= mAbsScale; y1 *= mAbsScale;

				x0 += ox; y0 += oy;
				x1 += ox; y1 += oy;

				mAbsRect.x = x0;
				mAbsRect.y = y0;
				mAbsRect.w = x1 - x0; 
				mAbsRect.h = y1 - y0;
			}

			float cx0 = mClientRect.dx0 * mAbsScale;
			float cy0 = mClientRect.dy0 * mAbsScale;
			float cx1 = mClientRect.dx1 * mAbsScale;
			float cy1 = mClientRect.dy1 * mAbsScale;

			mClient.x = mRect.x + cx0;
			mClient.y = mRect.y + cy0;
			mClient.w = mRect.w - cx0 - cx1;
			mClient.h = mRect.h - cy0 - cy1;

			mAbsClient.x = mAbsRect.x + cx0;
			mAbsClient.y = mAbsRect.y + cy0;
			mAbsClient.w = mAbsRect.w - cx0 - cx1;
			mAbsClient.h = mAbsRect.h - cy0 - cy1;
			
			if (mParent != NULL)
			{
				const Rect & clipRect = mParent->GetClipRect();
				float x0 = Max(mAbsClient.x, clipRect.x);
				float y0 = Max(mAbsClient.y, clipRect.y);
				float x1 = Min(mAbsClient.x + mAbsClient.w, clipRect.x + clipRect.w);
				float y1 = Min(mAbsClient.y + mAbsClient.h, clipRect.y + clipRect.h);

				mClipRect.x = x0; mClipRect.y = y0;
				mClipRect.w = x1 - x0; mClipRect.h = y1 - y0;
			}
			else
			{
				mClipRect = mAbsClient;
			}
		}

		if (mUpdateFlags & UPDATE_COLOR)
		{
			if (mParent != NULL)
			{
				if (mInheritColor)
					mAbsColor = mParent->mAbsColor * mColor;
				else
					mAbsColor = mColor;

				if (mInheritAlpha)
					mAbsColor.a = mParent->mAbsColor.a * mColor.a;
				else
					mAbsColor.a = mColor.a;
			}
			else
			{
				mAbsColor = mColor;
			}
		}

		if (mUpdateFlags & UPDATE_ORDER)
		{
			if (mParent)
			{
				mAbsOrder = mParent->mAbsOrder + mOrder;
			}
			else
			{
				mAbsOrder = mOrder;
			}

			if (mAbsOrder < 0)
			{
				int i = 0;
			}
		}

		mUpdateFlags = 0;
	}

	void Node::SetInheritColorEx(bool b, bool child)
	{
		SetInheritColor(b);

		if (!child)
			return ;

		for (int i = 0; i < mChildren.Size(); ++i)
		{
			Node * node = mChildren[i];
			node->SetInheritColorEx(b, child);
		}
	}
	
	void Node::SetVisible(bool visible)
	{
		if (mVisible != visible)
		{
			_needUpdate(UPDATE_RECT);

			mVisible = visible;
		}
	}

}}