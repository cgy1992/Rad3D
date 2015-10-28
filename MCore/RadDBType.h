/*
*	RadDB Type
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MRoot.h"

namespace Rad {

#define RDB_MODE_READ 1
#define RDB_MODE_WRITE 2
#define RDB_MODE_SUPPER 127
#define RDB_MODE_MONSTER 255

#define RDB_NAME_SIZE 16

	typedef int *rdb_handle;
	typedef Array<IObject *, t_alloc_pool<IObject *> > rdb_collection;

	inline void rdb_clear(rdb_collection * cl)
	{
		for (int i  = 0; i < cl->Size(); ++i)
		{
			delete cl->At(0);
		}

		cl->Clear();
	}

	//
	struct M_ENTRY rdb_obj_test : public IObject
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(IObject);

		int data0;
		float data1;
		String data2;
	};

	//
	typedef int64	rdb_int;
	typedef uint64	rdb_uint;
	typedef double	rdb_real;

	struct M_ENTRY rdb_value
	{
		DECLARE_POOL_ALLOC();

	public:
		enum {
			TYPE_VOID,

			TYPE_INT,
			TYPE_UINT,
			TYPE_REAL,
			TYPE_STRING,

			TYPE_NULL = 0xFF,
		};

	public:
		rdb_value();
		rdb_value(const rdb_value & v);
		~rdb_value();

		void
			Clear();

		void
			SetInt(rdb_int v);
		void
			SetUInt(rdb_uint v);
		void
			SetReal(rdb_real v);
		void
			SetString(const char * v);
		void
			SetNull();

		rdb_value
			UpType(int type);
		rdb_value
			DownType(int type);

		int
			Type() const { return (int)i_type; }
		rdb_int
			AsInt() const { d_assert(i_type == TYPE_INT); return v_int; }
		rdb_uint
			AsUInt() const { d_assert(i_type == TYPE_UINT); return v_uint; }
		rdb_real
			AsReal() const { d_assert(i_type == TYPE_REAL); return v_real; }
		const char *
			AsString() const { d_assert(i_type == TYPE_STRING); return v_string; }

		rdb_value
			operator +() const;
		rdb_value
			operator -() const;

		rdb_value
			operator +(const rdb_value & rk) const;
		rdb_value
			operator -(const rdb_value & rk) const;
		rdb_value
			operator *(const rdb_value & rk) const;
		rdb_value
			operator /(const rdb_value & rk) const;
		rdb_value
			operator &(const rdb_value & rk) const;
		rdb_value
			operator |(const rdb_value & rk) const;

		rdb_value &
			operator =(const rdb_value & rk);
		rdb_value &
			operator +=(const rdb_value & rk);
		rdb_value &
			operator -=(const rdb_value & rk);
		rdb_value &
			operator *=(const rdb_value & rk);
		rdb_value &
			operator /=(const rdb_value & rk);

		int
			compare(const rdb_value & rk) const;

		rdb_value 
			operator <(const rdb_value & rk) const;
		rdb_value 
			operator <=(const rdb_value & rk) const;
		rdb_value 
			operator >(const rdb_value & rk) const;
		rdb_value 
			operator >=(const rdb_value & rk) const;
		rdb_value 
			operator ==(const rdb_value & rk) const;
		rdb_value 
			operator !=(const rdb_value & rk) const;
		rdb_value 
			operator &&(const rdb_value & rk) const;
		rdb_value 
			operator ||(const rdb_value & rk) const;

	protected:
		byte i_type;

		union {
			rdb_int	v_int;
			rdb_uint v_uint;
			rdb_real v_real;
			char *	v_string;
		};
	};

#define RSQL_MAX_ARGS 8
	typedef FixedArray<rdb_value, RSQL_MAX_ARGS> rdb_vlist;

	//
	typedef bool (*rdb_function)(IObject * record, rdb_value * result, const rdb_vlist * args);

}