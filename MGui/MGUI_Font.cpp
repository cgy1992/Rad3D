#include "MGUI_Font.h"

namespace Rad { namespace MGUI {

	Font::Font(const FixedString32 & name)
		: mName(name)
		, mHeight(0)
	{
	}

	Font::~Font()
	{
	}

}}

