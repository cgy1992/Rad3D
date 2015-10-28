#include "MAudioSystem.h"
#include "MResourceManager.h"
#include "MAudio.h"
#include "MAudioWAV.h"
#include "MAudioOGG.h"
#include "MVideo.h"
#include "MVideoOGG.h"
#include "MLog.h"
#include "RadC.h"

namespace Rad {

	SoundSource::SoundSource(const String & name, const String & source)
		: ResourceDataStream(name, source)
	{
	}

	SoundSource::~SoundSource()
	{
	}

	void SoundSource::Release()
	{
		AudioSystem::Instance()->_delSource(this);
	}

	//
	ImplementSingleton(AudioSystem);

	AudioSystem::AudioSystem()
	{
		ADO_Reg(WAV_Test, WAV_Load);
		ADO_Reg(OGG_Test, OGG_Load);

		VDO_Reg(OGV_Test, OGV_Load);

		for (int i = 0; i < mGlobalVolumes.Size(); ++i)
		{
			mGlobalVolumes[i] = 0.8f;
		}
	}

	AudioSystem::~AudioSystem()
	{
		VDO_Clear();
		ADO_Clear();

		d_assert (mSourceMap.Empty());
	}

	SoundSourcePtr AudioSystem::_find(Hash2 hash, const String & filename)
	{
		int i = mSourceMap.Find(hash);

		d_assert (i == - 1 || mSourceMap[i].value->GetName() == filename);

		return i != -1 ? mSourceMap[i].value : NULL;
	}

	SoundSourcePtr AudioSystem::LoadSource(const String & filename, float priority)
	{
		Hash2 hash(filename.c_str());

		SoundSourcePtr p = _find(hash, filename);
		if (p == NULL)
		{
			p = new SoundSource(filename, filename);
			p->SetPriority(priority);

			mSourceMap.Insert(hash, p.c_ptr());
		}

		if (priority < 0)
		{
			p->EnsureLoad();
		}
		else
		{
			p->Load();
		}

		return p;
	}

	SoundSourcePtr AudioSystem::GetSource(const String & filename)
	{
		return _find(Hash2(filename.c_str()), filename);
	}

	void AudioSystem::_delSource(SoundSource * p)
	{
		Hash2 hash(p->GetName().c_str());
		int i = mSourceMap.Find(hash);

		d_assert (i != -1);

		delete mSourceMap[i].value;
		mSourceMap.Erase(i);
	}

	int AudioSystem::PlaySound(const String & filename, int category, int flag, int channel)
	{
		if (filename == "")
			return -1;

		DataStreamPtr stream = ResourceManager::Instance()->OpenResource(filename);
		if (stream == NULL)
		{
			d_log("!: file open failed, '%s'", filename.c_str());
			return -1;
		}

		return PlaySound(stream, category, flag, channel);
	}

	int AudioSystem::PlaySound(DataStreamPtr stream, int category, int flag, int channel)
	{
		IAudioPtr ado = ADO_Load(stream);

		if (ado == NULL)
		{
			d_log("!: load audio failed");
			return -1;
		}

		return PlaySound(ado, category, flag, channel);
	}
}
