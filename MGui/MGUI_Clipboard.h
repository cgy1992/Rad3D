/*
*	Clipboard
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Types.h"

namespace Rad { namespace MGUI {

	class MGUI_ENTRY Clipboard : public Singleton<Clipboard>
	{
	public:
		Clipboard();
		~Clipboard();

		void 
			SetText(const UString & _str);
		UString 
			GetText();

	protected:
		UString mText;
	};

}}
