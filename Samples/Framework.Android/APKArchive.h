#pragma once

#include "MArchive.h"

namespace Rad {

	class APKArchive : public Archive
	{
	public:
		APKArchive(const String & floderName);
		virtual ~APKArchive();

		virtual DataStreamPtr 
			Open(const String & filename);
		virtual bool
			Exist(const String & filename);

	};
}
