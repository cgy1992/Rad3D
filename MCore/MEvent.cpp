#include "MEvent.h"

namespace Rad {

	_tListener::~_tListener()
	{ 
		if (_Event) _Event->Detach(this);
	}
}