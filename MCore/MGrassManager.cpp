#include "MGrassManager.h"
#include "MWorld.h"

#define D_WAVE_GRID_SIZE (0.5f * UNIT_METRES)

namespace Rad {

	ImplementSingleton(GrassManager);

	GrassManager::GrassManager()
	{
		mShaderFX = ShaderFXManager::Instance()->Load("Grass", "Shader/MGrass.mfx");

		memset(&mWaveGrid[0], 0, sizeof(mWaveGrid));
		mWaveGridX = mWaveGridZ = 0;
		mWaveLastX = mWaveLastZ = 0;

		GrassTemplate::InitVertex();
	}

	GrassManager::~GrassManager()
	{
	}

	void GrassManager::SetShaderFX(ShaderFX * fx)
	{
		mShaderFX = fx;
	}

	ShaderFX * GrassManager::GetShaderFX()
	{
		return mShaderFX;
	}

	void GrassManager::_updateWave(float elapsedTime)
	{
		float fadeSpeed = elapsedTime * 3.0f;

		for (int i = 0; i < K_WAVE_GRID_ROWS; ++i)
		{
			for (int j = 0; j < K_WAVE_GRID_ROWS; ++j)
			{
				int x = j, y = i;

				float s = mWaveGrid[y * K_WAVE_GRID_ROWS + x];

				s = Max(0.0f, s - fadeSpeed);

				mWaveGrid[y * K_WAVE_GRID_ROWS + x] = s;
			}
		}

		Float3 p = World::Instance()->MainCamera()->GetWorldPosition();
		Float3 d = World::Instance()->MainCamera()->GetWorldRotation().GetDirVector();

		p += d * (8 * D_WAVE_GRID_SIZE);

		int & last_x = mWaveLastX;
		int & last_z = mWaveLastZ;
		int cur_x = (int)(p.x / D_WAVE_GRID_SIZE);
		int cur_z = (int)(p.z / D_WAVE_GRID_SIZE);

		if (cur_x != last_x || cur_x != cur_z)
		{
			int dx = cur_x - last_x;
			int dy = cur_z - last_z;

			for (int i = 0; i < K_WAVE_GRID_ROWS; ++i)
			{
				for (int j = 0; j < K_WAVE_GRID_ROWS; ++j)
				{
					int x = j + dx, y = i + dy;

					if (x >= 0 && x < K_WAVE_GRID_ROWS &&
						y >= 0 && y < K_WAVE_GRID_ROWS)
						mWaveGrid[j + K_WAVE_GRID_ROWS + i] = mWaveGrid[y + K_WAVE_GRID_ROWS + x];
					else
						mWaveGrid[j + K_WAVE_GRID_ROWS + i] = 0;
				}
			}

			last_x = cur_x; last_z = cur_z;
		}

		mWaveGridX = cur_x * D_WAVE_GRID_SIZE - K_WAVE_GRID_ROWS / 2 * D_WAVE_GRID_SIZE;
		mWaveGridZ = cur_z * D_WAVE_GRID_SIZE - K_WAVE_GRID_ROWS / 2 * D_WAVE_GRID_SIZE;
	}

	float GrassManager::_sampleWave(float fx, float fz)
	{
		fx -= mWaveGridX;
		fz -= mWaveGridZ;

		int x = (int)(fx / D_WAVE_GRID_SIZE);
		int y = (int)(fz / D_WAVE_GRID_SIZE);

		if (x < 0 || x >= K_WAVE_GRID_ROWS ||
			y < 0 || y >= K_WAVE_GRID_ROWS)
			return 0;

		float d = _sampleWaveEx(x, y) * 0.4f;
		d += _sampleWaveEx(x - 1, y) * 0.15f;
		d += _sampleWaveEx(x + 1, y) * 0.15f;
		d += _sampleWaveEx(x, y - 1) * 0.15f;
		d += _sampleWaveEx(x, y + 1) * 0.15f;

		return mWaveGrid[y * K_WAVE_GRID_ROWS + x];
	}

	float GrassManager::_sampleWaveEx(int x, int y)
	{
		if (x < 0 || x >= K_WAVE_GRID_ROWS ||
			y < 0 || y >= K_WAVE_GRID_ROWS)
			return 0;

		return mWaveGrid[y * K_WAVE_GRID_ROWS + x];
	}


	void GrassManager::PushWave(float fx, float fz, float strength)
	{
		fx -= mWaveGridX;
		fz -= mWaveGridZ;

		int x = (int)(fx / D_WAVE_GRID_SIZE);
		int y = (int)(fz / D_WAVE_GRID_SIZE);

		if (x < 0 || x >= K_WAVE_GRID_ROWS ||
			y < 0 || y >= K_WAVE_GRID_ROWS)
			return ;

		mWaveGrid[y * K_WAVE_GRID_ROWS + x] = Max(mWaveGrid[y * K_WAVE_GRID_ROWS + x], strength);

		if (x - 1 > 0)
			mWaveGrid[y * K_WAVE_GRID_ROWS + x - 1] = Max(mWaveGrid[y * K_WAVE_GRID_ROWS + x - 1], strength * 0.5f);

		if (x + 1 < K_WAVE_GRID_ROWS)
			mWaveGrid[y * K_WAVE_GRID_ROWS + x + 1] = Max(mWaveGrid[y * K_WAVE_GRID_ROWS + x + 1], strength * 0.5f);

		if (y - 1 > 0)
			mWaveGrid[(y - 1) * K_WAVE_GRID_ROWS + x] = Max(mWaveGrid[(y - 1) * K_WAVE_GRID_ROWS + x], strength * 0.5f);

		if (y + 1 < K_WAVE_GRID_ROWS)
			mWaveGrid[(y + 1) * K_WAVE_GRID_ROWS + x] = Max(mWaveGrid[(y + 1) * K_WAVE_GRID_ROWS + x], strength * 0.5f);
	}

	void GrassManager::_getAllBlocks(Array<GrassBlock *> & arr)
	{
		const World::Info * wi = World::Instance()->GetInfo();

		for (int j = 0; j < wi->SectionCount.y; ++j)
		{
			for (int i = 0; i < wi->SectionCount.x; ++i)
			{
				WorldSection * ws = World::Instance()->GetSection(i, j);
				GrassSection * section = ws->GetDataT<GrassSection>();
				if (section != NULL)
				{
					int count = section->_getBlockCount().x * section->_getBlockCount().y;
					for (int k = 0; k < count; ++k)
					{
						arr.PushBack(section->_getBlock(k));
					}
				}
			}
		}
	}

	void GrassManager::_getActiveBlocks(Array<GrassBlock *> & arr)
	{
		const FixedArray<WorldSection *, 9> & ws = World::Instance()->GetActiveSections();
		for (int i = 0; i < ws.Size(); ++i)
		{
			GrassSection * section = ws[i]->GetDataT<GrassSection>();
			if (section != NULL)
			{
				int count = section->_getBlockCount().x * section->_getBlockCount().y;
				for (int k = 0; k < count; ++k)
				{
					arr.PushBack(section->_getBlock(k));
				}
			}
		}
	}

	GrassSection * GrassManager::_getCurrentSection()
	{
		WorldSection * ws = World::Instance()->GetCurrentSection();
		if (ws != NULL)
		{
			return ws->GetDataT<GrassSection>();
		}

		return NULL;
	}
}

