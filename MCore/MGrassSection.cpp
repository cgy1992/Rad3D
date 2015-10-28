#include "MGrassSection.h"
#include "MHWBufferManager.h"
#include "MLog.h"

namespace Rad {

	ImplementRTTI(GrassSection, ISectionData);

	GrassSection::GrassSection()
	{
		mBlockArray = NULL;
		mRenderer = new GrassRenderer(this);
	}

	GrassSection::~GrassSection()
	{
		safe_delete(mRenderer);
		safe_delete_array(mBlockArray);
	}

	bool GrassSection::Init(WorldSection * section, const rml_node * def)
	{
		ISectionData::Init(section, def);

		rml_node * node = def->first_node("Blocks");
		if (node != NULL)
		{
			const char * str = node->value();
			str = str_getint(mBlockCount.x, str);
			str = str_getint(mBlockCount.y, str);

			int count = mBlockCount.x * mBlockCount.y;
			if (count > 0 && count <= 100)
			{
				mBlockArray = new GrassBlock[count];

				float x_step = section->GetBound().GetSize().x / mBlockCount.x;
				float z_step = section->GetBound().GetSize().z / mBlockCount.y;
				for (int j = 0; j < mBlockCount.x; ++j)
				{
					for (int i = 0; i < mBlockCount.y; ++i)
					{
						RectF rect;
						rect.x1 = i * x_step;
						rect.y1 = j * z_step;
						rect.x2 = rect.x1 + x_step;
						rect.y2 = rect.y1 + z_step;

						mBlockArray[j * mBlockCount.x + i].RcBound = rect;
						mBlockArray[j * mBlockCount.x + i].mSection = this;
					}
				}
			}
			else
			{
				d_log ("?: Grass blocks defined error.");
				return false;
			}
		}
		else
		{
			d_log ("?: Grass blocks defined error.");
			return false;
		}

		node = def->first_node("Template");
		if (node != NULL)
		{
			rml_node * texture = node->first_node("Texture");
			rml_node * uvtiles = node->first_node("UVTiles");
			rml_node * castAO = node->first_node("CastAO");

			if (texture != NULL && uvtiles != NULL)
			{
				mTexture = HWBufferManager::Instance()->LoadTexture(texture->value());

				int uTile = 1, vTile = 1;
				const char * str = uvtiles->value();

				str = str_getint(uTile, str);
				str = str_getint(vTile, str);

				if (uTile > 0 && vTile > 0)
				{
					float u_step = 1.0f / uTile;
					float v_step = 1.0f / vTile;

					for (int v = 0; v < vTile; ++v)
					{
						for (int u = 0; u < uTile; ++u)
						{
							GrassTemplate gt;
							gt.Id = mTempArray.Size();
							gt.UVRect = RectF(u * u_step, v * v_step, (u + 1) * u_step, (v + 1) * v_step);
							gt.CastAO = false;

							mTempArray.PushBack(gt);
						}
					}
				}
				else
				{
					d_log("?: Grass template defined error.");
					return false;
				}
			}
			else
			{
				d_log("?: Grass template defined error.");
				return false;
			}

			if (castAO != NULL)
			{
				const char * str = castAO->value();
				if (str[0] == 'a' && str[1] == 'l' && str[2] == 'l')
				{
					for (int i = 0; i < mTempArray.Size(); ++i)
					{
						mTempArray[i].CastAO = true;
					}
				}
				else
				{
					str = str_skipwhite(str);
					while (*str)
					{
						int i = -1;
						str = str_getint(i, str);

						if (i >= 0 && i < mTempArray.Size())
						{
							mTempArray[i].CastAO = true;
						}
						else
						{
							d_log ("?: Grass CastAO defined error, '%d'.", i);
						}

						str = str_skipwhite(str);
					}
				}
			}
		}

		return true;
	}

	void GrassSection::Serialize(Serializer & sl)
	{
		if (sl.IsOut())
		{
			OSerializer & OS = (OSerializer &)sl;

			OS << 0x1001;
			OS << GetInstanceCount();
			for (int j = 0; j < mBlockCount.x * mBlockCount.y; ++j)
			{
				GrassBlock * block = &mBlockArray[j];
				for (int i = 0; i < block->InstanceArray.Size(); ++i)
				{
					const GrassInstance & gi = block->InstanceArray[i];

					OS << gi.Id;
					OS << gi.Position;
					OS << gi.Normal;
					OS << gi.Size;
					OS << gi.Rotation;
					OS << gi.LightingColor;
				}
			}

			OS << 0;
		}
		else
		{
			ISerializer & IS = (ISerializer &)sl;

			int ckId;
			while (IS.Read(&ckId, sizeof(int)) && ckId != 0)
			{
				if (ckId == 0x1001)
				{
					int numOfInsts = 0;
					IS >> numOfInsts;
					for (int i = 0; i < numOfInsts; ++i)
					{
						GrassInstance gi;
						IS >> gi.Id;
						IS >> gi.Position;
						IS >> gi.Normal;
						IS >> gi.Size;
						IS >> gi.Rotation;
						IS >> gi.LightingColor;

						AddInstance(gi);
					}
				}
				else
				{
					d_assert (0);
				}
			}
		}
	}

	const GrassTemplate * GrassSection::GetTemplate(int id)
	{
		return &mTempArray[id];
	}

	int GrassSection::GetTemplateCount()
	{
		return mTempArray.Size();
	}

	void GrassSection::AddInstance(const GrassInstance & gi)
	{
		for (int i = 0; i < mBlockCount.x * mBlockCount.y; ++i)
		{
			GrassBlock * block = &mBlockArray[i];
			if (block->RcBound.Contain(gi.Position.x, gi.Position.z))
			{
				block->InstanceArray.PushBack(gi);
				return ;
			}
		}
	}

	void GrassSection::RemoveInstance(int id, const RectF & rc)
	{
		for (int j = 0; j < mBlockCount.x * mBlockCount.y; ++j)
		{
			GrassBlock * block = &mBlockArray[j];
			if (!block->RcBound.Intersect(rc))
				continue;

			for (int i = 0; i < block->InstanceArray.Size(); ++i)
			{
				const GrassInstance & gi = block->InstanceArray[i];
				if (rc.Contain(gi.Position.x, gi.Position.z))
				{
					block->InstanceArray.Erase(i--);
				}
			}
		}
	}

	int GrassSection::GetInstanceCount()
	{
		int count = 0;
		for (int i = 0; i < mBlockCount.x * mBlockCount.y; ++i)
		{
			count += mBlockArray[i].InstanceArray.Size();
		}

		return count;
	}

	void GrassSection::_getInstance(Array<const GrassInstance *> & garray, float x, float z, float radius)
	{
		float dx, dz;
		float radiusSq = radius * radius;

		for (int j = 0; j < mBlockCount.x * mBlockCount.y; ++j)
		{
			GrassBlock * block = &mBlockArray[j];
			if (!block->RcBound.Intersect(x, z, radius))
				continue;

			for (int i = 0; i < block->InstanceArray.Size(); ++i)
			{
				GrassInstance * gi = &block->InstanceArray[i];

				dx = gi->Position.x - x;
				dz = gi->Position.z - z;

				if (dx * dx + dz * dz < radiusSq)
				{
					garray.PushBack(gi);
				}
			}
		}
	}

	void GrassSection::_getInstanceEx(Array<const GrassInstance *> & garray, const Float3 & eye, const Float3 & dir, float radius)
	{
		float radiusSq = radius * radius;

		for (int j = 0; j < mBlockCount.x * mBlockCount.y; ++j)
		{
			GrassBlock * block = &mBlockArray[j];
			if (!block->RcBound.Intersect(eye.x, eye.z, radius))
				continue;

			for (int i = 0; i < block->InstanceArray.Size(); ++i)
			{
				GrassInstance * gi = &block->InstanceArray[i];

				Float3 d = gi->Position - eye;
				if (d.x * d.x + d.z * d.z < radiusSq && d.Dot(dir) > 0)
					garray.PushBack(gi);
			}
		}
	}

	GrassBlock * GrassSection::_getBlock(int i)
	{
		d_assert (i < mBlockCount.x * mBlockCount.y);

		return &mBlockArray[i];
	}

	GrassBlock * GrassSection::_getBlock(int i, int j)
	{
		d_assert (i < mBlockCount.x && j < mBlockCount.y);

		return &mBlockArray[j * mBlockCount.x + i];
	}

}