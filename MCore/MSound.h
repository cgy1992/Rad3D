/*
*	Sound
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MNode.h"
#include "MAudioSystem.h"
#include "MBillboard.h"

namespace Rad {

	class M_ENTRY Sound : public Node
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(Node);

	protected:
		String mFilename;
		float mVolume;
		float mAttenStart;
		float mAttenEnd;

	public:
		Sound();
		virtual ~Sound();

		virtual void
			OnPropertyChanged(const Property * p);

		virtual IObject *
			Clone();

		void 
			SetFilename(const String & filename);
		const String & 
			GetFilename() { return mFilename; }

		void 
			SetSource(SoundSourcePtr source);
		SoundSourcePtr       
			GetSource() { return mSource; }

		void 
			SetVolume(float v);
		float 
			GetVolume() { return mVolume; }

		void 
			SetAttenParam(float start, float end);
		float 
			GetAttenStart() { return mAttenStart; }
		float 
			GetAttenEnd() { return mAttenEnd; }

		int
			GetChannel() { return mChannel; }

		virtual void
			Update(float elapsedTime);

		virtual void 
			Load(float priority);
		virtual void 
			Unload();

	protected:
		void 
			OnSoundLoaded();
		void 
			OnSoundUnload();

		cListener0<Sound> L_SoundLoaded;
		cListener0<Sound> L_SoundUnload;

	protected:
		SoundSourcePtr mSource;
		int mChannel;
	};

}