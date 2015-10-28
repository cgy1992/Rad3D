#include "MRef.h"
#include "MThread.h"

namespace Rad {

	Mutex _ref_mutex;

#define _ref_lock() _ref_mutex.Lock()
#define _ref_unlock() _ref_mutex.Unlock()

	int RefMT::_refinc()
	{
		_ref_lock();

		++i_ref_count;

		_ref_unlock();

		return i_ref_count;
	}

	int RefMT::_refdec()
	{
		d_assert(i_ref_count > 0);

		_ref_lock();

		--i_ref_count;

		_ref_unlock();

		return i_ref_count;
	}

}

