#include "RadDBScript.h"
#include "RadDBSql.h"
#include "RadDBTable.h"

namespace Rad {

	bool rdb_property(IObject * obj, rdb_value * result, const rdb_vlist * args)
	{
		if (obj != NULL && rdb_check_args1(args, rdb_value::TYPE_STRING))
		{
			const char * name = args->At(0).AsString();
			const Property * prop = obj->GetProperty(name);

			if (prop != NULL)
			{
				rsql_property::get_value(*result, prop, obj);
			}
		}
		
		return result->Type() != rdb_value::TYPE_VOID;
	}

	bool rdb_typeof(IObject * obj, rdb_value * result, const rdb_vlist * args)
	{
		if (obj != NULL && rdb_check_args1(args, rdb_value::TYPE_STRING))
		{
			const char * classname = args->At(0).AsString();

			if (strcmp(obj->GetRTTI()->Name(), classname) == 0)
			{
				result->SetInt(1);
			}
			else
			{
				result->SetInt(0);
			}
		}

		return result->Type() != rdb_value::TYPE_VOID;
	}

	bool rdb_kindof(IObject * obj, rdb_value * result, const rdb_vlist * args)
	{
		if (obj != NULL && rdb_check_args1(args, rdb_value::TYPE_STRING))
		{
			const char * classname = args->At(0).AsString();

			result->SetInt(0);

			const RTTI_INFO * p = obj->GetRTTI();
			while (p != NULL)
			{
				if (strcmp(p->Name(), classname) == 0)
				{
					result->SetInt(1);
					break;
				}

				p = p->Base();
			}
		}

		return result->Type() != rdb_value::TYPE_VOID;
	}

	bool rdb_index(IObject * obj, rdb_value * result, const rdb_vlist * args)
	{
		if (obj != NULL)
			return false;

		if (rdb_check_args1(args, rdb_value::TYPE_STRING) && args->Size() == 3)
		{
			rdb_table * table = (rdb_table *)rsql_engine::Instance()->i_table;
		
			const char * name = args->At(0).AsString();
			rdb_value v_min = args->At(1);
			rdb_value v_max = args->At(2);

			int id = Math::Crc32(name);

			for (int i = 0; i < table->i_primaryks.Size(); ++i)
			{
				const Property * p = table->i_primaryks[i];
				if (p->id != id)
					continue;

				if (rsql_engine::Instance()->option_indexed[i])
				{
					rsql_engine::Instance()->set_error("index '%s' set multiple", name);
					return false;
				}

				if (p != NULL)
				{
					int type = rdb_value::TYPE_VOID;

					switch (p->type)
					{
					case PT_Int:
						type = (p->flag & PF_Unsigned) ? rdb_value::TYPE_UINT : rdb_value::TYPE_INT;
						break;

					case PT_Float:
						type = rdb_value::TYPE_REAL;
						break;

					case PT_FixedString:
						type = rdb_value::TYPE_STRING;
						break;

					case PT_String:
						type = rdb_value::TYPE_STRING;
						break;
					}

					d_assert (type != rdb_value::TYPE_VOID);

					if (v_min.Type() != rdb_value::TYPE_NULL && v_min.Type() != type)
					{
						v_min = v_min.Type() > type ? v_min.DownType(type) : v_min.UpType(type);
					}

					if (v_max.Type() != rdb_value::TYPE_NULL && v_max.Type() != type)
					{
						v_max = v_max.Type() > type ? v_max.DownType(type) : v_max.UpType(type);
					}

					if (v_min.Type() == rdb_value::TYPE_VOID || v_max.Type() == rdb_value::TYPE_VOID)
					{
						rsql_engine::Instance()->set_error("index '%s' type error", name);
						return false;
					}

					rsql_engine::Instance()->option_vmin[i].Clear();
					rsql_engine::Instance()->option_vmax[i].Clear();

					rsql_engine::Instance()->option_indexed[i] = true;
					rsql_engine::Instance()->option_vmin[i] = v_min;
					rsql_engine::Instance()->option_vmax[i] = v_max;

					return true;
				}
			}
		}

		return false;
	}

	bool rdb_count(IObject * obj, rdb_value * result, const rdb_vlist * args)
	{
		if (obj != NULL)
			return false;

		if (rdb_check_args1(args, rdb_value::TYPE_INT))
		{
			rsql_engine::Instance()->option_count = (int)args->At(0).AsInt();

			return true;
		}

		return false;
	}

	bool rdb_offset(IObject * obj, rdb_value * result, const rdb_vlist * args)
	{
		if (obj != NULL)
			return false;

		if (rdb_check_args1(args, rdb_value::TYPE_INT))
		{
			int offset = (int)args->At(0).AsInt();
			if (offset < 0)
			{
				rsql_engine::Instance()->set_error("offset must be greater 0.");
				return false;
			}

			rsql_engine::Instance()->option_offset = offset;

			return true;
		}

		return false;
	}

	bool rdb_orderby(IObject * obj, rdb_value * result, const rdb_vlist * args)
	{
		if (obj != NULL)
			return false;

		if (args->Size() > 0 && args->Size() < 4 &&
			rsql_engine::Instance()->option_orderby.Size() == 0)
		{
			for (int i = 0; i < args->Size(); ++i)
			{
				rdb_table * table = (rdb_table *)rsql_engine::Instance()->i_table;
				const char * name = args->At(0).AsString();

				const Property * p = table->i_object->GetProperty(name);
				if (p == NULL)
				{
					rsql_engine::Instance()->set_error("poperty '%s' not exist.", name);
					return false;
				}

				int type = rdb_value::TYPE_VOID;
				switch (p->type)
				{
				case PT_Int:
					type = (p->flag & PF_Unsigned) ? rdb_value::TYPE_UINT : rdb_value::TYPE_INT;
					break;

				case PT_Float:
					type = rdb_value::TYPE_REAL;
					break;

				case PT_FixedString:
					type = rdb_value::TYPE_STRING;
					break;

				case PT_String:
					type = rdb_value::TYPE_STRING;
					break;
				}

				if (type == rdb_value::TYPE_VOID)
				{
					rsql_engine::Instance()->set_error("order by property '%s' must be int, float or string.", name);
					return false;
				}

				rsql_engine::Instance()->option_orderby.PushBack(p);
			}
			
			return true;
		}

		return false;
	}
}