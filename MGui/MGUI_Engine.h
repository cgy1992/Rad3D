/*
*	MGUI Engine
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Types.h"
#include "MGUI_Locale.h"
#include "MGUI_LookFeel.h"
#include "MGUI_Helper.h"
#include "MGUI_Layout.h"
#include "MGUI_Clipboard.h"
#include "MGUI_FontManager.h"
#include "MGUI_InputManager.h"

namespace Rad { namespace MGUI {

	class MGUI_ENTRY Engine : public Singleton<Engine>
	{
	public:
		tEvent2<int, int> E_Resize;

	public:
		Engine();
		~Engine();

		void 
			OnResize(int w, int h);
		void 
			Update(float elapsedTime);
		void 
			Render();

		const Float2 & 
			GetInvSize() { return mInvSize; }
		const Rect & 
			GetRect() { return mRootLayout->GetRect(); }
		Widget * 
			GetRootLayout() { return mRootLayout; } 
		Layout * 
			GetTopLayout() { return mTopLayout; }

		Widget * 
			GetWidget(float _x, float _y) { return mRootLayout->Pick(_x, _y); }

	protected:
		Float2 mInvSize;
		Widget * mRootLayout;
		Layout * mTopLayout;

		Helper * mHelper;
		Locale * mLocale;
		LookFeelManager * mLookFeelManager;
		Clipboard * mClipboard;
		FontManager * mFontManager;
		InputManager * mInputManager;

		ShaderFX * mShaderFX;
	};

}}
