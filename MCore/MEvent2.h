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

	template <class T, class P1, class P2>
	class _tListener2
	{
		DECLARE_POOL_ALLOC();

	public:
		T * _Event;
		_tListener2 * _Prev;
		_tListener2 * _Next;

	public:
		_tListener2() : _Event(NULL), _Prev(NULL), _Next(NULL) {}
		virtual ~_tListener2() { if (_Event) (*_Event) -= this; }

		virtual void OnCall(P1 _p1, P2 _p2) = 0;
	};

	template <class P1, class P2>
	class tEvent2
	{
	public:
		typedef _tListener2<tEvent2, P1, P2> tMyListener;

	public:
		tEvent2() { mHead = NULL; }
		~tEvent2() { DetachAll(); }

		void DetachAll()
		{
			while (mHead)
				delete mHead;
		}

		void operator +=(tMyListener * _listener)
		{
			d_assert (_listener != NULL && _listener->_Event == NULL);
			d_assert (_listener->_Prev == NULL && _listener->_Next == NULL);

			if (mHead)
				mHead->_Prev = _listener;

			_listener->_Next = mHead;
			mHead = _listener;
			mHead->_Prev = NULL;

			_listener->_Event = this;
		}

		void operator -=(tMyListener * _listener)
		{
			d_assert (_listener != NULL && _listener->_Event == this);
			d_assert (mHead != NULL);

			if (_listener == mHead)
				mHead = _listener->_Next;

			if (_listener->_Prev)
				_listener->_Prev->_Next = _listener->_Next;

			if (_listener->_Next)
				_listener->_Next->_Prev = _listener->_Prev;

			_listener->_Prev = _listener->_Next = NULL;
			_listener->_Event = NULL;
		}

		void operator ()(P1 _p1, P2 _p2)
		{
			tMyListener * node = mHead;
			tMyListener * next = NULL;

			while (node != NULL)
			{
				next = node->_Next;

				node->OnCall(_p1, _p2);

				node = next;
			}
		}

	protected:
		tMyListener * mHead;
	};


	template <class T, class P1, class P2>
	class cListener2 : public _tListener2<tEvent2<P1, P2>, P1, P2>
	{
		typedef void (T::*Function)(P1 _param1, P2 _param2);

	public:
		cListener2() : mListener(NULL), mFunction(NULL) {}
		cListener2(T * _listener, Function _func) : mListener(_listener), mFunction(_func) {}
		virtual ~cListener2() {}

		virtual void OnCall(P1 _p1, P2 _p2)
		{
			d_assert (mListener != NULL && mFunction != NULL);

			(mListener->*mFunction)(_p1, _p2);
		}

		cListener2 * operator ()(T * _listener, Function _func)
		{
			mListener = _listener;
			mFunction = _func;

			return this;
		}

	protected:
		T * mListener;
		Function mFunction;
	};


	template <class P1, class P2>
	class ncListener2 : public _tListener2<tEvent2<P1, P2>, P1, P2>
	{
		typedef void (*Function)(P1 _param1, P2 _param2);

	public:
		ncListener2(Function _func)
			: mFunction(_func)
		{
		}

		virtual ~ncListener2() {}

		virtual void OnCall(P1 _p1, P2 _p2)
		{
			d_assert (mFunction != NULL);

			(mFunction)(_p1, _p2);
		}

	protected:
		Function mFunction;
	};

}
