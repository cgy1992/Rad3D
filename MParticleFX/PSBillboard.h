/*
*	PSBillboardSet
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PSTypes.h"

namespace Rad {

	class PS_BillboardSet;

	class FX_ENTRY PS_Billboard : public RenderObject
	{
	public:
		PS_Billboard();
		virtual ~PS_Billboard();

		void 
			Init(PS_BillboardSet * parent);

		virtual void 
			_getWorldPosition(Float3 & pos);
		virtual void 
			_getWorldBound(Aabb & bound);
		virtual void 
			_getWorldTM(Mat4 & form);

		void 
			_allocBuffer(int quota);
		void 
			_updateBuffer();
		void 
			_getBillboardXYAxis(Float3 & xAxis, Float3 & yAxis, const Float3 & pos, const Float3 & dir);

		void
			_update();

	protected:
		PS_BillboardSet * mParent;
		Float3 mCamPos, mCamXAxis, mCamYAxis, mCamZAxis;
		Float3 mCommonDir, mCommonUp;
	};

}