#include "MGUI_FontManager.h"
#include "MGUI_FontTTF.h"
#include "MGUI_FontImage.h"

namespace Rad {

	using namespace MGUI;

	ImplementSingleton(FontManager);

	FontManager::FontManager()
		: mDefaultFont(NULL)
	{
		AddFactory(new FontFactoryTTF);
		AddFactory(new FontFactoryImage);
	}

	FontManager::~FontManager()
	{
		for (int i = 0; i < mFonts.Size(); ++i)
		{
			delete mFonts[i];
		}

		mFonts.Clear();

		for (int i = 0; i < mFontFactorys.Size(); ++i)
		{
			delete mFontFactorys[i];
		}

		mFontFactorys.Clear();
	}

	void FontManager::Load(const String & _source)
	{
		DataStreamPtr stream = ResourceManager::Instance()->OpenResource(_source.c_str());
		if (stream == NULL)
		{
			d_log("?: Source '%s' can't opened.", _source.c_str());
			return ;
		}

		rml_doc xdoc;
		if (!xdoc.open(stream))
		{
			d_log("?: Font '%s'.", _source.c_str());
			return ;
		}

		rml_node * node = xdoc.first_node("Font");
		while (node)
		{
			String type = node->value();

			FontFactory * factory = GetFactory(type);
			
			if (factory)
			{
				Font * font = factory->Load(node);

				if (mDefaultFont == NULL)
					mDefaultFont = font;

				if (font)
					mFonts.PushBack(font);
			}
			else
			{
				d_log("?: FontFactory '%s' not exist.", type.c_str());
			}

			node = node->next_sibling("Font");
		}
	}

	void FontManager::AddFactory(FontFactory * factoy)
	{
		d_assert (GetFactory(factoy->GetType()) == NULL);

		mFontFactorys.PushBack(factoy);
	}

	FontFactory * FontManager::GetFactory(const String & type)
	{
		for (int i = 0; i < mFontFactorys.Size(); ++i)
		{
			if (mFontFactorys[i]->GetType() == type)
				return mFontFactorys[i];
		}

		return NULL;
	}

	int FontManager::GetFontCount()
	{
		return mFonts.Size();
	}

	Font * FontManager::GetFont(int _index)
	{
		return mFonts[_index];
	}

	Font * FontManager::GetFont(const FixedString32 & _name)
	{
		for (int i = 0; i < mFonts.Size(); ++i)
		{
			if (mFonts[i]->GetName() == _name)
				return mFonts[i];
		}

		return NULL;
	}

	void FontManager::SetDefaultFont(const FixedString32 & _name)
	{
		mDefaultFont = GetFont(_name);
	}

	Font * FontManager::GetDefaultFont()
	{
		return mDefaultFont;
	}

}

