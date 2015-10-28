/*
*	NodeDisplayer
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MNode.h"
#include "MBillboard.h"

namespace Rad {

	class M_ENTRY NodeDisplayer : public IComponent
	{
		DECLARE_RTTI();

	public:
		NodeDisplayer(const String & filename);
		virtual ~NodeDisplayer();

		virtual bool
			Serializable() { return false; }

		virtual void
			Attach(ComponentOwner * owner);
		virtual void
			Detach();
		virtual int
			Update(float elapsedTime) { return 0; }

	protected:
		String mFilename;
		Billboard * mBillboard;
	};

}