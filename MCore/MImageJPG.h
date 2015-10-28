#pragma once

#include "MImage.h"

namespace Rad {

	M_ENTRY bool 
		JPG_Test(DataStreamPtr stream);
	
	// Some format not support
	//
	M_ENTRY bool
		JPG_Load(Image & image, DataStreamPtr stream);

}