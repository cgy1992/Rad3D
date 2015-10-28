/*
*	GrassManager
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MSingleton.h"
#include "MGrass.h"
#include "MGrassSection.h"

namespace Rad {

	class M_ENTRY GrassManager : public Singleton<GrassManager>
	{
		enum {
			K_WAVE_GRID_ROWS = 32,
		};

	public:
		GrassManager();
		~GrassManager();

		void 
			SetShaderFX(ShaderFX * fx);
		ShaderFX * 
			GetShaderFX();

		void 
			_updateWave(float elapsedTime);
		float 
			_sampleWave(float x, float z);
		float 
			_sampleWaveEx(int i, int j);
		void 
			PushWave(float x, float z, float strength);

		void
			_getAllBlocks(Array<GrassBlock *> & arr);
		void
			_getActiveBlocks(Array<GrassBlock *> & arr);
		GrassSection *
			_getCurrentSection();

	protected:
		ShaderFX * mShaderFX;

		Field<float, K_WAVE_GRID_ROWS * K_WAVE_GRID_ROWS> mWaveGrid;
		float mWaveGridX, mWaveGridZ;
		int mWaveLastX, mWaveLastZ;
	};

}
