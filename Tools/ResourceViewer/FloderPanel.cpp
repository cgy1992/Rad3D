#include "FloderPanel.h"
#include "AllLookFeel.h"
#include "RenderViewer.h"

ImplementSingleton(FloderPanel);

FloderPanel::FloderPanel()
	: mArchive(NULL)
{
	mLayout = new MGUI::Layout(NULL, NULL);
	mTreeView = new MGUI::TreeView(AllLookFeel::Instance()->GetTreeView(), mLayout);
	mTreeView->SetAlign(MGUI::eAlign::STRETCH);
	mTreeView->E_SelectChanged += new cListener1<FloderPanel, MGUI::TreeViewItem *>(this, &FloderPanel::OnSelectChanged);

	Layout();
}

FloderPanel::~FloderPanel()
{
	delete mLayout;
}

void FloderPanel::Layout()
{
	MGUI::Rect rect = MGUI::Engine::Instance()->GetRect();

	float x = 0, y = 32, w = 256, h = rect.h - 32;

	mLayout->SetRect(x, y, w, h);
}

void _appendChild(const String & floder, MGUI::TreeViewItem * parent)
{
	FileSystem fs;
	if (!fs.Open(floder))
		return ;

	while (fs.NextFile())
	{
		if (fs.FileIsDir())
		{
			String filename = fs.FileName();

			if (filename != ".." && filename != ".")
			{
				MGUI::TreeViewItem * child = parent->Append(filename.c_wstr());
				_appendChild(fs.GetOpenDir() + "/" + filename, child);
			}
		}
	}
}

void FloderPanel::SetFloder(const String & floder)
{
	RenderViewer::Instance()->SetFloader("", "");

	mRootFloader = FileHelper::GetFileDir(floder);

	mTreeView->Clear();
	if (mArchive != NULL)
	{
		ResourceManager::Instance()->RemoveArchive(mArchive->GetName());
		mArchive = NULL;
	}

	MGUI::TreeViewItem * root = mTreeView->Append(FileHelper::GetBaseName(floder).c_wstr());

	_appendChild(floder, root);

	mArchive = new FilePathArchive(floder);
	ResourceManager::Instance()->AddArchive(mArchive);
}

void FloderPanel::OnSelectChanged(MGUI::TreeViewItem * item)
{
	String filename = "";

	while (item != NULL)
	{
		String s;
		s.FromUnicode(item->GetText().c_str());

		if (filename != "")
		{
			filename = s + "/" + filename;
		}
		else
		{
			filename = s;
		}

		item = item->GetParentItem();
	}

	if (mRootFloader != "")
	{
		filename = mRootFloader + "/" + filename;
	}

	filename.Replace('\\', '/');

	RenderViewer::Instance()->SetFloader(filename, "");
}
