/*
*	Event4 
*		4 parameter event & listener
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MEvent.h"

namespace Rad {

	template<class P1, class P2, class P3, class P4>
	class _tListener4 : public _tListener
	{
	public:
		virtual ~_tListener4() {}

		virtual void OnCall(P1 p1, P2 p2, P3 p3, P4 p4) = 0;
	};

	template<class P1, class P2, class P3, class P4>
	class tEvent4 : public _tEvent
	{
	public:
		void operator +=(_tListener4<P1, P2, P3, P4> * _listener)
		{
			Attach(_listener);
		}

		void operator -=(_tListener4<P1, P2, P3, P4> * _listener)
		{
			Detach(_listener);
		}

		void operator ()(P1 p1, P2 p2, P3 p3, P4 p4)
		{
			_tListener * node = mHead;
			_tListener * next = NULL;

			while (node != NULL)
			{
				next = node->_Next;

				static_cast<_tListener4<P1, P2, P3, P4> *>(node)->OnCall(p1, p2, p3, p4);

				node = next;
			}
		}
	};

	template <class T, class P1, class P2, class P3, class P4>
	class cListener4 : public _tListener4<P1, P2, P3, P4>
	{
		typedef void (T::*Function)(P1 p1, P2 p2, P3 p3, P4 p4);

		T * _This;
		Function _Fn;

	public:
		cListener4() : _This(NULL), _Fn(NULL) {}
		cListener4(T * _listener, Function _func) : _This(_listener), _Fn(_func) {}
		virtual ~cListener4() {}

		virtual void OnCall(P1 p1, P2 p2, P3 p3, P4 p4)
		{
			d_assert (_This != NULL && _Fn != NULL);

			(_This->*_Fn)(p1, p2, p3, p4);
		}

		cListener4 * operator()(T * _listener, Function _func)
		{
			_This = _listener;
			_Fn = _func;

			return this;
		}
	};

	template<class P1, class P2, class P3, class P4>
	class ncListener4 : public _tListener4<P1, P2, P3, P4>
	{
		typedef void (*Function)(P1 p1, P2 p2, P3 p3, P4 p4);

		Function _Fn;

	public:
		ncListener4(Function _func) : _Fn(_func) {}
		virtual ~ncListener4() {}

		virtual void OnCall(P1 p1, P2 p2, P3 p3, P4 p4)
		{
			d_assert (_Fn != NULL);

			_Fn(p1, p2, p3, p4);
		}
	};

}
