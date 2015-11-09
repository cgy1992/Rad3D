/*
*	PS_Mesh
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "ParticleFX.h"
#include "PSEmitterPoint.h"
#include "MMeshManager.h"

namespace Rad {

	class FX_ENTRY PS_EmitterMesh : public PS_EmitterPoint
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(PS_EmitterPoint);

	public:
		String mFilename;
		Float3 mScale;
		bool mRandom;

	public:
		PS_EmitterMesh();
		virtual ~PS_EmitterMesh();

		void 
			SetFilename(const String & filename);
		const String & 
			GetFilename() { return mFilename; }

		virtual void 
			InitParticle(Particle * p);

		virtual void 
			OnPropertyChanged(const Property * p);

	protected:
		void 
			_randomOnMeshVertex(Float3 & position, Float3 & direction);
		void 
			_randomDirection(Float3 & dir, const Float3 & n, const Float3 & common);

	protected:
		MeshSourcePtr mMesh;
		int mIndex;
	};

}