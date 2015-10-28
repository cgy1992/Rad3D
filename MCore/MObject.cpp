#include "MObject.h"
#include "MSerializer.h"

namespace Rad {

	ImplementRootRTTI(IObject);

	IObject::IObject()
		: mVersion(1)
	{
	}

	IObject::~IObject()
	{
	}

	const void * IObject::GetPropertyData(const Property * p) const
	{
		return (const char *)this + p->offset;
	}

	void IObject::SetPropertyData(const Property * p, const void * data)
	{
		char * addr = (char *)(this) + p->offset;

		if (p->type == PT_String)
		{
			*(String*)addr = *(String*)data;
		}
		else if (p->type == PT_UString)
		{
			*(UString*)addr = *(UString*)data;
		}
		else if (p->type < PT_UserData)
		{
			memcpy(addr, data, p->size);
		}

		OnPropertyChanged(p);

		E_PropertyChanged(this, p);
	}

	void IObject::Copy(const IObject * from)
	{
		d_assert (GetRTTI() == from->GetRTTI());

		int propSize = from->GetPropertySize();
		for (int j = 0; j < propSize; ++j)
		{
			const Property * p = from->GetProperty(j);
			const void * data = from->GetPropertyData(p);

			if (p->type < PT_UserData)
			{
				SetPropertyData(p, data);
			}
		}
	}

	void IObject::Serialize(Serializer & sl)
	{
		d_assert (Serializable());

		if (sl.IsOut())
		{
			OSerializer & OS = (OSerializer &)sl;

			OS << mVersion;

			int count = GetPropertySize();
			for (int i = 0; i < count; ++i)
			{
				const Property * p = GetProperty(i);

#ifdef M_DEBUG
				d_assert (p == GetPropertyById(p->id));
#endif

				if (p->type >= PT_UserData || p->size == 0)
					continue;

				const void * data = GetPropertyData(p);
				int dataLen = p->size;

				if (p->type == PT_String)
				{
					const char * str = p->AsString(data);

					data = str;
					dataLen = strlen(str) + 1;
				}
				else if (p->type == PT_UString)
				{
					const uchar_t * str = p->AsUString(data);

					data = str;
					dataLen = u_strlen(str) + 1;
					dataLen *= 2;
				}

				d_assert (dataLen < PROPERTY_MAX_SIZE);

				OS << p->id;
				OS << dataLen;

				OS.Write(data, dataLen);
			}

			// end
			OS << 0;
		}
		else
		{
			ISerializer & IS = (ISerializer &)sl;

			IS >> mVersion;

			int id = 0;
			int dataLen = 0;
			char data[PROPERTY_MAX_SIZE];

			while (IS.Read(&id, sizeof(int)) && id != 0)
			{
				IS >> dataLen;

				const Property * p = GetPropertyById(id);
				if (p)
				{
					d_assert (dataLen > 0 && dataLen < PROPERTY_MAX_SIZE);

					IS.Read(data, dataLen);

					if (p->type == PT_String)
					{
						String str = (char *)data;
						SetPropertyData(p, &str);
					}
					else if (p->type == PT_UString)
					{
						UString str = (uchar_t *)data;
						SetPropertyData(p, &str);
					}
					else
					{
						d_assert (dataLen == p->size);

						SetPropertyData(p, data);
					}
				}
				else
				{
					IS.Skip(dataLen);
				}
			}
		}
	}
	
	void IObject::Serialize(rml_node * root, bool isOut)
	{
		int propSize = GetPropertySize();

		if (!isOut)
		{
			rml_node * node = NULL;
			for (int i = 0; i < propSize; ++i)
			{
				const Property * prop = GetProperty(i);
				if (prop->type >= PT_UserData)
					continue;

				rml_node * node = root->first_node(prop->name);
				if (node == NULL)
					continue;

				const char * str = node->value();
				switch (prop->type)
				{
				case PT_Bool:
					{
						bool val = atoi(str) ? true : false;
						SetPropertyData(prop, &val);
					}
					break;

				case PT_Int:
					{
						int val = atoi(str);
						SetPropertyData(prop, &val);
					}
					break;

				case PT_Int2:
					{
						Int2 val;
						str = str_getint(val.x, str);
						str = str_getint(val.y, str);
						SetPropertyData(prop, &val);
					}
					break;

				case PT_Int3:
					{
						Int3 val;
						str = str_getint(val.x, str);
						str = str_getint(val.y, str);
						str = str_getint(val.z, str);
						SetPropertyData(prop, &val);
					}
					break;

				case PT_Int4:
					{
						Int4 val;
						str = str_getint(val.x, str);
						str = str_getint(val.y, str);
						str = str_getint(val.z, str);
						str = str_getint(val.w, str);
						SetPropertyData(prop, &val);
					}
					break;

				case PT_Float:
					{
						float val = (float)atof(str);
						SetPropertyData(prop, &val);
					}
					break;

				case PT_Float2:
					{
						Float2 val;
						str = str_getfloat(val.x, str);
						str = str_getfloat(val.y, str);
						SetPropertyData(prop, &val);
					}
					break;

				case PT_Float3:
					{
						Float3 val;
						str = str_getfloat(val.x, str);
						str = str_getfloat(val.y, str);
						str = str_getfloat(val.z, str);
						SetPropertyData(prop, &val);
					}
					break;
				case PT_Float4:
					{
						Float4 val;
						str = str_getfloat(val.x, str);
						str = str_getfloat(val.y, str);
						str = str_getfloat(val.z, str);
						str = str_getfloat(val.w, str);
						SetPropertyData(prop, &val);
					}
					break;

				case PT_FixedString:
					{
						const char * val = str;
						SetPropertyData(prop, val);
					}
					break;

				case PT_String:
					{
						String val = str;
						val.Trim();
						SetPropertyData(prop, &val);
					}
					break;

				case PT_UString:
					{
						String val = str;
						val.Trim();

						UString uval = val.c_wstr();
						SetPropertyData(prop, &uval);
					}
				}
			}
		}
		else
		{
			for (int i = 0; i < propSize; ++i)
			{
				const Property * prop = GetProperty(i);
				if (prop->type >= PT_UserData)
					continue;

				const void * data = GetPropertyData(prop);
				switch (prop->type)
				{
				case PT_Bool:
					{
						bool val = prop->AsBool(data);
						root->append(prop->name, val ? "1" : "0");
					}
					break;

				case PT_Int:
					{
						int val = prop->AsInt(data);
						root->append(prop->name, i2str(val).c_str());
					}
					break;

				case PT_Int2:
					{
						String str = "";
						Int2 val = prop->AsInt2(data);
						str += i2str(val.x) + " "
							+= i2str(val.y);
						root->append(prop->name, str.c_str());
					}
					break;

				case PT_Int3:
					{
						String str = "";
						Int3 val = prop->AsInt3(data);
						str += i2str(val.x) + " "
							+= i2str(val.y) + " "
							+= i2str(val.z);
						root->append(prop->name, str.c_str());
					}
					break;

				case PT_Int4:
					{
						String str = "";
						Int4 val = prop->AsInt4(data);
						str += i2str(val.x) + " "
							+= i2str(val.y) + " "
							+= i2str(val.z) + " "
							+= i2str(val.w);
						root->append(prop->name, str.c_str());
					}
					break;

				case PT_Float:
					{
						float val = prop->AsFloat(data);
						root->append(prop->name, f2str(val, true).c_str());
					}
					break;

				case PT_Float2:
					{
						String str = "";
						Float2 val = prop->AsFloat2(data);
						str += f2str(val.x, true) + " "
							+= f2str(val.y, true);
						root->append(prop->name, str.c_str());
					}
					break;

				case PT_Float3:
					{
						String str = "";
						Float3 val = prop->AsFloat3(data);
						str += f2str(val.x, true) + " "
							+= f2str(val.y, true) + " "
							+= f2str(val.z, true);
						root->append(prop->name, str.c_str());
					}
					break;
				case PT_Float4:
					{
						String str = "";
						Float4 val = prop->AsFloat4(data);
						str += f2str(val.x, true) + " "
							+= f2str(val.y, true) + " "
							+= f2str(val.z, true) + " "
							+= f2str(val.w, true);
						root->append(prop->name, str.c_str());
					}
					break;

				case PT_FixedString:
					{
						const char * val = prop->AsFixedString(data);
						if (strcmp(val, "") != 0)
						{
							root->append(prop->name, val);
						}
					}
					break;

				case PT_String:
					{
						String val = prop->AsString(data);
						if (val != "")
						{
							if (strcmp(prop->editorName, "PT_Script") == 0)
							{
								val = "\n" + val;
							}

							root->append(prop->name, val.c_str());
						}
					}
					break;

				case PT_UString:
					{
						String val;
						val.FromUnicode(prop->AsUString(data));
						if (val != "")
						{
							root->append(prop->name, val.c_str());
						}
					}
					break;
				}
			}
		}
	}

	//
	ImplementRootRTTI(IObjectFactory);

}