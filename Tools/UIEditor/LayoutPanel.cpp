#include "LayoutPanel.h"
#include "AllLookFeel.h"
#include "LayoutEditor.h"
#include "MainStatusBar.h"
#include "App.h"
#include <fstream>

LayoutPanel::LayoutPanel()
{
	mWidgetCreateDlg = new WidgetCreateDlg;
	mWidgetCreateDlg->E_EndDialog += new cListener1<LayoutPanel, bool>(this, &LayoutPanel::OnWidgetCreate_);

	mLayout = new MGUI::Layout(AllLookFeel::Instance()->GetPanel());

	float y_base = 0;

	mTreeView = new MGUI::TreeView(AllLookFeel::Instance()->GetTreeView(), mLayout);
	mTreeView->SetAlign(MGUI::eAlign::H_STRETCH);
	mTreeView->SetRect(0, y_base, 0, 24 + 256);
	y_base += 24 + 256 + 5;

	MGUI::Button * bnCreate = new MGUI::Button(AllLookFeel::Instance()->GetButton(), mLayout);
	bnCreate->SetAlign(MGUI::eAlign::H_STRETCH);
	bnCreate->SetRect(0, y_base, 0, 30);
	bnCreate->SetCaption(L"Create");
	y_base += 30 + 5;

	MGUI::Button * bnDelete = new MGUI::Button(AllLookFeel::Instance()->GetButton(), mLayout);
	bnDelete->SetAlign(MGUI::eAlign::H_STRETCH);
	bnDelete->SetRect(0, y_base, 0, 30);
	bnDelete->SetCaption(L"Delete");
	y_base += 30 + 5;

	bnCreate->E_MouseClick += new cListener1<LayoutPanel, const MGUI::MouseEvent *>(this, &LayoutPanel::OnCreate_);
	bnDelete->E_MouseClick += new cListener1<LayoutPanel, const MGUI::MouseEvent *>(this, &LayoutPanel::OnDelete_);

	mTreeView->E_SelectChanged += new cListener1<LayoutPanel, MGUI::TreeViewItem *>(this, &LayoutPanel::OnWidgetSelChanged_);
	
	LayoutEditor::Instance()->E_NameChanged += new cListener1<LayoutPanel, String>(this, &LayoutPanel::OnNameChanged_);
}

LayoutPanel::~LayoutPanel()
{
	delete mLayout;

	delete mWidgetCreateDlg;
}

void LayoutPanel::Layout()
{
	MGUI::Rect rect;

	rect.x = 0;
	rect.y = 32;
	rect.w = 256;
	rect.h = MGUI::Engine::Instance()->GetRect().h - 32 - 32;

	mLayout->SetRect(rect);
}

void LayoutPanel::OnCreate_(const MGUI::MouseEvent * e)
{
	mWidgetCreateDlg->DoModal(LayoutEditor::Instance()->GetEditLayout() == NULL);
}

void LayoutPanel::OnDelete_(const MGUI::MouseEvent * e)
{
	MGUI::TreeViewItem * pSelItem = mTreeView->GetSelectItem();

	if (pSelItem == NULL)
		return ;

	MGUI::Widget * widget = (MGUI::Widget *)pSelItem->GetUserData();

	if (widget == LayoutEditor::Instance()->GetEditLayout())
	{
		if (MessageBox(App::Instance()->_hWnd(), "Ready destroy layout?", "Warning", MB_OKCANCEL | MB_ICONWARNING) == IDOK)
		{
			LayoutEditor::Instance()->_distroyLayout();

			mTreeView->Clear();
		}
	}
	else
	{
		LayoutEditor::Instance()->notifyEditWidgetChanged(NULL);
		delete widget;

		mTreeView->Remove(pSelItem);
	}
}

void LayoutPanel::OnWidgetCreate_(bool _ok)
{
	if (_ok)
	{
		FixedString32 typeName = mWidgetCreateDlg->GetSelectType();
		MGUI::LookFeel * lookfeel = mWidgetCreateDlg->GetSelectLookFeel();
		bool first = false;

		MGUI::Widget * parent = LayoutEditor::Instance()->GetEditLayout();
		if (parent == NULL && typeName == "Widget")
		{
			if (!lookfeel)
				lookfeel = AllLookFeel::Instance()->GetEditorHelper();

			LayoutEditor::Instance()->_createLayout(lookfeel);
			parent = LayoutEditor::Instance()->GetEditLayout();

			mTreeView->Append(parent->GetName().c_wstr(), parent);

			first = true;
		}
		else if (parent && typeName != "" && (lookfeel != NULL || typeName == "Widget" || typeName == "Label"))
		{
			if (!lookfeel)
				lookfeel = AllLookFeel::Instance()->GetEditorHelper();

			MGUI::TreeViewItem * pParentItem = mTreeView->GetItem(0);

			if (mTreeView->GetSelectItem() != NULL)
			{
				pParentItem = mTreeView->GetSelectItem();
			}

			parent = (MGUI::Widget *)pParentItem->GetUserData();

			MGUI::Widget * _widget = MGUI::NEW_WIDGET<MGUI::Widget>(typeName.c_str(), lookfeel, parent);

			_widget->SetName("Widget");

			MGUI::TreeViewItem * _item = pParentItem->Append(_widget->GetName().c_wstr(), _widget);

			if (first)
				parent->SetRect(_widget->GetRect());
			else
			{
				MGUI::Rect rect = parent->GetRect();

				if (rect.x < 0 && rect.y < 0)
					_widget->Move(-rect.x, -rect.y);
				else if (rect.x < 0 && rect.y >= 0)
					_widget->Move(-rect.x, 0);
				else if (rect.x >= 0 && rect.y < 0)
					_widget->Move(0, -rect.y);
			}

			LayoutEditor::Instance()->notifyEditWidgetChanged(_widget);

			mTreeView->SetSelectItem(_item);
		}
	}
}

void LayoutPanel::OnWidgetSelChanged_(MGUI::TreeViewItem * pSelItem)
{
	if (pSelItem == NULL)
	{
		LayoutEditor::Instance()->notifyEditWidgetChanged(NULL);
	}
	else
	{
		MGUI::Widget * widget = (MGUI::Widget *)pSelItem->GetUserData();

		LayoutEditor::Instance()->notifyEditWidgetChanged(widget);
	}
}

void LayoutPanel::OnNameChanged_(String _name)
{
	MGUI::TreeViewItem * pSelItem = mTreeView->GetSelectItem();
	MGUI::Widget * widget = (MGUI::Widget *)pSelItem->GetUserData();

	d_assert (widget == LayoutEditor::Instance()->GetEditWidget());

	pSelItem->SetText(widget->GetName().c_wstr());
}

void _SaveWidget(xml_node node, MGUI::TreeViewItem * item)
{
	MGUI::Widget * widget = (MGUI::Widget *)item->GetUserData();

	FixedString32 lookfeel = widget->GetLookFeel() ? widget->GetLookFeel()->GetName() : "";

	xml_node child = node.append_node("Child");

	child.append_attribute("type", widget->GetRTTI()->Name());

	child.append_attribute("lookfeel", lookfeel.c_str());

	widget->SerializeXML(child, true);

	for (int i = 0; i < item->GetCount(); ++i)
	{
		MGUI::TreeViewItem * childItem = item->GetChildItem(i);

		_SaveWidget(child, childItem);
	}
}

bool LayoutPanel::SaveLayout(const String & filename)
{
	if (mTreeView->GetCount() == 0)
		return false;

	std::ofstream file;
	file.open(filename.c_str());

	if (!file.is_open())
		return false;

	xml_doc doc;
	xml_node node = doc.append_node("Layout");

	MGUI::TreeViewItem * pRootItem = mTreeView->GetItem(0);
	MGUI::Widget * widget = (MGUI::Widget *)pRootItem->GetUserData();

	if (widget->GetLookFeel())
	{
		FixedString32 lookfeel = widget->GetLookFeel()->GetName();

		node.append_attribute("lookfeel", lookfeel.c_str());
	}

	node.append_attribute("version", i2str(MGUI::Loader::K_Version).c_str());

	widget->SerializeXML(node, true);

	for (int i = 0; i < pRootItem->GetCount(); ++i)
	{
		_SaveWidget(node, pRootItem->GetChildItem(i));
	}

	doc.print(file);

	file.close();

	return true;
}

void LayoutPanel::Clear()
{
	mTreeView->Clear();
}

MGUI::TreeViewItem * _getParentItem(MGUI::Widget * parent, MGUI::TreeViewItem * parentItem)
{
	if (parentItem->GetUserData() == parent)
		return parentItem;

	for (int i = 0; i < parentItem->GetCount(); ++i)
	{
		MGUI::TreeViewItem * item = _getParentItem(parent, parentItem->GetChildItem(i));
		if (item != NULL)
			return item;
	}

	return NULL;
}

void LayoutPanel::OnWidgetLoaded(MGUI::Widget * w)
{
	if (mTreeView->GetCount() > 0)
	{
		MGUI::TreeViewItem * item = _getParentItem(w->GetParent(), mTreeView->GetItem(0));
		d_assert (item != NULL);

		item->Append(w->GetName().c_wstr(), w);
	}
	else
	{
		d_assert (TYPE_OF(MGUI::Layout, w));

		mTreeView->Append(w->GetName().c_wstr(), w);
	}
}