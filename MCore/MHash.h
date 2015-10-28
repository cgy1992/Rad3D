/*
*	Aabb
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MMath.h"

namespace Rad {

	struct Hash2
	{
		union {
			struct {
				dword dwHashA;
				dword dwHashB;
			};

			qword val;
		};

		Hash2()
		{
			dwHashA = dwHashB = 0;
		}

		Hash2(const char * str)
		{
			dwHashA = Math::Crc32(str);
			dwHashB = *str ? Math::Crc32(str + 1) : 0;
		}

		bool operator ==(const Hash2 & rk) const
		{
			return val == rk.val;
		}

		bool operator !=(const Hash2 & rk) const
		{
			return val != rk.val;
		}

		bool operator <(const Hash2 & rk) const
		{
			return val < rk.val;
		}

		bool operator <=(const Hash2 & rk) const
		{
			return val <= rk.val;
		}

		bool operator >(const Hash2 & rk) const
		{
			return val > rk.val;
		}

		bool operator >=(const Hash2 & rk) const
		{
			return val >= rk.val;
		}
	};

}
