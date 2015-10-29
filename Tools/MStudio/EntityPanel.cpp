#include "EntityPanel.h"
#include "MainFrame.h"
#include "Editor.h"
#include "MainRenderView.h"

ImplementSingleton(EntityPanel);

EntityPanel::EntityPanel()
{
	mLayout = MGUI::Layout::Load("EntityPanel.layout", NULL);
	d_assert(mLayout != NULL);

	mLayout->SetVisible(false);

	mLayoutTips = new MGUI::Layout(NULL);
	mLayoutTips->SetOrder(MGUI::eOrder::TEXT + 1000);
	mLayoutTips->SetAlign(MGUI::eAlign::STRETCH);
	mLayoutTips->SetPickFlag(MGUI::ePickFlag::NONE);

	mTextBoxTips = new MGUI::TextBox(NULL, mLayoutTips);
	mTextBoxTips->SetVisible(false);

	mGridBox = new MGUI::GridBox(NULL, mLayout->GetChild("Item"));
	mGridBox->SetColumn(2);
	mGridBox->SetAutoResizeType(MGUI::GridBox::NONE);
	mGridBox->SetAlign(MGUI::eAlign::STRETCH);
	mGridBox->_updateTM();
	mGridBox->SetGridSize(mGridBox->GetRect().w / 2, 36);

	InitEntityFactory();

	mLayout->E_Update += new cListener1<EntityPanel, const MGUI::Event *>(this, &EntityPanel::OnUpdate);
}

EntityPanel::~EntityPanel()
{
	safe_delete (mLayout);
	safe_delete (mLayoutTips);
}

void EntityPanel::Layout()
{
	MGUI::Rect rect = MGUI::Engine::Instance()->GetRect();

	float w = mLayout->GetRect().w;

	mLayout->SetRect(0, D_MAINMENU_H, w, rect.h - D_MAINMENU_H - D_MAINSTATUSBAR_H);
}

void EntityPanel::Show()
{
	mLayout->SetVisible(true);
}

void EntityPanel::Hide()
{
	mLayout->SetVisible(false);
}

void EntityPanel::InitEntityFactory()
{
	mCurrentFactory = NULL;

	for (int i = 0; i < EditorFactory::Instance()->GetEntityFactorys().Size(); ++i)
	{
		EntityFactory * p = EditorFactory::Instance()->GetEntityFactorys()[i];

		const MGUI::LookFeel * lk = AllLookFeel::Instance()->GetButton();
		MGUI::Button * bn = new MGUI::Button(lk, NULL);
		bn->SetRect(0, 0, 110, 30);
		bn->SetCaption(p->GetTypeName().c_wstr());

		bn->E_MouseDown += new cListener1<EntityPanel, const MGUI::MouseEvent *>(this, &EntityPanel::OnObjectPressed);
		bn->E_MouseUp += new cListener1<EntityPanel, const MGUI::MouseEvent *>(this, &EntityPanel::OnObjectReleased);

		bn->SetUserData(p);

		mGridBox->Append(bn);
	}
}

void EntityPanel::OnUpdate(const MGUI::Event * e)
{
	Float3 mp = MGUI::InputManager::Instance()->_getMousePosition();
	const MGUI::Rect & myRect = mTextBoxTips->GetRect();

	mTextBoxTips->SetRectPosition(mp.x - myRect.w / 2, mp.y - myRect.h / 2);
}

void EntityPanel::OnObjectPressed(const MGUI::MouseEvent * e)
{
	if (!Editor::Instance()->IsLoaded())
		return ;

	mCurrentFactory = (EntityFactory * )e->sender->GetUserData();
	if (mCurrentFactory != NULL)
	{
		mTextBoxTips->SetVisible(true);
		mTextBoxTips->SetCaption(mCurrentFactory->GetTypeName().c_wstr());
	}
}

void EntityPanel::OnObjectReleased(const MGUI::MouseEvent * e)
{
	if (!Editor::Instance()->IsLoaded())
		return ;

	if (mCurrentFactory != NULL &&
		MGUI::Engine::Instance()->GetWidget(e->x, e->y) == MainRenderView::Instance()->GetLayout())
	{
		Node * pNode = mCurrentFactory->NewObject();

		Float2 pt = IMouse::Instance()->GetPosition();
		Editor::Instance()->MapScreenUnit(pt.x, pt.y);

		Float3 pos = Editor::Instance()->GetHitPosition(pt.x, pt.y);

		pNode->SetPosition(pos);

		Editor::Instance()->SetSelectNode(pNode);
	}

	mCurrentFactory = NULL;
	mTextBoxTips->SetVisible(false);
}