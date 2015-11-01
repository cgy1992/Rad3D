/*
*	Event2 
*		2 parameter event & listener
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MMemory.h"

namespace Rad {

	template<class P1, class P2>
	class _tListener2 : public _tListener
	{
	public:
		_tListener2(void * thiz) : _tListener(thiz) {}
		virtual ~_tListener2() {}

		virtual void OnCall(P1 p1, P2 p2) = 0;
	};

	template<class P1, class P2>
	class tEvent2 : public _tEvent<_tListener2<P1, P2> >
	{
	public:
		void operator ()(P1 p1, P2 p2)
		{
			_tListener * node = mHead;
			_tListener * next = NULL;

			while (node != NULL)
			{
				next = node->_Next;

				static_cast<_tListener2<P1, P2> *>(node)->OnCall(p1, p2);

				node = next;
			}
		}
	};

	template <class T, class P1, class P2>
	class cListener2 : public _tListener2<P1, P2>
	{
		typedef void (T::*Function)(P1 p1, P2 p2);

		Function _Fn;

	public:
		cListener2() : _tListener2(NULL), _Fn(NULL) {}
		cListener2(T * _listener, Function _func)  : _tListener2(_listener), _Fn(_func) {}
		virtual ~cListener2() {}

		virtual void OnCall(P1 p1, P2 p2)
		{
			d_assert (_This != NULL && _Fn != NULL);

			T * thiz = (T *)_This;

			(thiz->*_Fn)(p1, p2);
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
		ncListener2(Function _func) : _tListener2(NULL), _Fn(_func) {}
		virtual ~ncListener2() {}

		virtual void OnCall(P1 p1, P2 p2)
		{
			d_assert (_Fn != NULL);

			_Fn(p1, p2);
		}
	};

}
