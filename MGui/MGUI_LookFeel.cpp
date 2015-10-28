#include "MGUI_LookFeel.h"
#include "MGUI_Engine.h"
#include "MRoot.h"
#include <fstream>

namespace Rad {

	using namespace MGUI;

	LookFeel::LookFeel(const FixedString32 & _name, const char * _internal)
		: mName(_name)
		, mInternal(_internal)
	{
		for (int i = 0; i < eWidgetState::MAX; ++i)
		{
			mTextColor[i] = Float4(1, 1, 1, 1);
		}
	}

	LookFeel::~LookFeel()
	{
		for (int i = 0; i < mChildren.Size(); ++i)
		{
			delete mChildren[i];
		}
	}

	void LookFeel::SetSkin(const String & filename)
	{
		mSkin = filename;
	}

	void LookFeel::SetUVRect(int _state, const Rect & _rect)
	{
		if (_state >= 0)
		{
			mUVRect[_state] = _rect;
		}
		else
		{
			for (int i = 0; i < eWidgetState::MAX; ++i)
			{
				mUVRect[i] = _rect;
			}
		}
	}

	void LookFeel::SetTextColor(int _state, const Float4 & _color)
	{
		if (_state >= 0)
		{
			mTextColor[_state] = _color;
		}
		else
		{
			for (int i = 0; i < eWidgetState::MAX; ++i)
			{
				mTextColor[i] = _color;
			}
		}
	}

	Rect LookFeel::GetUVClientRect(int _state) const
	{
		const Rect & uvRect = mUVRect[_state];

		float x0 = uvRect.x + mClientRect.dx0;
		float y0 = uvRect.y + mClientRect.dy0;
		float x1 = uvRect.x + uvRect.w - mClientRect.dx1;
		float y1 = uvRect.y + uvRect.h - mClientRect.dy1;

		return Rect(x0, y0, x1 - x0, y1 - y0);
	}

	LookFeel * LookFeel::GetChild(const FixedString32 & _name)
	{
		for (int i = 0; i < mChildren.Size(); ++i)
		{
			if (mChildren[i]->GetName() == _name)
				return mChildren[i];
		}

		return NULL;
	}

	const LookFeel * LookFeel::GetChild(const FixedString32 & _name) const
	{
		for (int i = 0; i < mChildren.Size(); ++i)
		{
			if (mChildren[i]->GetName() == _name)
				return mChildren[i];
		}

		return NULL;
	}

	LookFeel * LookFeel::Clone(const FixedString32 & _name)
	{
		LookFeel * kClone = new LookFeel(_name);

		kClone->mSkin = mSkin;

		kClone->mUVRect = mUVRect;
		kClone->mClientRect = mClientRect;

		for (int i = 0; i < mChildren.Size(); ++i)
		{
			kClone->AddChild(mChildren[i]->Clone(mChildren[i]->GetName()));
		}

		return kClone;
	}

	//
	LookFeelGroup::LookFeelGroup(const FixedString32 & typeName)
		: mTypeName(typeName)
	{
	}

	LookFeelGroup::~LookFeelGroup()
	{
		Clear();
	}

	void LookFeelGroup::Clear()
	{
		for (int i = 0; i < mLookFeels.Size(); ++i)
		{
			delete mLookFeels[i].value;
		}

		mLookFeels.Clear();
	}

	void LookFeelGroup::Append(LookFeel * _lookfeel)
	{
		int crc32 = Math::Crc32(_lookfeel->GetName().c_str());

		d_assert (Find(crc32, _lookfeel->GetName()) == NULL);

		mLookFeels.Insert(crc32, _lookfeel);
	}

	void LookFeelGroup::Remove(LookFeel * _lookfeel)
	{
		int crc32 = Math::Crc32(_lookfeel->GetName().c_str());
		int i = mLookFeels.Find(crc32);

		d_assert (i != -1);

		delete mLookFeels[i].value;
		mLookFeels.Erase(i);
	}

	LookFeel * LookFeelGroup::Find(int crc32, const FixedString32 & name)
	{
		int i = mLookFeels.Find(crc32);

		d_assert (i == -1 || mLookFeels[i].value->GetName() == name);

		return i != -1 ? mLookFeels[i].value : NULL;
	}

	LookFeel * LookFeelGroup::Find(const FixedString32 & name)
	{
		return Find(Math::Crc32(name.c_str()), name);
	}

	void LookFeelGroup::_Remove(int i)
	{
		delete mLookFeels[i].value;
		mLookFeels.Erase(i);
	}

	void LookFeelGroup::GetLookFeelMap(Map<FixedString32, LookFeel *> & lookfeels)
	{
		for (int i = 0; i < mLookFeels.Size(); ++i)
		{
			lookfeels.Insert(mLookFeels[i].value->GetName(), mLookFeels[i].value);
		}
	}

	//
	ImplementSingleton(LookFeelManager);

	LookFeelManager::LookFeelManager()
	{
		mDefaultLookfeel = NULL;

		Array<FixedString32> names;
		Root::ENUM_OBJECT_FACTORY(names, "Widget", true);

		for (int i = 0; i < names.Size(); ++i)
		{
			mGroups.Insert(Math::Crc32(names[i].c_str()), new LookFeelGroup(names[i]));
		}
	}

	LookFeelManager::~LookFeelManager()
	{
		UnloadAll();

		for (int i = 0; i < mGroups.Size(); ++i)
		{
			delete mGroups[i].value;
		}
	}

	LookFeel * _loadLookFeel(xml_node node, char * _internal)
	{
		LookFeel * lookfeel = new LookFeel("", _internal);

		const char * name = node.first_attribute("name");
		const char * skin = node.first_attribute("skin");
		const char * client = node.first_attribute("client");

		d_assert (name[0] != 0);

		lookfeel->SetName(name);

		if (skin[0] != 0)
		{
			lookfeel->SetSkin(skin);
		}

		if (client)
		{
			const char * str = client;

			ClientRect clRect;
			str = str_getfloat(clRect.dx0, str);
			str = str_getfloat(clRect.dy0, str);
			str = str_getfloat(clRect.dx1, str);
			str = str_getfloat(clRect.dy1, str);

			lookfeel->SetClientRect(clRect);
		}

		for (int i = 0; i < eWidgetState::MAX; ++i)
		{
			FixedString32 stateString = eWidgetState::GetStateString(i);

			xml_node state = node.first_node(stateString.c_str());

			if (state != NULL)
			{
				const char * str = state.first_attribute("uv");
				if (*str)
				{
					Rect uvRect;
					str = str_getfloat(uvRect.x, str);
					str = str_getfloat(uvRect.y, str);
					str = str_getfloat(uvRect.w, str);
					str = str_getfloat(uvRect.h, str);

					lookfeel->SetUVRect(i, uvRect);
				}
				
				str = state.first_attribute("text_color");
				if (*str != 0)
				{
					Float4 color;
					str = str_getfloat(color.r, str);
					str = str_getfloat(color.g, str);
					str = str_getfloat(color.b, str);
					str = str_getfloat(color.a, str);

					lookfeel->SetTextColor(i, color);
				}
			}
		}

		xml_node child = node.first_node("Child");
		while (child != NULL)
		{
			lookfeel->AddChild(_loadLookFeel(child, NULL));

			child = child.next_sibling("Child");
		}

		return lookfeel;
	}

	void LookFeelManager::Load(const char * source)
	{
		DataStreamPtr stream = ResourceManager::Instance()->OpenResource(source);

		if (stream == NULL)
		{
			d_log("?: LookFeel File '%s' can't open.", source);
			return ;
		}

		LoadFromStream(stream, source);
	}

	void LookFeelManager::LoadFromStream(DataStreamPtr stream, const char * source)
	{
		d_assert (stream != NULL);

		char * filename = new char[128];
		strcpy(filename, source);

		xml_doc xdoc;
		xdoc.open(stream);

		xml_node root = xdoc.first_node();
		if (root != NULL)
		{
			for (int i = 0; i < mGroups.Size(); ++i)
			{
				const char * typeString = mGroups[i].value->TypeName().c_str();

				xml_node node = root.first_node(typeString);
				while (node != NULL)
				{
					LookFeel * lookfeel = _loadLookFeel(node, filename);

					mGroups[i].value->Append(lookfeel);

					node = node.next_sibling(typeString);
				}
			}

			mFilenames.PushBack(filename);
		}
	}

	void _saveLookFeel(LookFeel * lookfeel, xml_node node)
	{
		node.append_attribute("name", lookfeel->GetName().c_str());
		
		if (lookfeel->GetSkin() != "")
		{
			node.append_attribute("skin", lookfeel->GetSkin().c_str());
		}

		const ClientRect & clRect = lookfeel->GetClientRect();
		if (clRect != ClientRect())
		{
			String str;
			str += f2str(clRect.dx0, true) + " ";
			str += f2str(clRect.dy0, true) + " ";
			str += f2str(clRect.dx1, true) + " ";
			str += f2str(clRect.dy1, true);

			node.append_attribute("client", str.c_str());
		}

		for (int i = 0; i < eWidgetState::MAX; ++i)
		{
			FixedString32 stateString = eWidgetState::GetStateString(i);
			const Rect & uv = lookfeel->GetUVRect(i);
			const Float4 & color = lookfeel->GetTextColor(i);

			if (uv != Rect())
			{
				String str;
				str += f2str(uv.x, true) + " ";
				str += f2str(uv.y, true) + " ";
				str += f2str(uv.w, true) + " ";
				str += f2str(uv.h, true);

				node.append_node(stateString.c_str()).append_attribute("uv", str.c_str());
			}

			if (color != Float4(1, 1, 1, 1))
			{
				String str;
				str += f2str(color.r, true) + " ";
				str += f2str(color.g, true) + " ";
				str += f2str(color.b, true) + " ";
				str += f2str(color.a, true);

				node.append_node(stateString.c_str()).append_attribute("text_color", str.c_str());
			}
		}

		for (int i = 0; i < lookfeel->GetChildCount(); ++i)
		{
			xml_node child = node.append_node("Child");

			_saveLookFeel(lookfeel->GetChild(i), child);
		}
	}

	bool LookFeelManager::Save(const String & filename)
	{
		std::ofstream file;
		file.open(filename.c_str());

		if (!file.is_open())
			return false;

		xml_doc doc;
		xml_node root = doc.append_node("LookFeel");
		root.append_attribute("version", i2str(K_Version).c_str());

		for (int i = 0; i < mGroups.Size(); ++i)
		{
			const FixedString32 & typeString = mGroups[i].value->TypeName();

			for (int j = 0; j < mGroups[i].value->Size(); ++j)
			{
				LookFeel * lookfeel = mGroups[i].value->At(j);

				xml_node node = root.append_node(typeString.c_str());

				_saveLookFeel(lookfeel, node);
			}
		}

		doc.print(file);

		file.close();

		return true;
	}

	void LookFeelManager::Unload(const char * source)
	{
		d_assert (source != NULL);

		const char * filename = NULL;
		for (int i = 0; i < mFilenames.Size(); ++i)
		{
			if (strcmp(mFilenames[i], source) == 0)
			{
				filename = mFilenames[i];
				mFilenames.Erase(i);
				break;
			}
		}

		if (filename == NULL)
			return ;

		for (int i = 0; i < mGroups.Size(); ++i)
		{
			for (int j = 0; j < mGroups[i].value->Size(); ++j)
			{
				if (mGroups[i].value->At(j)->_getInternal() == filename)
				{
					mGroups[i].value->_Remove(j--);
				}
			}
		}

		delete filename;
	}

	void LookFeelManager::UnloadAll()
	{
		for (int i = 0; i < mGroups.Size(); ++i)
		{
			mGroups[i].value->Clear();
		}

		for (int i = 0; i < mFilenames.Size(); ++i)
		{
			delete[] mFilenames[i];
		}
		mFilenames.Clear();
	}

	void LookFeelManager::Append(const FixedString32 & _type, LookFeel * _lookfeel)
	{
		LookFeelGroup * group = GetGroup(_type);

		if (group != NULL)
		{
			group->Append(_lookfeel);

			return ;
		}

		d_assert (0);
	}

	void LookFeelManager::Remove(const FixedString32 & _type, LookFeel * _lookfeel)
	{
		LookFeelGroup * group = GetGroup(_type);

		if (group != NULL)
		{
			group->Remove(_lookfeel);

			return ;
		}

		d_assert (0);
	}

	LookFeel * LookFeelManager::Find(const FixedString32 & _type, const FixedString32 & _name)
	{
		LookFeelGroup * group = GetGroup(_type);

		if (group != NULL)
		{
			return group->Find(_name);
		}

		return NULL;
	}

	LookFeelGroup * LookFeelManager::GetGroup(const FixedString32 & _type)
	{
		int crc32 = Math::Crc32(_type.c_str());
		int i = mGroups.Find(crc32);

		d_assert (i == -1 || mGroups[i].value->TypeName() == _type);

		return i != -1 ? mGroups[i].value : NULL;
	}

	LookFeelGroup * LookFeelManager::GetGroup(int i)
	{
		return mGroups[i].value;
	}

	int LookFeelManager::GetGroupSize()
	{
		return mGroups.Size();
	}

	void LookFeelManager::GetGroupMap(Map<FixedString32, LookFeelGroup *> & groups)
	{
		for (int i = 0; i < mGroups.Size(); ++i)
		{
			groups.Insert(mGroups[i].value->TypeName(), mGroups[i].value);
		}
	}

}