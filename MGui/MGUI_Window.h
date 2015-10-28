/*
*	Window
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Layout.h"

namespace Rad { namespace MGUI {

	class MGUI_ENTRY Window
	{
	public:
		Window(const String & filename, Widget * parent);
		virtual ~Window();

		Widget *
			GetParent() { return mParent; }
		Layout *
			GetLayout() { return mLayout; }

		void
			Load();
		void
			Unload();

		virtual void
			OnLoaded() {}
		virtual void
			OnUnload() {}

	protected:
		void
			OnResourceLoaded();
		void
			OnResourceUnload();

	protected:
		Widget * mParent;
		ResourceDataStreamPtr mSource;
		Layout * mLayout;
	};

}}
