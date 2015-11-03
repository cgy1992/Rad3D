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
			Save(const String & filename);
		void
			Unload();

		const char *
			GetString(const String & key);

		void
			Insert(const String & key, const String & str);
		void
			Remove(const String & key);

	protected:
		bool mEnable;
		bool mNeedSave;
		Map<String, String> mStringMap;
	};

	inline String & _L_TR(String & str, bool translation)
	{
		if (translation)
		{
			const char * tr = Locale::Instance()->GetString(str);
			if (tr != NULL)
			{
				str = tr;
			}
			else
			{
				Locale::Instance()->Insert(str, "");
			}
		}

		return str;
	}

#ifndef NO_LOCALE
	#define L_TR(s, b) _L_TR(s, b)
#else
	#define L_TR(s, b)
#endif

}}