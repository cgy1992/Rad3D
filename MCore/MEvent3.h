/*
*	Event3 
*		3 parameter event & listener
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MMemory.h"

namespace Rad {

	template <class T, class P1, class P2, class P3>
	class _tListener3
	{
		DECLARE_POOL_ALLOC();

	public:
		T * _Event;
		_tListener3 * _Prev;
		_tListener3 * _Next;

	public:
		_tListener3() : _Event(NULL), _Prev(NULL), _Next(NULL) {}
		virtual ~_tListener3() { if (_Event) (*_Event) -= this; }

		virtual void OnCall(P1 _p1, P2 _p2, P3 _p3) = 0;
	};

	template <class P1, class P2, class P3>
	class tEvent3
	{
	public:
		typedef _tListener3<tEvent3, P1, P2, P3> tMyListener;

	public:
		tEvent3() { mHead = NULL; }
		~tEvent3() { DetachAll(); }

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

		void operator ()(P1 _p1, P2 _p2, P3 _p3)
		{
			tMyListener * node = mHead;
			tMyListener * next = NULL;

			while (node != NULL)
			{
				next = node->_Next;

				node->OnCall(_p1, _p2, _p3);

				node = next;
			}
		}

	protected:
		tMyListener * mHead;
	};

	template <class T, class P1, class P2, class P3>
	class cListener3 : public _tListener3<tEvent3<P1, P2, P3>, P1, P2, P3>
	{
		typedef void (T::*Function)(P1 _param1, P2 _param2, P3 _param3);

	public:
		cListener3() : mListener(NULL), mFunction(NULL) {}
		cListener3(T * _listener, Function _func) : mListener(_listener), mFunction(_func) {}
		virtual ~cListener3() {}

		virtual void OnCall(P1 _p1, P2 _p2, P3 _p3)
		{
			d_assert (mListener != NULL && mFunction != NULL);

			(mListener->*mFunction)(_p1, _p2, _p3);
		}

		cListener3 * operator ()(T * _listener, Function _func)
		{
			mListener = _listener;
			mFunction = _func;

			return this;
		}

	protected:
		T * mListener;
		Function mFunction;
	};


	template <class P1, class P2, class P3>
	class ncListener3 : public _tListener3<tEvent3<P1, P2, P3>, P1, P2, P3>
	{
		typedef void (*Function)(P1 _param1, P2 _param2, P3 _param3);

	public:
		ncListener3(Function _func)
			: mFunction(_func)
		{
		}

		virtual ~ncListener3() {}

		virtual void OnCall(P1 _p1, P2 _p2, P3 _p3)
		{
			d_assert (mFunction != NULL);

			(mFunction)(_p1, _p2, _p3);
		}

	protected:
		Function mFunction;
	};

}
