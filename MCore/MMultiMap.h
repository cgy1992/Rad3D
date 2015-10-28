/*
*	MultiMap
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MArray.h"
#include "MAllocator.h"

namespace Rad {

	template <class Key, class T>
	class MultiMap
	{
	public :
		struct Node
		{
			Node * _prev;
			Node * _next;

			Key key;
			Array<T> members;

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
		MultiMap()
		{
			_begin = _end = &k_end;
			k_end._next = k_end._prev = NULL;
		}

		~MultiMap()
		{
			Clear();
		}

		void Clear()
		{
			mAlloc.Clear();

			_begin = _end = &k_end;
			k_end._next = k_end._prev = NULL;
		}

		int Size() const
		{
			return mAlloc.GetActiveCount();
		}

		bool Empty() const
		{
			return Size() == 0;
		}

		const Node * Begin() const
		{
			return _begin;
		}

		const Node * End() const
		{
			return _end;
		}

		Node * Begin()
		{
			return _begin;
		}

		Node * End()
		{
			return _end;
		}

		const Node * Find(const Key & k) const
		{
			const Node * n = Begin();
			while (n != End())
			{
				if (n->key == k)
					break;

				n = n->Next();
			}

			return n;
		}

		Node * Find(const Key & k)
		{
			Node * n = Begin();
			while (n != End())
			{
				if (n->key == k)
					break;

				n = n->Next();
			}

			return n;
		}

		Node * Insert(const Key & k, const T & v)
		{
			Node * i = Find(k);
			if (i != End())
			{
				i->members.PushBack(v);
			}
			else
			{
				i = mAlloc.Alloc();
				i->key = k;
				i->members.PushBack(v);

				i->_prev = _end->_prev;
				i->_next = _end->_next;
				_end->_prev = i;

				if (_begin == _end)
					_begin = i;
			}

			return i;
		}

		Node * Erase(const Key & k)
		{
			Node * i = Find(k);
			if (i != End())
			{
				Node * _next = i->_next;

				if (i->_prev != NULL)
					i->_prev->_next = i->_next;

				i->_next->_prev = i->_prev;

				if (i == _begin)
					_begin = _next;

				return _next;
			}

			return NULL;
		}

		MultiMap(const MultiMap & m)
		{
			_begin = _end = &k_end;
			k_end._next = k_end._prev = NULL;

			*this = m;
		}

		MultiMap & operator = (const MultiMap & m)
		{
			const Node * i = m.Begin();
			while (i != m.End())
			{
				Node * n = mAlloc.Alloc();
				n->key = i->key;
				n->members = i->members;

				n->_prev = _end->_prev;
				n->_next = _end->_next;
				_end->_prev = n;

				if (_begin == _end)
					_begin = n;

				i = i->Next();
			}

			return *this;
		}

	protected:
		Allocator<Node, 128> mAlloc;

		Node k_end;
		Node * _begin;
		Node * _end;
	};

}
