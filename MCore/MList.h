/*
*	List
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MAllocator.h"

namespace Rad {

	template <class T>
	class List
	{
	private:
		struct Node
		{
			Node * _prev;
			Node * _next;
			T val;

			Node() { _prev = _next =  NULL; }

			Node * 
				Prev() { return _prev; }
			Node * 
				Next() { return _next; }

			const Node * 
				Prev() const { return _prev; }
			const Node * 
				Next() const { return _next; }
		};

	public:
		List()
		{
			_begin = _end = &k_end;
			k_end._next = k_end._prev = NULL;
		}

		~List()
		{
			Clear();
		}

		Node * Begin()
		{
			return _begin;
		}

		Node * End()
		{
			return _end;
		}

		Node * Begin() const
		{
			return _begin;
		}

		Node * End() const
		{
			return _end;
		}

		int Size() const
		{
			return mAlloc.GetActiveCount();
		}

		void Clear()
		{
			mAlloc.Clear();

			_begin = _end = &k_end;
			k_end._next = k_end._prev = NULL;
		}

		bool Empty() const
		{
			return Size() == 0;
		}

		Node * Insert(Node * i, const T & val)
		{
			d_assert (i != NULL);

			Node * n = mAlloc.Alloc();
			n->val = val;

			if (i->_prev != NULL)
				i->_prev->_next = n;

			n->_prev = i->_prev;
			n->_next = i;
			i->_prev = n;

			if (i == _begin)
				_begin = n;

			return n;
		}

		Node * Erase(Node * i)
		{
			d_assert (i != NULL && i != _end);

			Node * _next = i->_next;

			if (i->_prev != NULL)
				i->_prev->_next = i->_next;

			i->_next->_prev = i->_prev;

			if (i == _begin)
				_begin = _next;

			mAlloc.Free(i);

			return _next;
		}

		void PushFront(const T & val)
		{
			Insert(Begin(), val);
		}

		void PushBack(const T & val)
		{
			Insert(End(), val);
		}
		
		List(const List & rk)
		{
			_begin = _end = &k_end;
			k_end._next = k_end._prev = NULL;

			*this = rk;
		}

		List& operator =(const List & rk)
		{
			d_assert (this != &rk);

			Clear();

			for (Node * i = rk.Begin(); i != rk.End(); i = i->Next())
			{
				PushBack(i->val);
			}

			return *this; 
		}

	protected:
		Allocator<Node, 128>  mAlloc;

		Node k_end;
		Node * _begin;
		Node * _end;
	};

}
