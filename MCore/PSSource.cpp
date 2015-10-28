#include "PSSource.h"
#include "PSManager.h"

namespace Rad {

	PS_Source::PS_Source(const String & name, const String & sourceName, bool _internal)
		: Resource(name, sourceName)
		, mInternal(_internal)
	{
	}

	PS_Source::~PS_Source()
	{
		OnUnload();
	}

	void PS_Source::Release()
	{
		if (!mInternal)
		{
			PS_Manager::Instance()->_delSource(this);
		}
		else
		{
			delete this;
		}
	}

	void PS_Source::OnLoad()
	{
		Resource::OnLoad();
	}

	void PS_Source::OnUnload()
	{
		mStream = NULL;
		Resource::OnUnload();
	}
}
