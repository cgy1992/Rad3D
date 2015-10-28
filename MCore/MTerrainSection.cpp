#include "MTerrainSection.h"
#include "MTerrainMesh.h"

namespace Rad {

	ImplementRTTI(TerrainSection, ISectionData);

	TerrainSection::TerrainSection()
	{
	}

	TerrainSection::~TerrainSection()
	{
		for (int i = 0; i < mMeshes.Size(); ++i)
		{
			mMeshes[i]->Destroy();
			mMeshes[i]->mSection = NULL;
		}
	}

	bool TerrainSection::Init(WorldSection * section, const rml_node * def)
	{
		ISectionData::Init(section, def);

		rml_node * node = def->first_node("Layers");
		if (node != NULL)
		{
			const char * str = node->value();
			while (*str)
			{
				int layerId = -1;
				str = str_getint(layerId, str);

				mLayers.PushBack(layerId);
			}
		}

		const Terrain::Info * ti = Terrain::Instance()->GetInfo();
		Int2 index = section->GetIndex();

		index *= ti->BlocksPerSection;

		mMeshes.Resize(ti->BlocksPerSection.x * ti->BlocksPerSection.y);
		for (int j = 0; j < ti->BlocksPerSection.y; ++j)
		{
			for (int i = 0; i < ti->BlocksPerSection.x; ++i)
			{
				mMeshes[j * ti->BlocksPerSection.x + i] = Terrain::Instance()->_getBlock(index.x + i, index.y + j);
			}
		}

		for (int i = 0; i < mMeshes.Size(); ++i)
		{
			mMeshes[i]->mSection = this;
		}

		return true;
	}

	void TerrainSection::Serialize(Serializer & sl)
	{
		if (sl.IsOut())
		{
			OSerializer & OS = (OSerializer &)sl;

			for (int i = 0; i < mMeshes.Size(); ++i)
			{
				OS << 0x0001;
				mMeshes[i]->Serialize(OS);
			}

			OS << 0;
		}
		else
		{
			ISerializer & IS = (ISerializer &)sl;

			int ckId, i = 0;
			while (IS.Read(&ckId, sizeof(int)) && ckId != 0)
			{
				if (ckId == 0x0001)
				{
					mMeshes[i++]->Serialize(IS);
				}
				else
				{
					d_assert (0);
				}
			}

			for (int i = 0; i < mMeshes.Size(); ++i)
			{
				mMeshes[i]->Build();
			}
		}
	}
}