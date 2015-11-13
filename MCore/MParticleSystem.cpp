#include "MParticleSystem.h"
#include "MWorld.h"
#include "MRoot.h"
#include "PSManager.h"

namespace Rad {

	DF_PROPERTY_BEGIN(ParticleSystem)
		DF_PROPERTY_EX(ParticleSystem, mFilename, "", "Filename", "PT_Filename particle(*.particle)|*.particle|", PT_String)
	DF_PROPERTY_END()

	void ParticleSystem::OnPropertyChanged(const Property * p)
	{
		if (p->name == "mFilename")
		{
			SetFilename(mFilename);
		}
		else
		{
			return Node::OnPropertyChanged(p);
		}
	}

	ImplementRTTI(ParticleSystem, Node);

	ParticleSystem::ParticleSystem()
		: Node("ParticleSystem")
	{
		mEnable = true;
	}

	ParticleSystem::~ParticleSystem()
	{
		Unload();
	}

	void ParticleSystem::SetFilename(const String & filename)
	{
		mFilename = filename;

		PS_SourcePtr ps = NULL;
		
		if (mFilename != "")
		{
			ps = PS_Manager::Instance()->LoadSource(filename);
		}

		SetSource(ps);
	}

	void ParticleSystem::SetSource(PS_SourcePtr ps)
	{
		if (mSource != ps)
		{
			Unload();

			mSource = ps;

			if (mSource != NULL)
			{
				mSource->E_Loaded += L_ParticleLoaded(this, &ParticleSystem::OnParticleLoaded);
				mSource->E_Unload += L_ParticleUnload(this, &ParticleSystem::OnParticleUnload);

				if (mSource->GetLoadState() == Resource::LOADED)
				{
					OnParticleLoaded();
				}
			}
		}
	}

	void ParticleSystem::SetEnable(bool enable)
	{
		for (int i = 0; i < mSetArray.Size(); ++i)
		{
			mSetArray[i]->SetEnable(enable);
		}

		mEnable = enable;
	}

	void ParticleSystem::Replay()
	{
		for (int i = 0; i < mSetArray.Size(); ++i)
		{
			mSetArray[i]->Reset();
		}
	}

	void ParticleSystem::Update(float elapsedTime)
	{
		Node::Update(elapsedTime);

		for (int i = 0; i < mSetArray.Size(); ++i)
		{
			mSetArray[i]->Update(elapsedTime);
		}

		if (mSetArray.Size() > 0)
		{
			Aabb bound = mSetArray[0]->GetBound();

			for (int i = 1; i < mSetArray.Size(); ++i)
			{
				bound.Merge(mSetArray[i]->GetBound());
			}

			SetWorldAabb(bound);
		}
		else
		{
			SetWorldAabb(Aabb(GetWorldPosition(), GetWorldPosition()));
		}
	}

	void ParticleSystem::AddRenderQueue(RenderQueue * rq)
	{
		for (int i = 0; i < mSetArray.Size(); ++i)
		{
			mSetArray[i]->AddRenderQueue(rq);
		}
	}

	PS_Set * ParticleSystem::NewSet(const char * type)
	{
		if (mSetArray.Size() < mSetArray.Capacity())
		{
			PS_Set * p = Root::NEW_OBJECT_T<PS_Set>(type);
			if (p != NULL)
			{
				p->Init(this);
				mSetArray.PushBack(p);
			}

			return p;
		}
		
		return NULL;
	}

	PS_Set * ParticleSystem::CloneSet(PS_Set * p)
	{
		PS_Set * render = NewSet(p->GetRTTI()->Name());
		if (render)
		{
			OSerializerM2 OS;
			p->Serialize(OS);

			ISerializerM IS((byte *)OS.Data(), OS.Size(), false);
			render->Serialize(IS);

			for (int i = 0; i < p->GetEmitterCount(); ++i)
			{
				render->CloneEmitter(p->GetEmitter(i));
			}

			for (int i = 0; i < p->GetModifierCount(); ++i)
			{
				render->CloneModifier(p->GetModifier(i));
			}

			if (p->GetShader() != NULL)
			{
				render->CloneShader(p->GetShader());
			}
		}

		return render;
	}

	void ParticleSystem::DeleteSet(int index)
	{
		delete mSetArray[index];
		mSetArray.Erase(index);
	}

	void ParticleSystem::DeleteSet(PS_Set * p)
	{
		d_assert (p->GetParent() == this);

		for (int i = 0; i < mSetArray.Size(); ++i)
		{
			if (mSetArray[i] == p)
			{
				DeleteSet(i);
				break;
			}
		}
	}

	PS_Set * ParticleSystem::GetSet(int index)
	{
		return mSetArray[index];
	}

	int ParticleSystem::GetSetCount()
	{
		return mSetArray.Size();
	}

	bool ParticleSystem::IsSetValid(PS_Set * p)
	{
		for (int i = 0; i < mSetArray.Size(); ++i)
		{
			if (mSetArray[i] == p)
				return true;
		}

		return false;
	}

	void ParticleSystem::SerializeTempalte(Serializer & sl)
	{
#define K_FILE_MAGIC0 MAKE_DWORD('P', 'S', '0', '0')
#define K_FILE_MAGIC1 MAKE_DWORD('P', 'S', '0', '1')
#define K_FILE_MAGIC2 MAKE_DWORD('P', 'S', '0', '2')
#define K_FILE_MAGIC3 MAKE_DWORD('P', 'S', '0', '3')

		int magic = K_FILE_MAGIC3;

		if (sl.IsOut())
		{
			OSerializer & OS = (OSerializer &)sl;

			OS << magic;

			int renderCount = mSetArray.Size();
			OS << renderCount;
			for (int i = 0; i < renderCount; ++i)
			{
				PS_Set * render = mSetArray[i];

				OS << FixedString32(render->GetRTTI()->Name());

				render->Serialize(OS);

				int emitterCount = render->GetEmitterCount();
				OS << emitterCount;
				for (int j = 0; j < emitterCount; ++j)
				{
					PS_Emitter * emitter = render->GetEmitter(j);

					OS << FixedString32(emitter->GetRTTI()->Name());

					emitter->Serialize(OS);
				}

				int modifierCount = render->GetModifierCount();
				OS <<  modifierCount;
				for (int j = 0; j < modifierCount; ++j)
				{
					PS_Modifier * modifier = render->GetModifier(j);

					OS << FixedString32(modifier->GetRTTI()->Name());

					modifier->Serialize(OS);
				}

				int hasShader = render->GetShader() != NULL ? 1 : 0;
				OS << hasShader;
				if (hasShader)
				{
					render->GetShader()->Serialize(OS);
				}
			}
		}
		else
		{
			ISerializer & IS = (ISerializer &)sl;

			IS >> magic;

			if (magic == K_FILE_MAGIC0)
			{
				int quota;
				bool localSpace;

				IS >> quota;
				IS >> localSpace;
			}

			if (magic == K_FILE_MAGIC1)
			{
				bool scaleAble;
				IS >> scaleAble;
			}

			int setCount = 0;
			IS >> setCount;
			for (int i = 0; i < setCount; ++i)
			{
				FixedString32 type;
				IS >> type;

				PS_Set * set = NewSet(type.c_str());
				if (set != NULL)
				{
					set->Serialize(IS);
				}
				else
				{
					d_log("X: PS_Set '%s' new failed.", type.c_str());
					return ;
				}

				int emitterCount = 0;
				IS >> emitterCount;
				for (int j = 0; j < emitterCount; ++j)
				{
					IS >> type;

					PS_Emitter * emitter = set->NewEmitter(type.c_str());

					if (emitter)
					{
						emitter->Serialize(IS);
					}
					else
					{
						d_log("X: PS_Emitter '%s' new failed.", type.c_str());
						return ;
					}
				}

				int modifierCount = 0;
				IS >> modifierCount;
				for (int j = 0; j < modifierCount; ++j)
				{
					IS >> type;

					PS_Modifier * modifier = set->NewModifier(type.c_str());

					if (modifier != NULL)
					{
						modifier->Serialize(IS);
					}
					else
					{
						d_log("X: PS_Modifier '%s' new failed.", type.c_str());
						return ;
					}
				}

				if (magic >= K_FILE_MAGIC3)
				{
					int hasShader = 0;
					IS >> hasShader;
					if (hasShader != 0)
					{
						d_assert (set->GetShader() != NULL);
						set->GetShader()->Serialize(IS);
					}
				}
			}
		}
	}

	void ParticleSystem::Load(float priority)
	{
		if (mSource != NULL && priority < 0)
		{
			mSource->EnsureLoad();
		}

		if (mSource == NULL && mFilename != "")
		{
			SetSource(PS_Manager::Instance()->LoadSource(mFilename, priority));
		}
	}

	void ParticleSystem::Unload()
	{
		OnParticleUnload();

		if (mSource != NULL)
		{
			mSource->E_Loaded -= L_ParticleLoaded(this, &ParticleSystem::OnParticleLoaded);
			mSource->E_Unload -= L_ParticleUnload(this, &ParticleSystem::OnParticleUnload);
			mSource = NULL;
		}
	}

	IObject * ParticleSystem::Clone()
	{
		ParticleSystem * p = new ParticleSystem;

		p->Copy(this);

		return p;
	}

	void ParticleSystem::OnParticleLoaded()
	{
		d_assert (mSource != NULL);
		d_assert (mSetArray.Size() == 0);

		if (mSource->GetStream() != NULL)
		{
			ISerializerD IS(mSource->GetStream());

			IS.Seek(0);
			SerializeTempalte(IS);
		}

		E_Loaded(this);
	}

	void ParticleSystem::OnParticleUnload()
	{
		for (int i = 0; i < mSetArray.Size(); ++i)
		{
			delete mSetArray[i];
		}

		mSetArray.Clear();

		E_Unload(this);
	}

}
