/*
*	Animation Serializer
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MRenderObject.h"

namespace Rad {

	class Animation;
	class MeshSource;

	class M_ENTRY AnimationSerializer
	{
	public:
		enum {
			K_Magic = MAKE_DWORD('A', 'N', 'I', 'M'),
			K_Version = 0,

			K_Animation_V0 = 0,
			K_Animation_V1 = 1,
			K_Animation_V2 = 2,
		};

	public:
		static void 
			Load(Animation * anim, DataStreamPtr stream);
		static bool 
			Save(Animation * anim, const String & filename);

		static void 
			ReadAnimation(MeshSource * mesh, Animation * anim, DataStreamPtr stream);
		static void 
			ReadAnimationEvent(Animation * anim, DataStreamPtr stream);

		static void
			WriteAnimation(MeshSource * mesh, Animation * anim, OSerializer * stream);
		static void 
			WriteAnimationEvent(Animation * anim, OSerializer * stream);
	};

}