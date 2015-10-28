/*
*	Linker
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MDebug.h"

namespace Rad {

	template <class T>
	struct LinkerT
	{
		T * prev;
		T * next;

		LinkerT()
		{
			prev = NULL;
			next = NULL;
		}
	};

#define DECLARE_LINKER(T) \
public: \
	LinkerT<T> __link_node

	template<class T>
	inline void LINKER_APPEND(T *& head, T * node)
	{
		d_assert ((node)->__link_node.prev == NULL && (node)->__link_node.next == NULL);
			
		if ((head) != NULL)
		{ 
			(head)->__link_node.prev = (node);
			(node)->__link_node.next = (head);
		} 

		head = node;
	}

	template<class T>
	inline void LINKER_APPEND_TO(T *& head, T * _at, T * node)
	{
		d_assert ((node)->__link_node.prev == NULL && (node)->__link_node.next == NULL); 
			
		if ((head) != NULL) 
		{ 
			d_assert ((_at) != NULL); 

			if((_at)->__link_node.next != NULL) 
				(_at)->__link_node.next->__link_node.prev = node; 

			(node)->__link_node.prev = (_at); 
			(node)->__link_node.next = (_at)->__link_node.next; 
			(_at)->__link_node.next = (node); 
		} 
		else 
		{ 
			head = node; 
		}
	}
	
	template<class T>
	inline void LINKER_INSERT(T *& head, T * _at, T * node)
	{
		d_assert ((node)->__link_node.prev == NULL && (node)->__link_node.next == NULL); 
		d_assert (_at != NULL); 

		(node)->__link_node.prev = (_at)->__link_node.prev; 
		(_at)->__link_node.prev = (node); 
		(node)->__link_node.next = (_at); 

		if ((node)->__link_node.prev != NULL) 
			(node)->__link_node.prev->__link_node.next = node; 
		else 
			head = node;
	}

	template<class T>
	inline void LINKER_REMOVE(T *& head, T * node)
	{
		if (head == node) 
			head = (node)->__link_node.next; 

		if ((node)->__link_node.prev) 
			(node)->__link_node.prev->__link_node.next = (node)->__link_node.next; 

		if ((node)->__link_node.next) 
			(node)->__link_node.next->__link_node.prev = (node)->__link_node.prev; 

		(node)->__link_node.prev = (node)->__link_node.next = NULL;
	}

	template<class T>
	inline T * LINKER_NEXT(T * node)
	{
		return ((node)->__link_node.next);
	}

	template <class T>
	inline T * LINKER_PREV(T * node)
	{
		return ((node)->__link_node.prev);
	}
	
}
