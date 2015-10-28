/*
*	PS_MeshSet
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PSTypes.h"
#include "PSMesh.h"
#include "PSShader.h"

namespace Rad {

	class FX_ENTRY PS_MeshSet : public PS_Set
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(PS_Set);

	protected:
		String mMeshName;

	public:
		PS_MeshSet();
		virtual ~PS_MeshSet();

		virtual void
			OnPropertyChanged(const Property * p);

		virtual void 
			AllocQuota(int quota);
		virtual void 
			AddRenderQueue(RenderQueue * rq);

		virtual void
			Update(float elapsedTime);

		void 
			SetMesh(const String & name);
		const String & 
			GetMesh() const { return mMeshName; }
		MeshSourcePtr
			_getSource() { return mSource; }

	protected:
		virtual Particle *	
			_quest_particle();
		virtual void		
			_free_particle(Particle * p);
		virtual void 
			_clear_particle();

	protected:
		MeshSourcePtr mSource;
	};
}