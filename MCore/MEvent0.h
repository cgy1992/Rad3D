/*
*	Event0
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MEvent.h"

namespace Rad {

	class _tListener0 : public _tListener
	{
	public:
		virtual ~_tListener0() {}

		virtual void OnCall() = 0;
	};

	class tEvent0 : public _tEvent
	{
		typedef _tListener0 listener_t;

	public:
		void operator +=(listener_t * _listener)
		{
			Attach(_listener);
		}

		void operator -=(listener_t * _listener)
		{
			Detach(_listener);
		}

		void operator ()()
		{
			_tListener * node = mHead;
			_tListener * next = NULL;

			while (node != NULL)
			{
				next = node->_Next;

				static_cast<listener_t *>(node)->OnCall();

				node = next;
			}
		}
	};

	template <class T>
	class cListener0 : public _tListener0
	{
		typedef void (T::*Function)();

		T * _This;
		Function _Fn;

	public:
		cListener0() : _This(NULL), _Fn(NULL) {}
		cListener0(T * _listener, Function _func)  : _This(_listener), _Fn(_func){}
		virtual ~cListener0() {}

		virtual void OnCall()
		{
			d_assert (_This != NULL && _Fn != NULL);

			(_This->*_Fn)();
		}

		cListener0 * operator()(T * _listener, Function _func)
		{
			_This = _listener;
			_Fn = _func;

			return this;
		}
	};

	class ncListener0 : public _tListener0
	{
		typedef void (*Function)();

		Function _Fn;

	public:
		ncListener0(Function _func) : _Fn(_func) {}
		virtual ~ncListener0() {}

		virtual void OnCall()
		{
			d_assert (_Fn != NULL);

			_Fn();
		}
	};
}
