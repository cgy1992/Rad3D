#include "XMLConverter.h"

XMLConverter::XMLConverter()
{
	mTree = NULL;
}

XMLConverter::~XMLConverter()
{
	Clear();
}

void XMLConverter::DoConvertFloder(const String & path, const String & prefixName)
{
	FileSystem fs;

	fs.Open(path);

	while (fs.NextFile())
	{
		String filename = fs.FileName();
		filename.ToLower();

		if (filename == "." || filename == "..")
			continue;

		if (fs.FileIsDir())
		{
			String fullname = path + "/" + filename;

			DoConvertFloder(fullname, prefixName + filename + "/");
		}
		else if (FileHelper::GetExternName(filename) == "layout")
		{
			String fullname = path + "/" + filename;

			Clear();

			MGUI::Loader loader;
			loader.E_WidgetLoaded += new cListener1<XMLConverter, MGUI::Widget *>(this, &XMLConverter::OnWidgetLoaded);

			DataStreamPtr stream = ResourceManager::Instance()->OpenFile(fullname.c_str());
			MGUI::Layout * layout = loader.Load(stream, NULL);
			if (layout != NULL)
			{
				SaveBinary(fullname);
			}
		}
	}
}

void XMLConverter::ConvertFloder(const String & path)
{
	DoConvertFloder(path, "");
}

void _saveChild(WidgetNode * n, OSerializer & OS)
{
	FixedString32 typeName = n->_widget->GetRTTI()->Name();
	FixedString32 lookfeelName = n->_widget->GetLookFeel() ? n->_widget->GetLookFeel()->GetName() : "";

	OS << typeName;
	OS << lookfeelName;

	n->_widget->Serialize(OS);

	if (n->_first_child != NULL)
	{
		OS << 1;

		WidgetNode * child = n->_first_child;
		while (child != NULL)
		{
			_saveChild(child, OS);

			child = child->_next_sibling;
		}
	}
	else
	{
		OS << 0;
	}
}

void XMLConverter::SaveBinary(const String & filename)
{
	OSerializerF OS(filename.c_str());

	OS << MAKE_DWORD('M', 'G', 'U', 'I');

	_saveChild(mTree, OS);
}

void XMLConverter::Clear()
{
	safe_delete(mTree);
}

WidgetNode * _getParentNode(MGUI::Widget * parent, WidgetNode * node)
{
	if (node->_widget == parent)
		return node;

	WidgetNode * child = node->_first_child;
	while (child != NULL)
	{
		WidgetNode * node = _getParentNode(parent, child);
		if (node != NULL)
			return node;

		child = child->_next_sibling;
	}

	return NULL;
}

void XMLConverter::OnWidgetLoaded(MGUI::Widget * w)
{
	WidgetNode * n = new WidgetNode;
	n->_widget = w;

	if (mTree != NULL)
	{
		WidgetNode * parent = _getParentNode(w->GetParent(), mTree);
		d_assert (parent);
		parent->append(n);
	}
	else
	{
		mTree = n;
	}
}