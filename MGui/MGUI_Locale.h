/*
*	MGUI Locale
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Types.h"

namespace Rad { namespace MGUI {

	class MGUI_ENTRY Locale : public Singleton<Locale>
	{
	public:
		Locale();
		~Locale();

		void 
			Load(const String & filename);
		void
			Unload();

		const char *
			GetString(const String & key);

		void
			Insert(const String & key, const String & str);
		void
			Remove(const String & key);

	protected:
		Map<String, String> mStringMap;
	};

}}