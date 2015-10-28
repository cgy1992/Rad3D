/*
*	Environment
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MSingleton.h"
#include "MEvParam.h"
#include "MSky.h"
#include "MSun.h"
#include "MRain.h"
#include "MSnow.h"

namespace Rad {

#define ENV_FILE_MAGIC MAKE_DWORD('E', 'V', 'N', '0')

	struct eEnvFlag
	{
		enum {
			LIGHT	= 0x0001,
			FOG		= 0x0002,
			SKY		= 0x0004,
			SUN		= 0x0008,
			SNOW	= 0x0010,
			RAIN	= 0x0020,

			ALL	= 0x7FFFFFFF
		};
	};

	class M_ENTRY Environment : public Singleton<Environment>
	{
	public:
		Environment();
		~Environment();

		void 
			Load(const String & filename);
		void 
			Save(const String & filename);

		void
			SetFlags(int flags);
		int
			GetFlags() { return mFlags; }

		Sky * 
			GetSky() { return mSky; }
		Sun * 
			GetSun() { return mSun; }
		Snow * 
			GetSnow() { return mSnow; }
		Rain * 
			GetRain() { return mRain; }

		EvKeyFrame * 
			GetEvKeyFrame() { return &mKeyFrame; }
		const EvParam * 
			GetEvParam() { return &mParam; }

		void 
			Update(float frameTime);

	protected:
		Float3
			_makeSunDir(float y, float p, float r);

		void
			OnRenderSolid();
		void 
			OnRenderTransparent();

	protected:
		int mFlags;
		Sky * mSky;
		Sun * mSun;
		Rain * mRain;
		Snow * mSnow;

		EvKeyFrame mKeyFrame;
		EvParam mParam;
	};

}
