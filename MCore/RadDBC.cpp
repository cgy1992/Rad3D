#include "RadDBC.h"

namespace Rad {

	ImplementRTTI(rdbc::packet, IObject);

	ImplementRTTI(rdbc::pk_logon, rdbc::packet);
	DF_PROPERTY_BEGIN(rdbc::pk_logon)
		DF_PROPERTY(rdbc::pk_logon, username, "", "", PT_String)
		DF_PROPERTY(rdbc::pk_logon, password, "", "", PT_String)
	DF_PROPERTY_END()

	ImplementRTTI(rdbc::pk_heartbeat, rdbc::packet)

	ImplementRTTI(rdbc::pk_auth, rdbc::packet);
	DF_PROPERTY_BEGIN(rdbc::pk_auth)
		DF_PROPERTY(rdbc::pk_auth, M, "", "", PT_Int)
		DF_PROPERTY(rdbc::pk_auth, K, "", "", PT_FixedString)
	DF_PROPERTY_END()

	ImplementRTTI(rdbc::pk_query, rdbc::packet);
	DF_PROPERTY_BEGIN(rdbc::pk_query)
		DF_PROPERTY(rdbc::pk_query, nodata, "", "", PT_Int)
		DF_PROPERTY(rdbc::pk_query, rsql, "", "", PT_String)
	DF_PROPERTY_END()
	void rdbc::pk_query::Serialize(Serializer & sl)
	{
		IObject::Serialize(sl);

		if (sl.IsOut())
		{
			OSerializer & OS = (OSerializer &)sl;
			if (obj != NULL)
			{
				OS << obj->GetRTTI()->TypeId();
				obj->Serialize(OS);
			}
			else
			{
				OS << (int)0;
			}
		}
		else
		{
			ISerializer & IS = (ISerializer &)sl;

			int typeId = 0;
			IS >> typeId;

			if (typeId != 0)
			{
				obj = Root::Instance()->NEW_OBJECT(typeId);
				if (obj != NULL)
				{
					obj->Serialize(IS);
				}
			}
			else
			{
				obj = NULL;
			}
		}
	}

	ImplementRTTI(rdbc::pk_object, rdbc::packet);
	void rdbc::pk_object::Serialize(Serializer & sl)
	{
		if (sl.IsOut())
		{
			OSerializer & OS = (OSerializer &)sl;

			OS << obj->GetRTTI()->TypeId();
			obj->Serialize(OS);
		}
		else
		{
			ISerializer & IS = (ISerializer &)sl;

			int typeId = 0;
			IS >> typeId;

			obj = Root::Instance()->NEW_OBJECT(typeId);
			if (obj != NULL)
			{
				obj->Serialize(IS);
			}
		}
	}

	ImplementRTTI(rdbc::pk_result, rdbc::packet);
	DF_PROPERTY_BEGIN(rdbc::pk_result)
		DF_PROPERTY(rdbc::pk_result, ret, "", "", PT_Int)
		DF_PROPERTY(rdbc::pk_result, error, "", "", PT_String)
	DF_PROPERTY_END()
}