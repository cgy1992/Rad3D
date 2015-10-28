/*
*	Token
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MArray.h"
#include "MString.h"
#include "MFixedString.h"

namespace Rad {

	#define TOKEN_KEY_MAP_SIZE 8

	class M_ENTRY Token
	{
	public:
		struct key_t {
			int type;
			FixedString8 str;

			key_t() {}
			key_t(int t, const char * s) : type(t), str(s) {}
		};

		struct node_t {
			const key_t * key;
			const char * str;

			int type() { d_assert(key != NULL); return key->type; }
		};

		class iterator
		{
		public:
			iterator(Token * tok, int i);
			iterator();
			~iterator();

			iterator 
				next();
			iterator 
				prev();
			bool 
				valid();
			node_t *
				node();

			node_t *
				operator->();

			iterator &
				operator =(const iterator & k)
			{
				cur = k.cur;
				tok = k.tok;
				return *this;
			}

			bool
				operator ==(const iterator & k)
			{
				return cur == k.cur;
			}

			bool
				operator !=(const iterator & k)
			{
				return cur != k.cur;
			}

			bool
				operator <(const iterator & k)
			{
				return cur < k.cur;
			}

			bool
				operator <=(const iterator & k)
			{
				return cur <= k.cur;
			}

			bool
				operator >(const iterator & k)
			{
				return cur > k.cur;
			}

			bool
				operator >=(const iterator & k)
			{
				return cur >= k.cur;
			}

		protected:
			Token * tok;
			int cur;
		};

		friend class iterator;

	public:
		Token();
		~Token();

		void 
			AddKey(int t, const char * s);
		
		void 
			Parse(char * str);
		void
			Clear();

		iterator
			Begin() { return iterator(this, 0); }
		iterator
			End() { return iterator(this, mNodes.Size()); }

	protected:
		void
			New(const key_t * key);

	protected:
		FixedArray<key_t, TOKEN_KEY_MAP_SIZE> mKeyMaps[256];
		Array<node_t> mNodes;
		node_t * mCur;
	};

}