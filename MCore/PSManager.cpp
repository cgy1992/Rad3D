#include "PSManager.h"
#include "MResourceManager.h"
#include "MWorld.h"
#include "RadC.h"

namespace Rad {

	float d_WorldHeight(float x, float y, float z)
	{
		float h = 0;
		if (Terrain::Instance()->IsValid())
		{
			Terrain::Instance()->GetHeightAt(h, x, z);
		}

		return h;
	}

	bool d_WorldCollide(Float3 * normal, Particle * p, PS_Set * render)
	{
		if (Terrain::Instance()->IsValid())
		{
			Float3 position = p->Position;
			if (render->IsLocalSpace())
			{
				position *= render->GetParent()->GetWorldTM();
			}

			float h = 0;
			if (Terrain::Instance()->GetHeightAt(h, position.x, position.z) && h > position.y)
			{
				Float3 n;
				if (Terrain::Instance()->GetNormalAt(n, position.x, position.z))
				{
					if (normal)
						*normal = n;

					return true;
				}
			}
		}

		return false;
	}

	PS_Manager::WorldHeightFunction PS_Manager::d_get_height = d_WorldHeight;
	PS_Manager::WorldCollideFunction PS_Manager::d_collide = d_WorldCollide;

	//
	ImplementSingleton(PS_Manager);

	PS_Manager::PS_Manager()
	{
		World::Instance()->E_Init += new cListener0<PS_Manager>(this, &PS_Manager::OnInit);
		World::Instance()->E_Shutdown += new cListener0<PS_Manager>(this, &PS_Manager::OnShutdown);
		World::Instance()->E_Update += new cListener1<PS_Manager, float>(this, &PS_Manager::OnUpdate);
	}

	PS_Manager::~PS_Manager()
	{
		d_assert(mSourceMap.Empty() && "All particle system has already delete?");
	}

	void PS_Manager::Play(const char * filename, const Float3 & position, float duration)
	{
		ParticleSystem * p = new ParticleSystem;
		p->SetFilename(filename);
		p->SetPosition(position);
		
		PlayInfo pi = { duration, p };

		mPlayInfos.PushBack(pi);
	}

	PS_SourcePtr PS_Manager::_find(Hash2 hash, const String & name)
	{
		int i = mSourceMap.Find(hash);

		d_assert (i == - 1 || mSourceMap[i].value->GetName() == name);

		return i != -1 ? mSourceMap[i].value : NULL;
	}

	PS_SourcePtr PS_Manager::GetSource(const String & filename)
	{
		return _find(Hash2(filename.c_str()), filename);
	}

	PS_SourcePtr PS_Manager::NewSource(const String & name)
	{
		d_assert (GetSource(name) == NULL);

		PS_Source * p = new PS_Source(name, "");

		mSourceMap.Insert(Hash2(name.c_str()), p);

		return p;
	}

	PS_SourcePtr PS_Manager::LoadSource(const String & filename, float priority)
	{
		Hash2 hash(filename.c_str());

		PS_SourcePtr p = _find(hash, filename);
		if (p == NULL)
		{
			p = new PS_Source(filename, filename);
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

	void PS_Manager::_delSource(PS_Source * p)
	{
		Hash2 hash(p->GetName().c_str());
		int i = mSourceMap.Find(hash);

		d_assert (i != -1);

		delete mSourceMap[i].value;
		mSourceMap.Erase(i);
	}

	void PS_Manager::OnInit()
	{
	}

	void PS_Manager::OnShutdown()
	{
		for (int i = 0; i < mPlayInfos.Size(); ++i)
		{
			delete mPlayInfos[i].p;
		}

		mPlayInfos.Clear();
	}

	void PS_Manager::OnUpdate(float frameTime)
	{
		for (int i = 0; i < mPlayInfos.Size(); ++i)
		{
			mPlayInfos[i].duration -= frameTime;
			if (mPlayInfos[i].duration <= 0)
			{
				delete mPlayInfos[i].p;
				
				mPlayInfos.Erase(i--);
			}
		}
	}

}
