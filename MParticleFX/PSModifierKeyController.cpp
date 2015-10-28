#include "PSModifierKeyController.h"

namespace Rad {

	ImplementRTTI(PS_ModifierKeyController, PS_Modifier);

	PS_ModifierKeyController::PS_ModifierKeyController()
	{
	}

	PS_ModifierKeyController::~PS_ModifierKeyController()
	{
	}

	void PS_ModifierKeyController::Serialize(Serializer & sl)
	{
		PS_Modifier::Serialize(sl);

		IKeyController * pKeyController = GetKeyController();

		if (sl.IsOut())
		{
			OSerializer & OS = (OSerializer &)sl;

			OS << 0x1001;
			pKeyController->Serialize(sl);

			OS << 0;
		}
		else
		{
			ISerializer & IS = (ISerializer &)sl;

			int ckId = 0;
			while (IS.Read(&ckId, sizeof(int)) && ckId != 0)
			{
				if (ckId == 0x1001)
				{
					pKeyController->Serialize(sl);
				}
			}
		}
	}

}