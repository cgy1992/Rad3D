#include "MWorldSection.h"
#include "MWorld.h"

namespace Rad {

	WorldSection::WorldSection()
		: mZone(NULL)
		, mDefNode(NULL)
	{
	}

	WorldSection::~WorldSection()
	{
		Unload();
	}

	void WorldSection::Init(const String & filename, const Int2 & index, const rml_node * defnode)
	{
		const World::Info * wi = World::Instance()->GetInfo();

		mFilename = filename;
		mIndex = index;
		mDefNode = defnode;

		mBound.minimum = Float3(wi->SectionSize.x * mIndex.x, 0, wi->SectionSize.z * mIndex.y);
		mBound.maximum = mBound.minimum + wi->SectionSize;
	}

	void WorldSection::AddNode(Node * node)
	{
		d_assert (node->GetSection() == NULL);

		node->_notifyAttachSection(this);
		
		mNodes.PushBack(node);

		World::Instance()->E_SectionAddNode(this, node);
	}

	void WorldSection::RemoveNode(Node * node, bool _delete)
	{
		World::Instance()->E_SectionRemoveNode(this, node);

		for (int i = 0; i < mNodes.Size(); ++i)
		{
			if (mNodes[i] == node)
			{
				if (_delete)
				{
					delete node;
				}

				mNodes.Erase(i);
				return;
			}
		}

		d_assert (0);
	}

	ISectionData * WorldSection::GetData(const char * className)
	{
		for (int i = 0; i < mDatas.Size(); ++i)
		{
			if (strcmp(mDatas[i]->GetRTTI()->Name(), className) == 0)
			{
				return mDatas[i];
			}
		}

		return NULL;
	}

	void WorldSection::Update(float elapsedTime)
	{
		if (!IsValid())
			return ;

		if (World::Instance()->IsMultiThreadLoad() && World::Instance()->GetLoadRule() != NULL)
		{
			Float3 camPos = World::Instance()->MainCamera()->GetWorldPosition();

			for (int i = 0; i < mNodes.Size(); ++i)
			{
				Node * node = mNodes[i];
				float priority = World::Instance()->GetLoadRule()->GetLoadPriority(node, camPos);

				if (priority >= 0)
				{
					node->Load(priority);
				}
				else
				{
					node->Unload();
				}
			}
		}

		for (int i = 0; i < mDatas.Size(); ++i)
		{
			mDatas[i]->Update(elapsedTime);
		}
	}

	void WorldSection::Load()
	{
		if (!IsValid())
			return ;

		d_assert (mResource == NULL);

		mResource = new ResourceDataStream(mFilename, mFilename);
		mResource->E_Loaded += L_ResourceLoaded(this, &WorldSection::_OnResourceLoaded);
		mResource->E_Unload += L_ResourceUnload(this, &WorldSection::_OnResourceUnload);

		mResource->Load();
	}

	void WorldSection::Unload()
	{
		if (!IsValid())
			return ;

		_OnResourceUnload();

		if (mResource != NULL)
		{
			mResource->E_Loaded -= L_ResourceLoaded(this, &WorldSection::_OnResourceLoaded);
			mResource->E_Unload -= L_ResourceUnload(this, &WorldSection::_OnResourceUnload);

			mResource = NULL;
		}
	}

	void WorldSection::_OnResourceLoaded()
	{
		d_assert (mZone == NULL);

		const World::Info * wi = World::Instance()->GetInfo();

		mZone = new Zone(this);
		mZone->Resize(mBound, wi->SectionZoneTile.x, wi->SectionZoneTile.y, wi->SectionZoneTile.z, wi->SectionZoneDepth);
		World::Instance()->MainZone()->AddChild(mZone);

		const rml_node * d_node = mDefNode->first_node("Data");
		while (d_node)
		{
			const char * name = d_node->value();

			ISectionData * data = Root::Instance()->NEW_OBJECT_T<ISectionData>(name);
			if (data != NULL && data->Init(this, d_node))
			{
				mDatas.PushBack(data);
			}
			else
			{
				safe_delete (data);
				d_log("X: New section data '%s' failed.", name);
			}

			d_node = d_node->next_sibling("Data");
		}

		if (mResource->GetDataStream() != NULL)
		{
			ISerializerD IS(mResource->GetDataStream());
			Serialize(IS);
		}

		mResource->E_Loaded -= L_ResourceLoaded(this, &WorldSection::_OnResourceLoaded);
		mResource->E_Unload -= L_ResourceUnload(this, &WorldSection::_OnResourceUnload);
		mResource = NULL;

		World::Instance()->E_SectionLoaded(this);
	}

	void WorldSection::_OnResourceUnload()
	{
		World::Instance()->E_SectionUnload(this);

		for (int i = 0; i < mDatas.Size(); ++i)
		{
			delete mDatas[i];
		}
		mDatas.Clear();

		for (int i = 0; i < mNodes.Size(); ++i)
		{
			delete mNodes[i];
		}
		mNodes.Clear();

		if (mZone != NULL)
		{
			World::Instance()->MainZone()->RemoveChild(mZone, true);
			mZone = NULL;
		}
	}

	void WorldSection::Serialize(Serializer & sl)
	{
		if (!IsValid())
			return ;

		const int K_MAGIC = MAKE_DWORD('W', 'S', '$', '0');
		const int K_NODE = 0x1000;
		const int K_DATA = 0x2000;

		if (sl.IsOut())
		{
			OSerializer & OS = (OSerializer &)sl;
			OSerializerTester OT;

			OS << K_MAGIC;

			for (int i = 0; i < mNodes.Size(); ++i)
			{
				if (mNodes[i]->Serializable())
				{
					OS << K_NODE;

					int typeId = mNodes[i]->GetRTTI()->TypeId();
					
					OT.Clear();
					OT << typeId;
					mNodes[i]->Serialize(OT);

					OS << OT.Size();
					OS << typeId;
					mNodes[i]->Serialize(OS);
				}
			}

			for (int i = 0; i < mDatas.Size(); ++i)
			{
				if (mDatas[i]->Serializable())
				{
					OS << K_DATA;

					FixedString32 name = mDatas[i]->GetRTTI()->Name();

					OT.Clear();
					OT << name;
					mDatas[i]->Serialize(OT);

					OS << OT.Size();
					OS << name;
					mDatas[i]->Serialize(OS);
				}
			}
		}
		else
		{
			ISerializer & IS = (ISerializer &)sl;

			int magic;
			IS >> magic;

			d_assert (magic == K_MAGIC);

			int ckId, length;
			while (IS.Read(&ckId, sizeof(ckId)) && IS.Read(&length, sizeof(length)))
			{
				switch(ckId)
				{
				case K_NODE:
					{
						int typeId = 0;
						IS >> typeId;

						Node * node = Root::NEW_OBJECT_T<Node>(typeId);
						node->Serialize(IS);

						AddNode(node);
					}
					break;

				case K_DATA:
					{
						FixedString32 name;
						IS >> name;

						ISectionData * data = GetData(name.c_str());
						if (data != NULL)
						{
							data->Serialize(IS);
						}
						else
						{
							d_log("?: Section data '%s' error in '%s'", name.c_str(), mFilename.c_str());
							IS.Skip(length - 32);
						}
					}
					break;
				}
			}
		}
	}

	//
	ImplementRTTI(ISectionData, IObject);

	bool ISectionData::Init(WorldSection * section, const rml_node * def)
	{
		mWorldSection = section;

		return true;
	}
}