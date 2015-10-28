/*
*	HashMap
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MAllocator.h"

namespace Rad {

	template <class Key, class Type, class HashFunc>
	class HashMap
	{
	public:
		struct Node
		{
			friend class HashMap;

			Key        key;
			Type       value;

		protected:
			int		   hash;
			Node *     prev;
			Node *     next;
		};

		struct List
		{
			Node *     head;
			Node *     tail;

			List() : head(NULL), tail(NULL) {}
		};
    
	public:
		HashMap(int size)
		{
			d_assert (size > 0);

			// must be pow of 2.
			--size; 
			size |= size >> 1; 
			size |= size >> 2;
			size |= size >> 4;
			size |= size >> 8;
			size |= size >> 16;
			++size;

			mList = new List[size];
			mHashSize = size;
		}

		HashMap()
		{
			mList = new List[256];
			mHashSize = 256;
		}

		~HashMap()
		{
			mAlloc.Clear();
			delete[] mList;
		}

		void Clear()
		{
			mAlloc.Clear();
			for (int i = 0; i < mHashSize; ++i)
			{
				mList[i].head = mList[i].tail = NULL;
			}
		}

		int Size() const
		{
			return mAlloc.GetActiveCount();
		}

		int HashSize() const
		{
			return mHashSize;
		}

		bool Empty() const
		{
			return Size() == 0;
		}

		const Node * Begin() const
		{
			int hash = 0;

			Node * node = mList[hash].head;
			while (!node && ++hash < mHashSize)
			{
				node = mList[hash].head;
			}

			return NULL;
		}

		const Node * End() const
		{
			return NULL;
		}
		
		const Node * Find(const Key & k) const
		{
			int hash = mFunc(k) & (mHashSize - 1);

			Node * node = mList[hash].head;
			while (node && !(node->val.key == k))
			{
				node = node->next;
			}

			return node;
		}

		const Node * Next(Node * i) const
		{
			d_assert (i != End());

			int hash = i->hash;
			Node * node = i->next;

			while (!node && ++hash < mHashSize)
			{
				node = _MyHead(hash);
			}

			return node;
		}

		const Node * Prev(Node * i) const
		{
			d_assert (i != End());

			int hash = i->hash;
			Node * node = i->prev;

			while (!node && --hash >= 0)
			{
				node = _MyTail(hash);
			}

			return node;
		}

		Node * Begin()
		{
			int hash = 0;

			Node * node = mList[hash].head;
			while (!node && ++hash < mHashSize)
			{
				node = mList[hash].head;
			}

			return NULL;
		}

		Node * End()
		{
			return NULL;
		}

		Node * Find(const Key & k)
		{
			int hash = mFunc(k) & (mHashSize - 1);

			Node * node = mList[hash].head;
			while (node && !(node->key == k))
			{
				node = node->next;
			}

			return node;
		}

		Node * Next(Node * i)
		{
			d_assert (i != End());

			int hash = i->hash;
			Node * node = i->next;

			while (!node && ++hash < mHashSize)
			{
				node = _MyHead(hash);
			}
			
			return node;
		}

		Node * Prev(Node * i)
		{
			d_assert (i != End());

			int hash = i->hash;
			Node * node = i->prev;

			while (!node && --hash >= 0)
			{
				node = _MyTail(hash);
			}

			return node;
		}

		Node * Insert(const Key & k, const Type & v)
		{
			d_assert (Find(k) == End());

			int hash = mFunc(k) & (mHashSize - 1);

			Node * prev = mList[hash].tail;
			Node * node = mAlloc.Alloc();

			node->next = NULL;
			node->prev = prev;
			node->key = k;
			node->value = v;
			node->hash = hash;

			if (prev)
				prev->next = node;
			else
				mList[hash].head = node;

			mList[hash].tail = node;

			return node;
		}

		Node * Erase(Node * i)
		{
			d_assert (i != End());

			Node * node = i;
			i = Next(i);

			int hash = node->hash;

			if (node->prev)
			{
				node->prev->next = node->next;
			}
			else
			{
				mList[hash].head = node->next;
			}

			if (node->next)
			{
				node->next->prev = node->prev;
			}
			else
			{
				mList[hash].tail = node->prev;
			}

			mAlloc.Free(node);

			return i;
		}

		Node * Erase(const Key & k)
		{
			Node * i = Find(k);
			if (i != End())
			{
				return Erase(i);
			}

			return End();
		}

	protected:
		Node * _MyHead(int hash) const { return mList[hash].head; }
		Node * _MyTail(int hash) const { return mList[hash].tail; }

	protected:
		Allocator<Node, 128> mAlloc;
		HashFunc mFunc;
		int mHashSize;
		List * mList;
	};

}
