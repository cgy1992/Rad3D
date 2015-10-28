/*
*	Event1 
*		1 parameter event & listener
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MMemory.h"

namespace Rad {

	template <class T, class P1>
	class _tListener1
	{
		DECLARE_POOL_ALLOC();

	public:
		T * _Event;
		_tListener1 * _Prev;
		_tListener1 * _Next;

	public:
		_tListener1() : _Event(NULL), _Prev(NULL), _Next(NULL) {}
		virtual ~_tListener1() { if (_Event) (*_Event) -= this; }

		virtual void OnCall(P1 _p1) = 0;
	};

	template <class P1>
	class tEvent1
	{
	public:
		typedef _tListener1<tEvent1, P1> tMyListener;

	public:
		tEvent1() { mHead = NULL; }
		~tEvent1() { DetachAll(); }

		void DetachAll()
		{
			while (mHead)
			{
				delete mHead;
			}
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

		void operator ()(P1 _p1)
		{
			tMyListener * node = mHead;
			tMyListener * next = NULL;

			while (node != NULL)
			{
				next = node->_Next;

				node->OnCall(_p1);

				node = next;
			}
		}

	protected:
		tMyListener * mHead;
	};

	template <class T, class P1>
	class cListener1 : public _tListener1<tEvent1<P1>, P1>
	{
		typedef void (T::*Function)(P1 _param1);

	public:
		cListener1() : mListener(NULL), mFunction(NULL) {};
		cListener1(T * _listener, Function _func) : mListener(_listener), mFunction(_func) {}
		virtual ~cListener1() {}

		virtual void OnCall(P1 _p1)
		{
			d_assert (mListener != NULL && mFunction != NULL);

			(mListener->*mFunction)(_p1);
		}

		cListener1 * operator ()(T * _listener, Function _func)
		{
			mListener = _listener;
			mFunction = _func;

			return this;
		}

	protected:
		T * mListener;
		Function mFunction;
	};

	template <class P1>
	class ncListener1 : public _tListener1<tEvent1<P1>, P1>
	{
		typedef void (*Function)(P1 _param1);

	public:
		ncListener1(Function _func)
			: mFunction(_func)
		{
		}

		virtual ~ncListener1() {}

		virtual void OnCall(P1 _p1)
		{
			d_assert (mFunction != NULL);

			(mFunction)(_p1);
		}

	protected:
		Function mFunction;
	};
}
