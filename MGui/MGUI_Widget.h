/*
*	MGUI Widget
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Event.h"
#include "MGUI_Node.h"
#include "MGUI_LookFeel.h"
#include "MGUI_WidgetInput.h"

namespace Rad { namespace MGUI {

	class Layout;
		 
	class MGUI_ENTRY Widget : public Node, public WidgetInput
	{
		DECLARE_RTTI();

	public:
		Widget(const LookFeel * _lookfeel, Widget * _parent);
		virtual ~Widget();

		virtual void 
			UpdateRenderItem(Layout * _layout);

		virtual void
			Update(float elapsedTime);
		virtual void 
			OnUpdate(float elapsedTime) {}
		virtual void 
			OnUpdateWidgetState();

		virtual Widget * 
			Pick(float _x, float _y);

		Widget * 
			GetParent() { return (Widget *)mParent; }
		const LookFeel * 
			GetLookFeel() { return mLookFeel; }
		TexturePtr
			GetSkin() { return mSkin; }

		void
			SetInheritState(bool inherit) { mInheritState = inherit; }
		bool
			IsInheritState() const { return mInheritState; }
		int 
			GetState() const { return mState; }

		void 
			SetEnable(bool enable) { mEnable = enable; }
		bool 
			IsEnable() const { return mEnable; }

		void 
			SetPickFlag(int flag) { mPickFlag = flag; }
		int 
			GetPickFlag() const { return mPickFlag; }
		void
			SetPickMaskEnable(bool enable) { mPickMaskEnable = enable; }
		bool
			IsPickMaskEnable() const { return mPickMaskEnable; }

		Widget * 
			GetChild(const FixedString32 & _name);
		Widget * 
			GetChild(int _index);
		template <class T> T * 
			GetChildT(const FixedString32 & _name);
		int 
			GetChildCount();

		virtual bool 
			CanKeyFoucsed() { return false; }

		virtual void
			SerializeXML(xml_node root, bool is_out);

	protected:
		virtual bool 
			_LookupPickMask(float _x, float y);;

	protected:
		const LookFeel * mLookFeel;
		TexturePtr mSkin;

		bool mEnable;
		
		bool mPickMaskEnable;
		int mPickFlag;

		bool mInheritState;
		int mState;
	};

	template <class T>
	T * Widget::GetChildT(const FixedString32 & _name)
	{
		Widget * widget = GetChild(_name);

		d_assert (KIND_OF(T, widget));

		return (T *)widget;
	}

}}
