#include "MShape.h"

namespace Rad {

	ImplementRootRTTI(Shape);

	void Shape::OnAttach(Node * owner)
	{
		d_assert (mOwner == NULL);

		mOwner = owner;
	}

}