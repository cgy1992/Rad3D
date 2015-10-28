#include "RadCType.h"

namespace Rad {

	radc_var::radc_var(const char * _name)
		: i_type(radc_enum::type_void)
		, i_name(_name)
		, i_flag(0)
	{
		r_value = 0;
		p_value = 0;
	}

	radc_var::radc_var(const radc_var & v)
		: i_type(radc_enum::type_void)
		, i_name(NULL)
		, i_flag(0)
	{
		r_value = 0;
		p_value = 0;

		*this = v;
	}

	radc_var::~radc_var()
	{
		clear();
	}

	void radc_var::clear()
	{
		if (i_type == radc_enum::type_string)
		{
			safe_pool_free(s_value);
		}

		i_type = radc_enum::type_void;
		r_value = 0;
		p_value = 0;
	}

	void radc_var::setReal(radc_real v)
	{
		clear();

		i_type = radc_enum::type_real;
		r_value = v;
	}

	void radc_var::setString(const char * v, bool fmt)
	{
		clear();

		i_type = radc_enum::type_string;

		int len = strlen(v);
		safe_pool_free(s_value);
		s_value = (char *)pool_alloc(len + 1);
		strcpy(s_value, v);

		if (fmt)
		{
			fmtString();
		}
	}

	void radc_var::fmtString()
	{
		d_assert (i_type == radc_enum::type_string);

		char * str = s_value;
		while (*str)
		{
			if (*str == '\\')
			{
				char ch = *(str + 1);
				char ch2 = 0;

				switch (ch)
				{
				case 't':
					ch2 = '\t';
					break;

				case 'r':
					ch2 = '\r';

				case 'n':
					ch2 = '\n';
				}

				if (ch2 != 0)
				{
					*str = ch2;

					strcpy(str + 1, str + 2);
				}
			}

			++str;
		}
	}

	void radc_var::setPtr(void * v)
	{
		clear();

		i_type = radc_enum::type_ptr;
		p_value = v;
	}

	radc_var radc_var::up_type(int r_type)
	{
		d_assert (i_type < r_type);

		radc_var v;

		switch (r_type)
		{
		case radc_enum::type_string:
			if (i_type == radc_enum::type_real)
			{
				v.setString(f2str((float)r_value, true).c_str());
			}
			break;
		}

		return v;
	}

	radc_var radc_var::down_type(int r_type)
	{
		d_assert (i_type > r_type);

		radc_var v;

		return v;
	}

	radc_var radc_var::operator <(const radc_var & rk) const
	{
		d_assert (i_type == rk.i_type);

		radc_var v(NULL);

		switch (i_type)
		{
		case radc_enum::type_real:
			v.setInt(r_value < rk.r_value ? 1 : 0);
			break;

		case radc_enum::type_string:
			v.setInt(strcmp(s_value, rk.s_value) < 0 ? 1 : 0);
			break;
		}

		return v;
	}

	radc_var radc_var::operator <=(const radc_var & rk) const
	{
		d_assert (i_type == rk.i_type);

		radc_var v(NULL);

		switch (i_type)
		{
		case radc_enum::type_real:
			v.setInt(r_value <= rk.r_value ? 1 : 0);
			break;

		case radc_enum::type_string:
			v.setInt(strcmp(s_value, rk.s_value) <= 0 ? 1 : 0);
			break;
		}

		return v;
	}

	radc_var radc_var::operator >(const radc_var & rk) const
	{
		d_assert (i_type == rk.i_type);

		radc_var v(NULL);

		switch (i_type)
		{
		case radc_enum::type_real:
			v.setInt(r_value > rk.r_value ? 1 : 0);
			break;

		case radc_enum::type_string:
			v.setInt(strcmp(s_value, rk.s_value) > 0 ? 1 : 0);
			break;
		}

		return v;
	}

	radc_var radc_var::operator >=(const radc_var & rk) const
	{
		d_assert (i_type == rk.i_type);

		radc_var v(NULL);

		switch (i_type)
		{
		case radc_enum::type_real:
			v.setInt(r_value >= rk.r_value ? 1 : 0);
			break;

		case radc_enum::type_string:
			v.setInt(strcmp(s_value, rk.s_value) >= 0 ? 1 : 0);
			break;
		}

		return v;
	}

	radc_var radc_var::operator ==(const radc_var & rk) const
	{
		d_assert (i_type == rk.i_type);

		radc_var v(NULL);

		switch (i_type)
		{
		case radc_enum::type_real:
			v.setInt(r_value == rk.r_value ? 1 : 0);
			break;

		case radc_enum::type_string:
			v.setInt(strcmp(s_value, rk.s_value) == 0 ? 1 : 0);
			break;
		}

		return v;
	}

	radc_var radc_var::operator !=(const radc_var & rk) const
	{
		d_assert (i_type == rk.i_type);

		radc_var v(NULL);

		switch (i_type)
		{
		case radc_enum::type_real:
			v.setInt(r_value != rk.r_value ? 1 : 0);
			break;

		case radc_enum::type_string:
			v.setInt(strcmp(s_value, rk.s_value) != 0 ? 1 : 0);
			break;
		}

		return v;
	}

	radc_var radc_var::operator ||(const radc_var & rk) const
	{
		d_assert (i_type == rk.i_type);

		radc_var v(NULL);

		switch (i_type)
		{
		case radc_enum::type_real:
			v.setInt(r_value || rk.r_value ? 1 : 0);
			break;
		}

		return v;
	}

	radc_var radc_var::operator &&(const radc_var & rk) const
	{
		d_assert (i_type == rk.i_type);

		radc_var v(NULL);

		switch (i_type)
		{
		case radc_enum::type_real:
			v.setInt(r_value && rk.r_value ? 1 : 0);
			break;
		}

		return v;
	}

	radc_var & radc_var::operator =(const radc_var & rk)
	{
		clear();

		switch (rk.i_type)
		{
		case radc_enum::type_real:
			setReal(rk.r_value);
			break;

		case radc_enum::type_string:
			setString(rk.s_value);
			break;

		case radc_enum::type_ptr:
			setPtr(rk.p_value);
			break;
		}

		return *this;
	}

	radc_var & radc_var::operator +=(const radc_var & rk)
	{
		d_assert (i_type == rk.i_type);

		switch (i_type)
		{
		case radc_enum::type_real:
			r_value += rk.r_value;
			break;

		case radc_enum::type_string:
			setString(rk.s_value);
			break;

		case radc_enum::type_ptr:
			clear();
			break;
		}

		return *this;
	}

	radc_var & radc_var::operator -=(const radc_var & rk)
	{
		d_assert (i_type == rk.i_type);

		switch (i_type)
		{
		case radc_enum::type_real:
			r_value -= rk.r_value;
			break;

		case radc_enum::type_string:
			clear();
			break;

		case radc_enum::type_ptr:
			clear();
			break;
		}

		return *this;
	}

	radc_var & radc_var::operator *=(const radc_var & rk)
	{
		d_assert (i_type == rk.i_type);

		switch (i_type)
		{
		case radc_enum::type_real:
			r_value *= rk.r_value;
			break;

		case radc_enum::type_string:
			clear();
			break;

		case radc_enum::type_ptr:
			clear();
			break;
		}

		return *this;
	}

	radc_var & radc_var::operator /=(const radc_var & rk)
	{
		d_assert (i_type == rk.i_type);

		switch (i_type)
		{
		case radc_enum::type_real:
			r_value /= rk.r_value;
			break;

		case radc_enum::type_string:
			clear();
			break;

		case radc_enum::type_ptr:
			clear();
			break;
		}

		return *this;
	}

	radc_var radc_var::operator +(const radc_var & rk)
	{
		d_assert (i_type == rk.i_type);

		radc_var v(NULL);

		switch (i_type)
		{
		case radc_enum::type_real:
			v.setReal(r_value + rk.r_value);
			break;

		case radc_enum::type_string:
			{
				String str(s_value);
				str += rk.s_value;
				
				v.setString(str.c_str());
			}
			break;
		}

		return v;
	}

	radc_var radc_var::operator -(const radc_var & rk)
	{
		d_assert (i_type == rk.i_type);

		radc_var v(NULL);

		switch (i_type)
		{
		case radc_enum::type_real:
			v.setReal(r_value - rk.r_value);
			break;
		}

		return v;
	}

	radc_var radc_var::operator *(const radc_var & rk)
	{
		d_assert (i_type == rk.i_type);

		radc_var v(NULL);

		switch (i_type)
		{
		case radc_enum::type_real:
			v.setReal(r_value * rk.r_value);
			break;
		}

		return v;
	}

	radc_var radc_var::operator /(const radc_var & rk)
	{
		d_assert (i_type == rk.i_type);

		radc_var v(NULL);

		switch (i_type)
		{
		case radc_enum::type_real:
			v.setReal(r_value / rk.r_value);
			break;
		}

		return v;
	}


	// varlist
	radc_varlist::radc_varlist()
	{
	}

	radc_varlist::~radc_varlist()
	{
		clear();
	}

	void radc_varlist::clear()
	{
		for (int i = 0; i < varlist.Size(); ++i)
		{
			varlist[i].clear();
		}

		varlist.Clear();
	}

	int radc_varlist::size() const
	{
		return varlist.Size();
	}

	void radc_varlist::push(radc_real v)
	{
		d_assert (varlist.Size() < varlist.Capacity());

		radc_var p(NULL);

		p.setReal(v);

		varlist.PushBack(p);
	}

	void radc_varlist::push(const char * v)
	{
		d_assert (varlist.Size() < varlist.Capacity());

		radc_var p(NULL);

		p.setString(v);

		varlist.PushBack(p);
	}

	void radc_varlist::push(void * v)
	{
		d_assert (varlist.Size() < varlist.Capacity());

		radc_var p(NULL);

		p.setPtr(v);

		varlist.PushBack(p);
	}

	void radc_varlist::push(const radc_var & v)
	{
		d_assert (varlist.Size() < varlist.Capacity());

		varlist.PushBack(v);
	}

	radc_real radc_varlist::getReal(int index) const
	{
		d_assert (index < varlist.Size());

		if (index < varlist.Size())
		{
			return varlist[index].getReal();
		}

		return 0;
	}

	const char * radc_varlist::getString(int index) const
	{
		d_assert (index < varlist.Size());

		if (index < varlist.Size())
		{
			return varlist[index].getString();
		}

		return 0;
	}

	void * radc_varlist::getPtr(int index) const
	{
		d_assert (index < varlist.Size());

		if (index < varlist.Size())
		{
			return varlist[index].getPtr();
		}

		return NULL;
	}

	int radc_varlist::getType(int index) const
	{
		d_assert (index < varlist.Size());

		return varlist[index].getType();
	}
}
