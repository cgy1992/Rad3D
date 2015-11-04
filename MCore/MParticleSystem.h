/*
*	ParticleSystem
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PSSet.h"
#include "PSEmitter.h"
#include "PSModifier.h"
#include "PSSource.h"

namespace Rad {

	class M_ENTRY ParticleSystem : public Node
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(Node);

	protected:
		String mFilename;

	public:
		ParticleSystem();
		virtual ~ParticleSystem();

		virtual void 
			OnPropertyChanged(const Property * p);

		void 
			SetFilename(const String & filename);
		const String &
			GetFilename() { return mFilename; }

		void
			SetSource(PS_SourcePtr ps);
		PS_SourcePtr
			GetSource() { return mSource; }

		void		
			SetEnable(bool enable);
		bool
			IsEnable() { return mEnable; }

		void
			Replay();

		virtual void
			Update(float elapsedTime);
		virtual void 
			AddRenderQueue(RenderQueue * rq);

		PS_Set *	
			NewSet(const char * type);
		PS_Set *
			CopySet(PS_Set * p);
		void		
			DeleteSet(int index);
		void		
			DeleteSet(PS_Set * p);
		PS_Set *	
			GetSet(int index);
		int			
			GetSetCount();
		bool
			IsSetValid(PS_Set * p);

		void
			SerializeTempalte(Serializer & sl);

		virtual void 
			Load(float priority);
		virtual void 
			Unload();

		virtual IObject *
			Clone();

	protected:
		void 
			OnParticleLoaded();
		void 
			OnParticleUnload();
		
	protected:
		PS_SourcePtr mSource;
		bool mEnable;
	
		FixedArray<PS_Set *, 8> mSetArray;

		cListener0<ParticleSystem> L_ParticleLoaded;
		cListener0<ParticleSystem> L_ParticleUnload;
	};

}