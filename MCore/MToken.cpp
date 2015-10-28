#include "MToken.h"

namespace Rad {

	Token::key_t k_key_defualt = Token::key_t(0, "");

	//
	Token::iterator::iterator(Token * _tok, int _i)
	{
		tok = _tok;
		cur = _i;
	}

	Token::iterator::iterator()
	{
		tok = NULL;
		cur = -1;
	}

	Token::iterator::~iterator()
	{
	}

	Token::iterator Token::iterator::next()
	{
		return Token::iterator(tok, cur + 1);
	}

	Token::iterator Token::iterator::prev()
	{
		return Token::iterator(tok, cur - 1);
	}

	bool Token::iterator::valid()
	{
		return cur >= 0 && cur < tok->mNodes.Size();
	}

	Token::node_t * Token::iterator::node()
	{
		return valid() ? &tok->mNodes[cur] : NULL;
	}

	Token::node_t * Token::iterator::operator->()
	{
		d_assert (valid());

		return &tok->mNodes[cur];
	}

	//
	Token::Token()
	{
	}

	Token::~Token()
	{
	}

	void Token::AddKey(int t, const char * s)
	{
		key_t k(t, s);

		FixedArray<key_t, TOKEN_KEY_MAP_SIZE> & arr = mKeyMaps[*s];

		int inserted = false;
		for (int i = 0; i < arr.Size(); ++i)
		{
			if (k.str.Length() >= arr[i].str.Length())
			{
				arr.Insert(i, k);
				inserted = true;
				break;
			}
		}

		if (!inserted)
		{
			arr.PushBack(k);
		}
	}

	void Token::New(const key_t * key)
	{
		mNodes.Resize(mNodes.Size() + 1);

		mCur = &mNodes[mNodes.Size() - 1];
		mCur->key = key;
		mCur->str = "";
	}

	void Token::Parse(char * str)
	{
		Clear();

		bool in_str = false;

		while (*str)
		{
			bool matched = false;

			if (*str == '\"')
			{
				in_str = !in_str;
			}

			if (!in_str && *str == ';')
			{
				*str = ' ';
			}

			if (!in_str && mKeyMaps[*str].Size() > 0)
			{
				const FixedArray<key_t, TOKEN_KEY_MAP_SIZE> & arr =  mKeyMaps[*str];

				for (int i = 0; i < arr.Size() && !matched; ++i)
				{
					if (str_match(str, arr[i].str.c_str()))
					{
						New(&arr[i]);
						*str = 0;
						str += arr[i].str.Length();

						matched = true;
					}
				}
			}

			if (!matched)
			{
				if (!in_str && *str == ' ')
				{
					*str = 0;
					mCur = NULL;
				}
				else
				{
					if (mCur == NULL || mCur->key != &k_key_defualt)
					{
						New(&k_key_defualt);
						mCur->str = str;
					}
				}

				++str;
			}
		}
	}

	void Token::Clear()
	{
		mNodes.Clear();
		mCur = NULL;
	}
}