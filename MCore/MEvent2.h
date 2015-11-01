/*
*	Event2 
*		2 parameter event & listener
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MEvent.h"

namespace Rad {

	template<class P1, class P2>
	class _tListener2 : public _tListener
	{
	public:
		virtual ~_tListener2() {}

		virtual void OnCall(P1 p1, P2 p2) = 0;
	};

	template<class P1, class P2>
	class tEvent2 : public _tEvent
	{
		typedef _tListener2<P1, P2> listener_t;

	public:
		void operator +=(listener_t * _listener)
		{
			Attach(_listener);
		}

		void operator -=(listener_t * _listener)
		{
			Detach(_listener);
		}

		void operator ()(P1 p1, P2 p2)
		{
			_tListener * node = mHead;
			_tListener * next = NULL;

			while (node != NULL)
			{
				next = node->_Next;

				static_cast<listener_t *>(node)->OnCall(p1, p2);

				node = next;
			}
		}
	};

	template <class T, class P1, class P2>
	class cListener2 : public _tListener2<P1, P2>
	{
		typedef void (T::*Function)(P1 p1, P2 p2);

		T * _This;
		Function _Fn;

	public:
		cListener2() : _This(NULL), _Fn(NULL) {}
		cListener2(T * _listener, Function _func)  : _This(_listener), _Fn(_func) {}
		virtual ~cListener2() {}

		virtual void OnCall(P1 p1, P2 p2)
		{
			d_assert (_This != NULL && _Fn != NULL);

			(_This->*_Fn)(p1, p2);
		}

		cListener2 * operator()(T * _listener, Function _func)
		{
			_This = _listener;
			_Fn = _func;

			return this;
		}
	};

	template<class P1, class P2>
	class ncListener2 : public _tListener2<P1, P2>
	{
		typedef void (*Function)(P1 p1, P2 p2);

		Function _Fn;

	public:
		ncListener2(Function _func) : _Fn(_func) {}
		virtual ~ncListener2() {}

		virtual void OnCall(P1 p1, P2 p2)
		{
			d_assert (_Fn != NULL);

			_Fn(p1, p2);
		}
	};

}
