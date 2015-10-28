/*
*	Event0
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MMemory.h"

namespace Rad {

	template <class T>
	class _tListener0
	{
		DECLARE_POOL_ALLOC();

	public:
		T * _Event;
		_tListener0 * _Prev;
		_tListener0 * _Next;

	public:
		_tListener0() : _Event(NULL), _Prev(NULL), _Next(NULL) {}
		virtual ~_tListener0() { if (_Event) (*_Event) -= this; }

		virtual void OnCall() = 0;
	};

	class tEvent0
	{
	public:
		typedef _tListener0<tEvent0> tMyListener;

	public:
		tEvent0() : mHead(NULL) {}
		~tEvent0() { DetachAll(); }

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

		void operator ()()
		{
			tMyListener * node = mHead;
			tMyListener * next = NULL;

			while (node != NULL)
			{
				next = node->_Next;

				node->OnCall();

				node = next;
			}
		}

	protected:
		tMyListener * mHead;
	};

	template <class T>
	class cListener0 : public _tListener0<tEvent0>
	{
		typedef void (T::*Function)();

	public:
		cListener0() : mListener(NULL), mFunction(NULL) {}
		cListener0(T * _listener, Function _func) : mListener(_listener), mFunction(_func) {}
		virtual ~cListener0() {}

		virtual void OnCall()
		{
			d_assert (mListener != NULL && mFunction != NULL);

			(mListener->*mFunction)();
		}

		cListener0 * operator ()(T * _listener, Function _func)
		{
			mListener = _listener;
			mFunction = _func;

			return this;
		}

	protected:
		T * mListener;
		Function mFunction;
	};

	class ncListener0 : public _tListener0<tEvent0>
	{
		typedef void (*Function)();

	public:
		ncListener0(Function _func)
			: mFunction(_func)
		{
		}

		virtual ~ncListener0() {}

		virtual void OnCall()
		{
			d_assert (mFunction != NULL);

			mFunction();
		}

	protected:
		Function mFunction;
	};
}
