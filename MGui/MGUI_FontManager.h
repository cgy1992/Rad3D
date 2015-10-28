/*
*	Font Manager
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Types.h"
#include "MGUI_Font.h"

namespace Rad { namespace MGUI {

	class MGUI_ENTRY FontManager : public Singleton<FontManager>
	{
	public:
		FontManager();
		~FontManager();

		void 
			Load(const String & _source);

		void 
			AddFactory(FontFactory * factoy);
		FontFactory * 
			GetFactory(const String & type);

		int 
			GetFontCount();
		Font * 
			GetFont(int _index);
		Font * 
			GetFont(const FixedString32 & _name);
		
		void 
			SetDefaultFont(const FixedString32 & _name);
		Font * 
			GetDefaultFont();

	protected:
		Font * mDefaultFont;
		Array<Font*> mFonts;
		Array<FontFactory *> mFontFactorys;
	};

}}
