#include "RadDBType.h"

namespace Rad {

	//
	ImplementRTTI(rdb_obj_test, IObject);
	DF_PROPERTY_BEGIN(rdb_obj_test)
		DF_PROPERTY_F(rdb_obj_test, data0, "", "", PT_Int, PF_Primary)
		DF_PROPERTY(rdb_obj_test, data1, "", "", PT_Float)
		DF_PROPERTY(rdb_obj_test, data2, "", "", PT_String)
	DF_PROPERTY_END()

	//
	rdb_value::rdb_value()
	{
		i_type = TYPE_VOID;
		v_int = 0;
	}

	rdb_value::rdb_value(const rdb_value & v)
	{
		i_type = TYPE_VOID;
		v_int = 0;

		*this = v;
	}

	rdb_value::~rdb_value()
	{
		Clear();
	}

	void rdb_value::Clear()
	{
		if (i_type == TYPE_STRING)
		{
			safe_pool_free(v_string);
		}

		i_type = TYPE_VOID;
		v_int = 0;
	}

	void rdb_value::SetInt(rdb_int v)
	{
		Clear();

		i_type = TYPE_INT;
		v_int = v;
	}

	void rdb_value::SetUInt(rdb_uint v)
	{
		Clear();

		i_type = TYPE_UINT;
		v_uint = v;
	}

	void rdb_value::SetReal(rdb_real v)
	{
		Clear();

		i_type = TYPE_REAL;
		v_real = v;
	}

	void rdb_value::SetString(const char * v)
	{
		Clear();

		i_type = TYPE_STRING;

		int len = strlen(v);
		v_string  = (char *)pool_alloc(len + 1);
		strcpy(v_string, v);
	}

	void rdb_value::SetNull()
	{
		Clear();

		i_type = TYPE_NULL;
	}

	rdb_value rdb_value::UpType(int type)
	{
		d_assert (i_type < type);

		rdb_value v;

		switch (type)
		{
		case TYPE_UINT:
			if (i_type == TYPE_INT)
			{
				v.SetUInt((rdb_uint)AsInt());
			}
			break;

		case TYPE_REAL:
			if (i_type == TYPE_INT)
			{
				v.SetReal((rdb_real)AsInt());
			}
			else if (i_type == TYPE_UINT)
			{
				v.SetReal((rdb_real)AsUInt());
			}
			break;
		}

		return v;
	}

	rdb_value rdb_value::DownType(int type)
	{
		d_assert (i_type > type);

		rdb_value v;

		switch (type)
		{
		case TYPE_INT:
			if (i_type == TYPE_UINT)
			{
				v.SetInt((rdb_int)AsUInt());
			}
			else if (i_type == TYPE_REAL)
			{
				v.SetInt((rdb_int)AsReal());
			}
			break;

		case TYPE_UINT:
			if (i_type == TYPE_REAL)
			{
				v.SetUInt((rdb_uint)AsReal());
			}
			break;
		}

		return v;
	}

	rdb_value rdb_value::operator +() const
	{
		rdb_value v;

		switch (i_type)
		{
		case TYPE_INT:
			v.SetInt(+v_int);
			break;

		case TYPE_UINT:
			v.SetUInt(+v_uint);
			break;

		case TYPE_REAL:
			v.SetReal(+v_real);
			break;
		}
		
		 return v;
	}

	rdb_value rdb_value::operator -() const
	{
		rdb_value v;

		switch (i_type)
		{
		case TYPE_INT:
			v.SetInt(-v_int);
			break;

		case TYPE_REAL:
			v.SetReal(-v_real);
			break;
		}

		return v;
	}

	rdb_value rdb_value::operator +(const rdb_value & rk) const
	{
		d_assert (i_type == rk.i_type);

		rdb_value v;

		switch (i_type)
		{
		case TYPE_INT:
			v.SetInt(v_int + rk.v_int);
			break;

		case TYPE_UINT:
			v.SetUInt(v_uint + rk.v_uint);
			break;

		case TYPE_REAL:
			v.SetReal(v_real + rk.v_real);
			break;

		case TYPE_STRING:
			{
				String str = v_string;
				str += rk.v_string;

				v.SetString(str.c_str());
			}
			break;
		}

		return v;
	}

	rdb_value rdb_value::operator -(const rdb_value & rk) const
	{
		d_assert (i_type == rk.i_type);

		rdb_value v;

		switch (i_type)
		{
		case TYPE_INT:
			v.SetInt(v_int - rk.v_int);
			break;

		case TYPE_UINT:
			v.SetUInt(v_uint - rk.v_uint);
			break;

		case TYPE_REAL:
			v.SetReal(v_real - rk.v_real);
			break;
		}

		return v;
	}

	rdb_value rdb_value::operator *(const rdb_value & rk) const
	{
		d_assert (i_type == rk.i_type);

		rdb_value v;

		switch (i_type)
		{
		case TYPE_INT:
			v.SetInt(v_int * rk.v_int);
			break;

		case TYPE_UINT:
			v.SetUInt(v_uint * rk.v_uint);
			break;

		case TYPE_REAL:
			v.SetReal(v_real * rk.v_real);
			break;
		}

		return v;
	}

	rdb_value rdb_value::operator /(const rdb_value & rk) const
	{
		d_assert (i_type == rk.i_type);

		rdb_value v;

		switch (i_type)
		{
		case TYPE_INT:
			v.SetInt(v_int / rk.v_int);
			break;

		case TYPE_UINT:
			v.SetUInt(v_uint / rk.v_uint);
			break;

		case TYPE_REAL:
			v.SetReal(v_real / rk.v_real);
			break;
		}

		return v;
	}

	rdb_value rdb_value::operator &(const rdb_value & rk) const
	{
		d_assert (i_type == rk.i_type);

		rdb_value v;

		switch (i_type)
		{
		case TYPE_INT:
			v.SetInt(rk.v_int & rk.v_int);
			break;

		case TYPE_UINT:
			v.SetUInt(rk.v_uint & rk.v_uint);
			break;
		}

		return v;
	}

	rdb_value rdb_value::operator |(const rdb_value & rk) const
	{
		d_assert (i_type == rk.i_type);

		rdb_value v;

		switch (i_type)
		{
		case TYPE_INT:
			v.SetInt(rk.v_int | rk.v_int);
			break;

		case TYPE_UINT:
			v.SetUInt(rk.v_uint | rk.v_uint);
			break;
		}

		return v;
	}

	rdb_value & rdb_value::operator =(const rdb_value & rk)
	{
		d_assert (i_type == TYPE_VOID || i_type == rk.i_type);

		rdb_value & v = *this;

		switch (rk.i_type)
		{
		case TYPE_INT:
			v.SetInt(rk.v_int);
			break;

		case TYPE_UINT:
			v.SetUInt(rk.v_uint);
			break;

		case TYPE_REAL:
			v.SetReal(rk.v_real);
			break;

		case TYPE_STRING:
			v.SetString(rk.v_string);
			break;

		case TYPE_NULL:
			v.SetNull();
			break;
		}

		return v;
	}

	rdb_value & rdb_value::operator +=(const rdb_value & rk)
	{
		d_assert (i_type == rk.i_type);

		rdb_value & v = *this;

		switch (i_type)
		{
		case TYPE_INT:
			v.SetInt(v_int + rk.v_int);
			break;

		case TYPE_UINT:
			v.SetUInt(v_uint + rk.v_uint);
			break;

		case TYPE_REAL:
			v.SetReal(v_real + rk.v_real);
			break;

		case TYPE_STRING:
			{
				String str = v_string;
				str += rk.v_string;

				v.SetString(str.c_str());
			}
			break;
		}

		return v;
	}

	rdb_value & rdb_value::operator -=(const rdb_value & rk)
	{
		d_assert (i_type == rk.i_type);

		rdb_value & v = *this;

		switch (i_type)
		{
		case TYPE_INT:
			v.SetInt(v_int - rk.v_int);
			break;

		case TYPE_UINT:
			v.SetUInt(v_uint - rk.v_uint);
			break;

		case TYPE_REAL:
			v.SetReal(v_real - rk.v_real);
			break;

		case TYPE_STRING:
			v.Clear();
			break;
		}

		return v;
	}

	rdb_value & rdb_value::operator *=(const rdb_value & rk)
	{
		d_assert (i_type == rk.i_type);

		rdb_value & v = *this;

		switch (i_type)
		{
		case TYPE_INT:
			v.SetInt(v_int * rk.v_int);
			break;

		case TYPE_UINT:
			v.SetUInt(v_uint * rk.v_uint);
			break;

		case TYPE_REAL:
			v.SetReal(v_real * rk.v_real);
			break;

		case TYPE_STRING:
			v.Clear();
			break;
		}

		return v;
	}

	rdb_value & rdb_value::operator /=(const rdb_value & rk)
	{
		d_assert (i_type == rk.i_type);

		rdb_value & v = *this;

		switch (i_type)
		{
		case TYPE_INT:
			v.SetInt(v_int / rk.v_int);
			break;

		case TYPE_UINT:
			v.SetUInt(v_uint / rk.v_uint);
			break;

		case TYPE_REAL:
			v.SetReal(v_real / rk.v_real);
			break;

		case TYPE_STRING:
			v.Clear();
			break;
		}

		return v;
	}

	int rdb_value::compare(const rdb_value & rk) const
	{
		d_assert (i_type != TYPE_VOID && i_type == rk.i_type);

		int ret = -1;

		switch (i_type)
		{
		case TYPE_INT:
			ret = v_int < rk.v_int ? -1 : (v_int == rk.v_int ? 0 : 1);
			break;

		case TYPE_UINT:
			ret = v_uint < rk.v_uint ? -1 : (v_uint == rk.v_uint ? 0 : 1);
			break;

		case TYPE_REAL:
			ret = v_real < rk.v_real ? -1 : (v_real == rk.v_real ? 0 : 1);
			break;

		case TYPE_STRING:
			ret = strcmp(v_string, rk.v_string);
			break;
		}

		return ret;
	}

	rdb_value rdb_value::operator <(const rdb_value & rk) const
	{
		d_assert (i_type == rk.i_type);

		rdb_value v;

		switch (i_type)
		{
		case TYPE_INT:
			v.SetInt(v_int < rk.v_int ? 1 : 0);
			break;

		case TYPE_UINT:
			v.SetUInt(v_uint < rk.v_uint ? 1 : 0);
			break;

		case TYPE_REAL:
			v.SetReal(v_real < rk.v_real ? 1 : 0);
			break;

		case TYPE_STRING:
			v.SetReal(strcmp(v_string, rk.v_string) < 0 ? 1 : 0);
			break;
		}

		return v;
	}

	rdb_value rdb_value::operator <=(const rdb_value & rk) const
	{
		d_assert (i_type == rk.i_type);

		rdb_value v;

		switch (i_type)
		{
		case TYPE_INT:
			v.SetInt(v_int <= rk.v_int ? 1 : 0);
			break;

		case TYPE_UINT:
			v.SetUInt(v_uint <= rk.v_uint ? 1 : 0);
			break;

		case TYPE_REAL:
			v.SetReal(v_real <= rk.v_real ? 1 : 0);
			break;

		case TYPE_STRING:
			v.SetReal(strcmp(v_string, rk.v_string) <= 0 ? 1 : 0);
			break;
		}

		return v;
	}

	rdb_value rdb_value::operator >(const rdb_value & rk) const
	{
		d_assert (i_type == rk.i_type);

		rdb_value v;

		switch (i_type)
		{
		case TYPE_INT:
			v.SetInt(v_int > rk.v_int ? 1 : 0);
			break;

		case TYPE_UINT:
			v.SetUInt(v_uint > rk.v_uint ? 1 : 0);
			break;

		case TYPE_REAL:
			v.SetReal(v_real > rk.v_real ? 1 : 0);
			break;

		case TYPE_STRING:
			v.SetReal(strcmp(v_string, rk.v_string) > 0 ? 1 : 0);
			break;
		}

		return v;
	}

	rdb_value rdb_value::operator >=(const rdb_value & rk) const
	{
		d_assert (i_type == rk.i_type);

		rdb_value v;

		switch (i_type)
		{
		case TYPE_INT:
			v.SetInt(v_int >= rk.v_int ? 1 : 0);
			break;

		case TYPE_UINT:
			v.SetUInt(v_uint >= rk.v_uint ? 1 : 0);
			break;

		case TYPE_REAL:
			v.SetReal(v_real >= rk.v_real ? 1 : 0);
			break;

		case TYPE_STRING:
			v.SetReal(strcmp(v_string, rk.v_string) >= 0 ? 1 : 0);
			break;
		}

		return v;
	}

	rdb_value rdb_value::operator ==(const rdb_value & rk) const
	{
		d_assert (i_type == rk.i_type);

		rdb_value v;

		switch (i_type)
		{
		case TYPE_INT:
			v.SetInt(v_int == rk.v_int ? 1 : 0);
			break;

		case TYPE_UINT:
			v.SetUInt(v_uint == rk.v_uint ? 1 : 0);
			break;

		case TYPE_REAL:
			v.SetReal(v_real == rk.v_real ? 1 : 0);
			break;

		case TYPE_STRING:
			v.SetReal(strcmp(v_string, rk.v_string) == 0 ? 1 : 0);
			break;
		}

		return v;
	}

	rdb_value rdb_value::operator !=(const rdb_value & rk) const
	{
		d_assert (i_type == rk.i_type);

		rdb_value v;

		switch (i_type)
		{
		case TYPE_INT:
			v.SetInt(v_int != rk.v_int ? 1 : 0);
			break;

		case TYPE_UINT:
			v.SetUInt(v_uint != rk.v_uint ? 1 : 0);
			break;

		case TYPE_REAL:
			v.SetReal(v_real != rk.v_real ? 1 : 0);
			break;

		case TYPE_STRING:
			v.SetReal(strcmp(v_string, rk.v_string) != 0 ? 1 : 0);
			break;
		}

		return v;
	}

	rdb_value rdb_value::operator &&(const rdb_value & rk) const
	{
		d_assert (i_type == rk.i_type);

		rdb_value v;

		switch (i_type)
		{
		case TYPE_INT:
			v.SetInt(v_int && rk.v_int ? 1 : 0);
			break;

		case TYPE_UINT:
			v.SetUInt(v_uint && rk.v_uint ? 1 : 0);
			break;

		case TYPE_REAL:
			v.SetReal(v_real && rk.v_real ? 1 : 0);
			break;
		}

		return v;
	}

	rdb_value rdb_value::operator ||(const rdb_value & rk) const
	{
		d_assert (i_type == rk.i_type);

		rdb_value v;

		switch (i_type)
		{
		case TYPE_INT:
			v.SetInt(v_int || rk.v_int ? 1 : 0);
			break;

		case TYPE_UINT:
			v.SetUInt(v_uint || rk.v_uint ? 1 : 0);
			break;

		case TYPE_REAL:
			v.SetReal(v_real || rk.v_real ? 1 : 0);
			break;
		}

		return v;
	}

}