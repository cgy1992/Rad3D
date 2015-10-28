#include "MMath.h"

#define lz77_wnd_size (1024)
#define lz77_off_bits (10)
#define lz77_off_mask ((1 << lz77_off_bits) - 1)
//#define lz77_len_bits (5)
//#define lz77_len_mask ((1 << lz77_len_bits) - 1)
//#define lz77_len_max  lz77_len_mask
#define lz77_len_max 256
#define lz77_golomb_m 3

namespace Rad {

	void lz77_test(const byte * str1, const byte * str2, int s1len, int * poff, int * plen)
	{
		int offset, length;

		*poff = offset = 0;
		*plen = length = 0;

		while (s1len >= 3)
		{
			do 
			{
				const byte * b1 = str1;
				const byte * b2 = str2;

				if (*b1++ != *b2++ ||
					*b1++ != *b2++ ||
					*b1++ != *b2++ )
					continue;

				length = 3;

				for (int i = 3; i < s1len && i < lz77_len_max; ++i)
				{
					if (*b1++ == *b2++)
						++length;
					else
						break;
				}

			} while (0);
			
			if (length > *plen)
			{
				*plen = length;
				*poff = offset;
			}

			++str1;
			++offset;
			--s1len;
		}
	}

	void lz77_write1bit(byte * buff, int offset)
	{
		int i_byte = offset / 8;
		int i_bit = offset & 7;

		*(buff + i_byte) |= (1 << i_bit);
	}

	void lz77_write0bit(byte * buff, int offset)
	{
		int i_byte = offset / 8;
		int i_bit = offset & 7;

		*(buff + i_byte) &= ~(1 << i_bit);
	}

	int lz77_readbit(const byte * buff, int offset)
	{
		int i_byte = offset / 8;
		int i_bit = offset & 7;

		return ((int)(*(buff + i_byte)) >> i_bit) & 1;
	}

	int lz77_writelen(int x, byte * buff, int & bitoff)
	{
		int q, r;

		q = (x - 1) >> 3;
		r = x - (q << 3) - 1;

		for (int i = 0; i < q; ++i)
		{
			lz77_write1bit(buff, bitoff++);
		}

		lz77_write0bit(buff, bitoff++);

		for (int i = 0; i < lz77_golomb_m; ++i)
		{
			if ((r >> i) & 1)
				lz77_write1bit(buff, bitoff++);
			else
				lz77_write0bit(buff, bitoff++);
		}

		return lz77_golomb_m + q + 1;
	}

	int lz77_readlen(const byte * buff, int & bitoff)
	{
		int q = 0, r = 0;
		int bit;

		while (1)
		{
			bit = lz77_readbit(buff, bitoff++);
			if (!bit)
				break;

			++q;
		}

		for (int i = 0; i < lz77_golomb_m; ++i)
		{
			bit = lz77_readbit(buff, bitoff++);
			bit <<= i;
			r |= bit;
		}

		return (q << lz77_golomb_m) + r + 1;
	}

	int Math::LZ77_Encode(byte * dest, int dsize, const byte * src, int ssize)
	{
		int slider = -lz77_wnd_size;
		int position = 0;
		int bitoff = 3;
		int maxbits = dsize * 8;
		int maxlen = 0;

		while (position < ssize)
		{
			const byte * wnd = NULL;

			if (slider >= 0)
			{
				wnd = src + slider;
				maxlen = lz77_wnd_size;
			}
			else if (slider >= -lz77_wnd_size)
			{
				wnd = src;
				maxlen = lz77_wnd_size + slider;
			}
			else
			{
				wnd = NULL;
				maxlen = 0;
			}

			if (maxlen > ssize - position)
			{
				maxlen = ssize - position;
			}

			int offset, length;
			const byte * uncomped = src + position;

			lz77_test(wnd, uncomped, maxlen, &offset, &length);
			if (length >= 3)
			{
				if (bitoff + 1 + lz77_off_bits + 8 <= maxbits)
				{
					lz77_write1bit(dest, bitoff++);

					for (int i = 0; i < lz77_off_bits; ++i)
					{
						if ((offset >> i) & 1)
							lz77_write1bit(dest, bitoff++);
						else
							lz77_write0bit(dest, bitoff++);
					}

					lz77_writelen(length, dest, bitoff);
				}
				else
				{
					return -1;
				}

				slider += length;
				position += length;
			}
			else
			{
				if (bitoff + 9 <= maxbits)
				{
					lz77_write0bit(dest, bitoff++);

					int ch = *uncomped;
					for (int i = 0; i < 8; ++i)
					{
						if ((ch >> i) & 1)
							lz77_write1bit(dest, bitoff++);
						else
							lz77_write0bit(dest, bitoff++);
					}
				}
				else
				{
					return -1;
				}

				slider += 1;
				position += 1;
			}
		}

		int encodes = (bitoff + 7) / 8;
		int bitmore = encodes * 8 - bitoff;
		for (int i = 0; i < 3; ++i)
		{
			if ((bitmore >> i) & 1)
				lz77_write1bit(dest, i);
			else
				lz77_write0bit(dest, i);
		}

		return encodes;
	}

	int Math::LZ77_Decode(byte * dest, int dsize, const byte * src, int ssize)
	{
		const byte * wnd = NULL;
		int slider = -lz77_wnd_size;
		int bitoff = 0;
		int decodes = 0;
		int bit = 0;
		int srcbits = ssize * 8;

		bit = lz77_readbit(src, bitoff++);
		bit |= lz77_readbit(src, bitoff++) << 1;
		bit |= lz77_readbit(src, bitoff++) << 2;
		srcbits -= bit;

		while (bitoff < srcbits)
		{
			bit = lz77_readbit(src, bitoff++);
			if (bit)
			{
				if (slider >= 0)
					wnd = dest + slider;
				else
					wnd = dest;

				int offset = 0;
				for(int i = 0; i < lz77_off_bits; i++)
				{
					bit = lz77_readbit(src, bitoff++);
					offset |= (bit<<i);
				}

				int len = lz77_readlen(src, bitoff);
				d_assert (len < lz77_len_max);

				if (decodes + len <= dsize)
					memcpy(dest + decodes, wnd + offset, len);
				else
					return -1;

				decodes += len;
				slider += len;
			}
			else
			{
				byte ch = 0;
				for(int i = 0; i < 8 ; i++)
				{
					bit = lz77_readbit(src, bitoff++);
					ch |= bit << i;
				}

				if (decodes + 1 <= dsize)
					dest[decodes] = ch;
				else
					return -1;

				decodes += 1;
				slider += 1;
			}
		}

		d_assert (bitoff == srcbits);

		return decodes;
	}

	// only use byte operator
	/*
	int Math::LZ77_Encode(byte * dest, int dsize, const byte * src, int ssize)
	{
		const byte * wnd = NULL;
		int slider = -lz77_wnd_size;
		int position = 0;
		int encodes = 0;
		int maxLen = 0;

		while (position < ssize)
		{
			if (slider >= 0)
			{
				wnd = src + slider;
				maxLen = lz77_wnd_size;
			}
			else
			{
				wnd = src;
				maxLen = lz77_wnd_size + slider;
			}

			if (maxLen > ssize - position)
			{
				maxLen = ssize - position;
			}

			int offset, length;
			const byte * uncomped = src + position;

			lz77_test(wnd, uncomped, maxLen, &offset, &length);
			if (length > 3)
			{
				word offlen = 1 << 15;
				offlen |= offset << lz77_len_bits;
				offlen |= length;
				
				if (encodes + 2 <= dsize)
					*(word *)(&dest[encodes]) = (word)M_SWAP16(offlen);
				else
					return -1;

				encodes += 2;
				slider += length;
				position += length;
			}
			else
			{
				int ch = *uncomped;
				if (ch & 0x80)
				{
					if (encodes + 2 <= dsize)
						*(word *)(&dest[encodes]) = (word)M_SWAP16(0x80 << 8);
					else
						return -1;

					encodes += 2;
				}

				if (encodes + 1 <= dsize)
					dest[encodes] = (byte)ch;
				else
					return -1;

				encodes += 1;
				slider += 1;
				position += 1;
			}
		}

		d_assert (position == ssize);

		return encodes;
	}

	int Math::LZ77_Decode(byte * dest, int dsize, const byte * src, int ssize)
	{
		const byte * wnd = NULL;
		int slider = -lz77_wnd_size;
		int position = 0;
		int decodes = 0;

		while (position < ssize)
		{
			byte b0 = src[position++];
			if (b0 & 0x80)
			{
				if (slider >= 0)
					wnd = dest + slider;
				else
					wnd = dest;

				word offlen = (((word)b0 & 0x7F) << 8) | (word)src[position++];
				if (offlen != 0)
				{
					int offset = (offlen >> lz77_len_bits) & lz77_off_mask;
					int len = offlen & lz77_len_mask;
					d_assert (len > 3 && len <= lz77_len_max);

					if (decodes + len <= dsize)
						memcpy(dest + decodes, wnd + offset, len);
					else
						return -1;

					slider += len;
					decodes += len;
				}
				else
				{
					if (decodes + 1 <= dsize)
						dest[decodes++] = src[position++];
					else
						return -1;

					slider += 1;
				}
			}
			else
			{
				if (decodes + 1 <= dsize)
					dest[decodes++] = b0;
				else
					return -1;

				slider += 1;
			}
		}

		d_assert (position == ssize);

		return decodes;
	}
	*/

}