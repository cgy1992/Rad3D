#include "MGUI_Loader.h"
#include "MGUI_Factory.h"

namespace Rad { namespace MGUI {

	Layout * Loader::Load(const String & source, Widget * parent)
	{
		DataStreamPtr stream = ResourceManager::Instance()->OpenResource(source);

		if (stream == NULL)
		{
			d_log("?: Layout file '%s' can't open.", source.c_str());
			return NULL;
		}

		return Load(stream, parent);
	}

	Layout * Loader::Load(DataStreamPtr stream, Widget * parent)
	{
		xml_doc xdoc;

		if (!xdoc.open(stream))
			return NULL;

		xml_node node = xdoc.first_node("Layout");
		if (node != NULL)
		{
			FixedString32 lkString = node.first_attribute("lookfeel");
			const LookFeel * lookfeel = NULL;

			if (lkString != "")
			{
				lookfeel = LookFeelManager::Instance()->Find("Widget", lkString);
				if (lookfeel == NULL)
				{
					lookfeel = LookFeelManager::Instance()->GetDefaultLookFeel();
				}
			}

			Layout * layout = new Layout(lookfeel, parent);

			_loadWidget(layout, node);

			return layout;
		}

		return NULL;
	}

	void Loader::_loadWidget(Widget * _widget, xml_node _node)
	{
		_widget->SerializeXML(_node, false);

		E_WidgetLoaded(_widget);

		xml_node child = _node.first_node("Child");
		while (child != NULL)
		{
			FixedString32 typeString = child.first_attribute("type");
			FixedString32 lkString = child.first_attribute("lookfeel");

			const LookFeel * lookfeel = NULL;
			if (lkString != "")
			{
				lookfeel = LookFeelManager::Instance()->Find(typeString, lkString);
				if (lookfeel == NULL)
				{
					lookfeel = LookFeelManager::Instance()->GetDefaultLookFeel();
				}
			}

			Widget * childWidget = NEW_WIDGET<Widget>(typeString.c_str(), lookfeel, _widget);

			_loadWidget(childWidget, child);

			child = child.next_sibling("Child");
		}
	}

	/*
	Layout * Loader::LoadBinary(const String & source, Widget * parent)
	{
		DataStreamPtr stream = ResourceManager::Instance()->OpenResource(source);

		if (stream == NULL)
		{
			d_log("?: Layout file '%s' can't open.", source.c_str());
			return NULL;
		}

		return LoadBinary(stream, parent);
	}

	Layout * Loader::LoadBinary(DataStreamPtr stream, Widget * parent)
	{
		ISerializerD IS(stream);

		const int K_MAGIC = MAKE_DWORD('M', 'G', 'U' , 'I');

		int magic = 0;
		IS >> magic;

		d_assert(magic == K_MAGIC);

		FixedString32 typeName, lookfeelName;
		IS >> typeName;
		IS >> lookfeelName;

		const LookFeel * lookfeel = NULL;
		if (lookfeelName != "")
		{
			lookfeel = LookFeelManager::Instance()->GetLookFeel("Widget", lookfeelName);
			if (lookfeel == NULL)
			{
				lookfeel = LookFeelManager::Instance()->GetDefaultLookFeel();
			}
		}

		Layout * layout = new Layout(lookfeel, parent);

		_loadWidgetBinary(layout, IS);

		return layout;
	}

	void Loader::_loadWidgetBinary(Widget * _widget, ISerializer & IS)
	{
		_widget->Serialize(IS);

		E_WidgetLoaded(_widget);

		int child = 0;
		IS >> child;

		while (child != 0)
		{
			FixedString32 typeName, lookfeelName;
			IS >> typeName;
			IS >> lookfeelName;

			const LookFeel * lookfeel = NULL;
			if (lookfeelName != "")
			{
				lookfeel = LookFeelManager::Instance()->GetLookFeel("Widget", lookfeelName);
				if (lookfeel == NULL)
				{
					lookfeel = LookFeelManager::Instance()->GetDefaultLookFeel();
				}
			}

			Widget * childWidget = NEW_WIDGET<Widget>(typeName.c_str(), lookfeel, _widget);

			_loadWidgetBinary(childWidget, IS);

			IS >> child;
		}
	}
	*/
}}
