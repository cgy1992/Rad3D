/*
*	MGUI Node
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Types.h"

namespace Rad { namespace MGUI {

	class MGUI_ENTRY Node : public ComponentOwner
	{
	public:
		enum eUpdateFlags {
			UPDATE_RECT = 1,
			UPDATE_COLOR = 2,
			UPDATE_ORDER = 4,

			UPDATE_ALL = 0x7FFFF
		};

	public:
		Node(Node * parent);
		virtual ~Node();

		void 
			SetName(const FixedString32 & _name);
		const FixedString32 & 
			GetName() { return mName; }

		void 
			SetUserData(void * userData) { mUserData = userData; }
		void * 
			GetUserData() { return mUserData; }

		void 
			SetAlign(eAlign align);
		eAlign 
			GetAlign() { return mAlign; }

		void 
			SetRect(const Rect & rect);
		void 
			SetRect(float x, float y, float w, float h);
		const Rect & 
			GetRect();

		void
			Move(float x, float y);

		void 
			SetRectPosition(float x, float y);
		void 
			GetRectPosition(float & x, float & y);

		void 
			SetClientRect(const ClientRect & clientRect);
		const ClientRect & 
			GetClientRect() { return mClientRect; }

		const Rect & 
			GetClipRect() { return mClipRect; }

		void 
			SetColor(const Float4 & color);
		const Float4 & 
			GetColor() { return mColor; }

		void 
			SetAlpha(float a);
		float 
			GetAlpha() { return mColor.a; }

		void 
			SetOrder(int order);
		int 
			GetOrder() { return mOrder; }

#if 0
		// not support current
		void
			SetRotation(float rotation);
		float
			GetRotation() { return mRotation; }
#endif

		void
			SetScale(float scale);
		float
			GetScale() { return mScale; }
	
		const Rect & 
			GetClient();
		const Rect & 
			GetAbsRect();
		const Rect & 
			GetAbsClient();
		const Float4 & 
			GetAbsColor();
		int 
			GetAbsOrder();
		float
			GetAbsRotation();
		float
			GetAbsScale();

		void 
			SetInheritAlpha(bool b) { mInheritAlpha = b; }
		bool 
			IsInheritAlpha() { return mInheritAlpha; }

		void 
			SetInheritColor(bool b) { mInheritColor = b; }
		bool 
			IsInheritColor() { return  mInheritColor; }

		void 
			SetInheritColorEx(bool b, bool child);

		void 
			SetVisible(bool visible);
		bool 
			IsVisible() const { return mVisible; }

		void
			SetDockable(bool b) { mDockable = b; }
		bool
			IsDockable() { return mDockable; }

		void 
			_needUpdate(int flags);
		void 
			_updateTM();

		void 
			_notifyAttach(Node * _child);
		void 
			_notifyDetach(Node * _child);

		void 
			_sortChild();

	protected:
		void 
			_updateAlign();
		
	protected:
		Node * mParent;
		Array<Node *> mChildren;

		FixedString32 mName;
		void * mUserData;

		eAlign mAlign;
		Rect mRect;
		Rect mClient;
		ClientRect mClientRect;
		Rect mClipRect;
		Float4 mColor;
		int mOrder;
		float mRotation;
		float mScale;

		Rect mAbsRect;
		Rect mAbsClient;
		Float4 mAbsColor;
		int mAbsOrder;
		float mAbsRotation;
		float mAbsScale;

		bool mVisible;
		bool mDockable;
		bool mInheritColor;
		bool mInheritAlpha;
		
		int mUpdateFlags;
	};
	
}}
