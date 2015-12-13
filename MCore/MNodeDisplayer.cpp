#include "MNodeDisplayer.h"

namespace Rad {

	ImplementRTTI(NodeDisplayer, IComponent);

	NodeDisplayer::NodeDisplayer(const String & filename)
		: mFilename(filename)
		, mBillboard(NULL)
	{
	}

	NodeDisplayer::~NodeDisplayer()
	{
		d_assert (mBillboard == NULL);
	}

	void NodeDisplayer::Attach(ComponentOwner * owner)
	{
		d_assert(KIND_OF(Node, owner));

		IComponent::Attach(owner);

		mBillboard = new Billboard;
		mBillboard->SetSize(Float2(METER_LEN * 0.5f, METER_LEN * 0.5f));
		mBillboard->SetTexture(mFilename);
		mBillboard->GetMaterial()->blendMode = eBlendMode::ALPHA_BLEND;

		((Node *)owner)->Attach(mBillboard);
	}

	void NodeDisplayer::Detach()
	{
		delete mBillboard;

		IComponent::Detach();
	}

}