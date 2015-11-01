/*
*	Event
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MMemory.h"

namespace Rad {

	class _iEvent;

	class M_ENTRY _tListener
	{
		DECLARE_POOL_ALLOC();

	public:
		_iEvent * _Event;
		_tListener * _Prev;
		_tListener * _Next;

		void * _This;

	public:
		_tListener(void * thiz);
		virtual ~_tListener();
	};

	class M_ENTRY _iEvent
	{
	public:
		_iEvent() : mHead(NULL), mTail(NULL) {}
		~_iEvent() { DetachAll(); }

		void Attach(_tListener * _listener)
		{
			d_assert (_listener != NULL && _listener->_Event == NULL);
			d_assert (_listener->_Prev == NULL && _listener->_Next == NULL);

			_listener->_Event = this;

			if (mTail)
			{
				mTail->_Next = _listener;
				_listener->_Prev = mTail;

				mTail = _listener;
			}
			else
			{
				mHead = _listener;
				mTail = _listener;
			}
		}

		void Detach(_tListener * _listener)
		{
			d_assert (_listener != NULL && _listener->_Event == this);
			d_assert (mHead != NULL);

			if (_listener == mHead)
				mHead = _listener->_Next;

			if (_listener == mTail)
				mTail = _listener->_Prev;

			if (_listener->_Prev)
				_listener->_Prev->_Next = _listener->_Next;

			if (_listener->_Next)
				_listener->_Next->_Prev = _listener->_Prev;

			_listener->_Prev = _listener->_Next = NULL;
			_listener->_Event = NULL;
		}

		void Detach(void * _this)
		{
			_tListener * node = mHead;
			while (node != NULL)
			{
				if (node->_This == _this)
				{
					delete node;
					break;
				}

				node = node->_Next;
			}
		}

		void DetachAll()
		{
			while (mHead)
				delete mHead;

			mTail = NULL;
		}

	protected:
		_tListener * mHead;
		_tListener * mTail;
	};

	template <class T>
	class _tEvent : public _iEvent
	{
	public:
		void operator +=(T * _listener)
		{
			Attach(_listener);
		}

		void operator -=(T * _listener)
		{
			Detach(_listener);
		}
	};

}

#include "MEvent0.h"
#include "MEvent1.h"
#include "MEvent2.h"
#include "MEvent3.h"
#include "MEvent4.h"
#include "MEvent5.h"
#include "MEvent6.h"
