#include "MEnvironment.h"
#include "MWorld.h"
#include "MResourceManager.h"

namespace Rad {

	ImplementSingleton(Environment);

	Environment::Environment()
		: mFlags(0)
		, mSky(NULL)
		, mSun(NULL)
		, mSnow(NULL)
		, mRain(NULL)
	{
		World::Instance()->E_RenderSolidBegin += new cListener0<Environment>(this, &Environment::OnRenderSolid);
		World::Instance()->E_RenderAlphaEnd += new cListener0<Environment>(this, &Environment::OnRenderTransparent);
	}

	Environment::~Environment()
	{
		safe_delete (mSky);
		safe_delete (mSun);
		safe_delete (mSnow);
		safe_delete (mRain);
	}

	void Environment::SetFlags(int flags)
	{
		mFlags = flags;

		if (mFlags & eEnvFlag::SKY)
		{
			if (mSky == NULL)
				mSky = new Sky;
		}
		else
		{
			safe_delete(mSky);
		}

		if (mFlags & eEnvFlag::SUN)
		{
			if (mSun == NULL)
				mSun = new Sun;
		}
		else
		{
			safe_delete(mSun);
		}

		if (mFlags & eEnvFlag::SNOW)
		{
			if (mSnow == NULL)
				mSnow = new Snow;
		}
		else
		{
			safe_delete(mSnow);
		}

		if (mFlags & eEnvFlag::SUN)
		{
			if (mSun == NULL)
				mSun = new Sun;
		}
		else
		{
			safe_delete(mSun);
		}
	}

	void Environment::Update(float frameTime)
	{
		EvParam p;

		const EvKeyFrame & k = mKeyFrame;

		Float3 camPos = World::Instance()->MainCamera()->GetWorldPosition();
		float farClip = World::Instance()->MainCamera()->GetFarClip() * 0.9f;

		p.SkyLum = k.SkyLum;
		p.SkyU = 0;
		p.SkyV = k.SkyVOff;
		p.SkyTexture = k.SkyTexture;

		p.SunDir = _makeSunDir(0, k.SunPicth, k.SunRoll);
		p.SunPos = camPos - farClip * p.SunDir;
		p.SunSize = k.SunSize;
		p.SunTexture = k.SunTexture;

		p.MainLightDir = p.SunDir;
		p.MainLightAmbient = k.MainLightAmbient * k.MainLightStrength;
		p.MainLightDiffuse = k.MainLightDiffuse * k.MainLightStrength;
		p.MainLightSpecular = k.MainLightSpecular * k.MainLightStrength;

		p.FogStart = k.FogStart;
		p.FogEnd = k.FogEnd;
		p.FogColor = k.FogColor;

		p.WindDir = Float3(k.WindDirX, 0, k.WindDirZ);

		p.GrassWaveSpeed = k.GrassWaveSpeed;
		p.GrassWaveStrength = k.GrassWaveStrength;
		p.GrassVisibleRadius = k.GrassVisibleRadius;

		mParam = p;

		if (mFlags & eEnvFlag::LIGHT)
		{
			World::Instance()->MainLight()->SetDirection(p.MainLightDir);
			World::Instance()->MainLight()->SetAmbient(p.MainLightAmbient);
			World::Instance()->MainLight()->SetDiffuse(p.MainLightDiffuse);
			World::Instance()->MainLight()->SetSpecular(p.MainLightSpecular);
		}
		
		if (mFlags & eEnvFlag::FOG)
		{
			RenderSystem::Instance()->SetFog(p.FogColor, p.FogStart, p.FogEnd);
		}

		if (mSnow)
			mSnow->Update(frameTime);

		if (mRain)
			mRain->Update(frameTime);
	}

	void Environment::Load(const String & filename)
	{
		DataStreamPtr stream = ResourceManager::Instance()->OpenResource(filename);
		if (stream == NULL)
		{
			d_log("!: Environment load failed, '%s'.", filename.c_str());
			return ;
		}

		ISerializerD IS(stream);

		int magic;
		IS >> magic;

		d_assert (magic == ENV_FILE_MAGIC);

		mKeyFrame.Serialize(IS);
	}

	void Environment::Save(const String & filename)
	{
		FILE * fp = fopen(filename.c_str(), "wb");
		if (!fp)
		{
			d_log("!: Environment save failed, '%s'.", filename.c_str());
			return ;
		}

		OSerializerF OS(fp, true);

		OS << (int)(ENV_FILE_MAGIC);
		mKeyFrame.Serialize(OS);
	}

	Float3 Environment::_makeSunDir(float y, float p, float r)
	{
		Mat4 matY, matP, matR;

		y = Math::DegreeToRadian(y);
		p = Math::DegreeToRadian(p);
		r = Math::DegreeToRadian(r);

		matY.MakeRotationY(y);
		matP.MakeRotationX(p);
		matR.MakeRotationZ(r);

		matR *= matY;
		matR *= matP;

		Float3 dir = Float3(-1, 0, 0);
		dir.TransformN(matR);

		return dir;
	}

	void Environment::OnRenderSolid()
	{
		if (mSky)
			mSky->Render();

		if (mSun)
			mSun->Render();
	}

	void Environment::OnRenderTransparent()
	{
		if (mSnow)
			mSnow->Render();

		if (mRain)
			mRain->Render();
	}
}