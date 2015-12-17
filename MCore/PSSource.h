/*
*	PSSource
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MResource.h"

namespace Rad {

	class M_ENTRY PS_Source : public Resource
	{
	public:
		PS_Source(const String & name, const String & sourceName, bool _internal = false);
		virtual ~PS_Source();

		virtual void 
			Release();

		virtual void 
			OnLoad();
		virtual void 
			OnUnload();
		virtual void 
			_load(DataStreamPtr stream) { SetStream(stream); }

		void
			SetStream(DataStreamPtr stream) { mStream = stream; }
		DataStreamPtr 
			GetStream() { return mStream; }

	protected:
		DataStreamPtr mStream;
		bool mInternal;
	};

	typedef SmartPtr<PS_Source> PS_SourcePtr;

}