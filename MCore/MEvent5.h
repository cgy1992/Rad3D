/*
*	Event5 
*		5 parameter event & listener
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MEvent.h"

namespace Rad {

	template<class P1, class P2, class P3, class P4, class P5>
	class _tListener5 : public _tListener
	{
	public:
		virtual ~_tListener5() {}

		virtual void OnCall(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) = 0;
	};

	template<class P1, class P2, class P3, class P4, class P5>
	class tEvent5 : public _tEvent
	{
		typedef _tListener5<P1, P2, P3, P4, P5> listener_t;

	public:
		void operator +=(listener_t * _listener)
		{
			Attach(_listener);
		}

		void operator -=(listener_t * _listener)
		{
			Detach(_listener);
		}

		void operator ()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
		{
			_tListener * node = mHead;
			_tListener * next = NULL;

			while (node != NULL)
			{
				next = node->_Next;

				static_cast<listener_t *>(node)->OnCall(p1, p2, p3, p4, p5);

				node = next;
			}
		}
	};

	template <class T, class P1, class P2, class P3, class P4, class P5>
	class cListener5 : public _tListener5<P1, P2, P3, P4, P5>
	{
		typedef void (T::*Function)(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5);

		T * _This;
		Function _Fn;

	public:
		cListener5() : _This(NULL), _Fn(NULL) {}
		cListener5(T * _listener, Function _func) : _This(_listener), _Fn(_func) {}
		virtual ~cListener5() {}

		virtual void OnCall(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
		{
			d_assert (_This != NULL && _Fn != NULL);

			(_This->*_Fn)(p1, p2, p3, p4, p5);
		}

		cListener5 * operator()(T * _listener, Function _func)
		{
			_This = _listener;
			_Fn = _func;

			return this;
		}
	};

	template<class P1, class P2, class P3, class P4, class P5>
	class ncListener5 : public _tListener5<P1, P2, P3, P4, P5>
	{
		typedef void (*Function)(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5);

		Function _Fn;

	public:
		ncListener5(Function _func) : _Fn(_func) {}
		virtual ~ncListener5() {}

		virtual void OnCall(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
		{
			d_assert (_Fn != NULL);

			_Fn(p1, p2, p3, p4, p5);
		}
	};

}
