/*
*	Vertex Animation
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MTypes.h"

namespace Rad {

	class Mesh;
	class AnimationController;

	class M_ENTRY VertexAnimation
	{
		DECLARE_ALLOC();

	public:
		struct KeyFrame {
			float time;

			Float3 * positions;
			Float3 * normals;

			KeyFrame() : time(0), positions(NULL), normals(NULL) { }
			~KeyFrame() {}
		};

	public:
		VertexAnimation(int meshId, int vertexCount);
		~VertexAnimation();

		void 
			Update(Mesh * mesh, AnimationController * controller);

		KeyFrame *
			CreateKeyFrame(float time);
		void
			RemoveKeyFrame(int index);
		int 
			GetFrameCount();
		KeyFrame * 
			GetKeyFrame(int index);
		
		int
			GetMeshId() { return mMeshId; }
		int
			GetVertexCount() { return mVertexCount; }

	protected:
		int mMeshId;
		int mVertexCount;
		Array<KeyFrame> mFrames;
	};

}