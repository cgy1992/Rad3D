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

	class _tEvent;

	class M_ENTRY _tListener
	{
		DECLARE_POOL_ALLOC();

	public:
		_tEvent * _Event;
		_tListener * _Prev;
		_tListener * _Next;

	public:
		_tListener() : _Event(NULL), _Prev(NULL), _Next(NULL) {}
		virtual ~_tListener();
	};

	class _tEvent
	{
	protected:
		_tListener * mHead;

	public:
		_tEvent() : mHead(NULL) {}
		~_tEvent() { DetachAll(); }

		_tListener * GetHead()
		{
			return mHead;
		}

		_tListener * GetTail()
		{
			_tListener * tail = mHead;
			while (tail)
			{
				if (tail->_Next == NULL)
					return tail;

				tail = tail->_Next;
			}

			return NULL;
		}

		void Attach(_tListener * _listener)
		{
			d_assert (_listener != NULL && _listener->_Event == NULL);
			d_assert (_listener->_Prev == NULL && _listener->_Next == NULL);

			_listener->_Event = this;

			// attach to tail
			//
			_tListener * tail = GetTail();
			if (tail)
			{
				tail->_Next = _listener;
				_listener->_Prev = tail;
			}
			else
			{
				mHead = _listener;
			}
		}

		void Detach(_tListener * _listener)
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

		void DetachAll()
		{
			while (mHead)
				delete mHead;
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