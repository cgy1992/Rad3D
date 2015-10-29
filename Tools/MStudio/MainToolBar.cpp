#include "EntityToolBar.h"
#include "MainFrame.h"
#include "EntityLayout.h"
#include "EntityPropertyPanel.h"
#include "Editor.h"

EntityToolBar::EntityToolBar()
{
	mLayout = MGUI_LayoutLoader::Load("EntityToolBar.layout", NULL);
	mLayout->SetVisible(false);

	mWidget_Pick = mLayout->GetChild("Pick"); d_assert (mWidget_Pick);
	mWidget_Move = mLayout->GetChild("Move"); d_assert (mWidget_Move);
	mWidget_Rotate = mLayout->GetChild("Rotate"); d_assert (mWidget_Rotate);
	mWidget_Scale = mLayout->GetChild("Scale"); d_assert (mWidget_Scale);

	mWidget_Pick->SetAlpha(1.0f);
	mWidget_Move->SetAlpha(0.5f);
	mWidget_Rotate->SetAlpha(0.5f);
	mWidget_Scale->SetAlpha(0.5f);

	mWidget_Pick->E_MouseClick += new cListener1<EntityToolBar, const MGUI_MouseEvent *>(this, &EntityToolBar::OnPick);
	mWidget_Move->E_MouseClick += new cListener1<EntityToolBar, const MGUI_MouseEvent *>(this, &EntityToolBar::OnMove);
	mWidget_Rotate->E_MouseClick += new cListener1<EntityToolBar, const MGUI_MouseEvent *>(this, &EntityToolBar::OnRotate);
	mWidget_Scale->E_MouseClick += new cListener1<EntityToolBar, const MGUI_MouseEvent *>(this, &EntityToolBar::OnScale);

	MGUI_Widget * pEditorLayout = mLayout->GetChild("Editor");
	d_assert(pEditorLayout != NULL);

	mLayout_Position = MGUI_LayoutLoader::Load("EntityPosition.Layout", pEditorLayout);
	mLayout_Rotation = MGUI_LayoutLoader::Load("EntityRotation.Layout", pEditorLayout);
	mLayout_Scale = MGUI_LayoutLoader::Load("EntityScale.Layout", pEditorLayout);
	d_assert (mLayout_Position && mLayout_Rotation && mLayout_Scale);

	mLayout_Position->SetAlign(MGUI_Align::LEFT | MGUI_Align::V_CENTER);
	mLayout_Rotation->SetAlign(MGUI_Align::LEFT | MGUI_Align::V_CENTER);
	mLayout_Scale->SetAlign(MGUI_Align::LEFT | MGUI_Align::V_CENTER);

	mLayout_Position->SetVisible(true);
	mLayout_Rotation->SetVisible(false);
	mLayout_Scale->SetVisible(false);

	mEditBox_PositionX = (MGUI_EditBox *)mLayout_Position->GetChild("x");
	mEditBox_PositionY = (MGUI_EditBox *)mLayout_Position->GetChild("y");
	mEditBox_PositionZ = (MGUI_EditBox *)mLayout_Position->GetChild("z");
	d_assert(mEditBox_PositionX && mEditBox_PositionY && mEditBox_PositionZ);

	mEditBox_RotationX = (MGUI_EditBox *)mLayout_Rotation->GetChild("x");
	mEditBox_RotationY = (MGUI_EditBox *)mLayout_Rotation->GetChild("y");
	mEditBox_RotationZ = (MGUI_EditBox *)mLayout_Rotation->GetChild("z");
	d_assert(mEditBox_RotationX && mEditBox_RotationY && mEditBox_RotationZ);

	mEditBox_Scale = (MGUI_EditBox *)mLayout_Scale->GetChild("x");
	d_assert(mEditBox_Scale);

	mEditBox_PositionX->E_KeyLostFocus += new cListener1<EntityToolBar, const MGUI_FocusEvent *>(this, &EntityToolBar::OnPositionChanged);
	mEditBox_PositionY->E_KeyLostFocus += new cListener1<EntityToolBar, const MGUI_FocusEvent *>(this, &EntityToolBar::OnPositionChanged);
	mEditBox_PositionZ->E_KeyLostFocus += new cListener1<EntityToolBar, const MGUI_FocusEvent *>(this, &EntityToolBar::OnPositionChanged);

	mEditBox_RotationX->E_KeyLostFocus += new cListener1<EntityToolBar, const MGUI_FocusEvent *>(this, &EntityToolBar::OnRotationChanged);
	mEditBox_RotationY->E_KeyLostFocus += new cListener1<EntityToolBar, const MGUI_FocusEvent *>(this, &EntityToolBar::OnRotationChanged);
	mEditBox_RotationZ->E_KeyLostFocus += new cListener1<EntityToolBar, const MGUI_FocusEvent *>(this, &EntityToolBar::OnRotationChanged);

	mEditBox_Scale->E_KeyLostFocus += new cListener1<EntityToolBar, const MGUI_FocusEvent *>(this, &EntityToolBar::OnScaleChanged);

	Editor::Instance()->E_ShapeScelected += new cListener0<EntityToolBar>(this, &EntityToolBar::OnShapeSelectedChanged);
	Editor::Instance()->E_ShapePositionChanged += new cListener0<EntityToolBar>(this, &EntityToolBar::OnShapePositionChanged);
	Editor::Instance()->E_ShapeRotationChanged += new cListener0<EntityToolBar>(this, &EntityToolBar::OnShapeRotationChanged);
	Editor::Instance()->E_ShapeScaleChanged += new cListener0<EntityToolBar>(this, &EntityToolBar::OnShapeScaleChanged);
}

EntityToolBar::~EntityToolBar()
{
	delete mLayout;
}

void EntityToolBar::Layout()
{
	MGUI_Rect rect = MGUI_Engine::Instance()->GetRect();
	float x = EntityPropertyPanel::Instance()->GetLayout()->GetAbsRect().x + EntityPropertyPanel::Instance()->GetLayout()->GetAbsRect().w;
	float h = mLayout->GetRect().h;

	mLayout->SetRect(x, D_MAINMENU_H, rect.w - x, h);
}

void EntityToolBar::Show()
{
	mLayout->SetVisible(true);
}

void EntityToolBar::Hide()
{
	mLayout->SetVisible(false);
}

void EntityToolBar::OnPick(const MGUI_MouseEvent * e)
{
	EntityLayout::Instance()->SetOperator(EntityLayout::eOP_Pick);
	
	mWidget_Pick->SetAlpha(1.0f);
	mWidget_Move->SetAlpha(0.5f);
	mWidget_Rotate->SetAlpha(0.5f);
	mWidget_Scale->SetAlpha(0.5f);

	mLayout_Position->SetVisible(true);
	mLayout_Rotation->SetVisible(false);
	mLayout_Scale->SetVisible(false);
}

void EntityToolBar::OnMove(const MGUI_MouseEvent * e)
{
	EntityLayout::Instance()->SetOperator(EntityLayout::eOP_Move);

	mWidget_Pick->SetAlpha(0.5f);
	mWidget_Move->SetAlpha(1.0f);
	mWidget_Rotate->SetAlpha(0.5f);
	mWidget_Scale->SetAlpha(0.5f);

	mLayout_Position->SetVisible(true);
	mLayout_Rotation->SetVisible(false);
	mLayout_Scale->SetVisible(false);
}

void EntityToolBar::OnRotate(const MGUI_MouseEvent * e)
{
	EntityLayout::Instance()->SetOperator(EntityLayout::eOP_Rotate);

	mWidget_Pick->SetAlpha(0.5f);
	mWidget_Move->SetAlpha(0.5f);
	mWidget_Rotate->SetAlpha(1.0f);
	mWidget_Scale->SetAlpha(0.5f);

	mLayout_Position->SetVisible(false);
	mLayout_Rotation->SetVisible(true);
	mLayout_Scale->SetVisible(false);
}

void EntityToolBar::OnScale(const MGUI_MouseEvent * e)
{
	EntityLayout::Instance()->SetOperator(EntityLayout::eOP_Scale);

	mWidget_Pick->SetAlpha(0.5f);
	mWidget_Move->SetAlpha(0.5f);
	mWidget_Rotate->SetAlpha(0.5f);
	mWidget_Scale->SetAlpha(1.0f);

	mLayout_Position->SetVisible(false);
	mLayout_Rotation->SetVisible(false);
	mLayout_Scale->SetVisible(true);
}

void EntityToolBar::OnPositionChanged(const MGUI_FocusEvent * e)
{
	Shape * pShape = Editor::Instance()->GetSelectedShape();
	if (pShape == NULL)
		return ;

	TString128 x_str, y_str, z_str;
	x_str.FromUnicode(mEditBox_PositionX->GetCaption().c_str());
	y_str.FromUnicode(mEditBox_PositionY->GetCaption().c_str());
	z_str.FromUnicode(mEditBox_PositionZ->GetCaption().c_str());

	Float3 v;
	v.x = x_str.ToFloat();
	v.y = y_str.ToFloat();
	v.z = z_str.ToFloat();

	pShape->SetPosition(v);
}

void EntityToolBar::OnRotationChanged(const MGUI_FocusEvent * e)
{
	Shape * pShape = Editor::Instance()->GetSelectedShape();
	if (pShape == NULL)
		return ;

	TString128 x_str, y_str, z_str;
	x_str.FromUnicode(mEditBox_RotationX->GetCaption().c_str());
	y_str.FromUnicode(mEditBox_RotationY->GetCaption().c_str());
	z_str.FromUnicode(mEditBox_RotationZ->GetCaption().c_str());

	Float3 v;
	v.x = x_str.ToFloat();
	v.y = y_str.ToFloat();
	v.z = z_str.ToFloat();

	pShape->SetRotation(v);
}

void EntityToolBar::OnScaleChanged(const MGUI_FocusEvent * e)
{
	Shape * pShape = Editor::Instance()->GetSelectedShape();
	if (pShape == NULL)
		return ;

	TString128 x_str;
	x_str.FromUnicode(mEditBox_Scale->GetCaption().c_str());

	float v = x_str.ToFloat();

	pShape->SetScale(v);
}

void EntityToolBar::OnShapeSelectedChanged()
{
	Shape * pShape = Editor::Instance()->GetSelectedShape();
	if (pShape == NULL)
	{
		mEditBox_PositionX->SetCaption(WSTR_(""));
		mEditBox_PositionY->SetCaption(WSTR_(""));
		mEditBox_PositionZ->SetCaption(WSTR_(""));

		mEditBox_RotationX->SetCaption(WSTR_(""));
		mEditBox_RotationY->SetCaption(WSTR_(""));
		mEditBox_RotationZ->SetCaption(WSTR_(""));

		mEditBox_Scale->SetCaption(WSTR_(""));
	}
	else
	{
		OnShapePositionChanged();
		OnShapeRotationChanged();
		OnShapeScaleChanged();
	}
}

void EntityToolBar::OnShapePositionChanged()
{
	Shape * pShape = Editor::Instance()->GetSelectedShape();
	if (pShape != NULL)
	{
		Float3 position = pShape->GetPosition();

		mEditBox_PositionX->SetCaption(util::f2str(position.x, true, "%.2f").c_wstr());
		mEditBox_PositionY->SetCaption(util::f2str(position.y, true, "%.2f").c_wstr());
		mEditBox_PositionZ->SetCaption(util::f2str(position.z, true, "%.2f").c_wstr());
	}
}

void EntityToolBar::OnShapeRotationChanged()
{
	Shape * pShape = Editor::Instance()->GetSelectedShape();
	if (pShape != NULL)
	{
		Float3 rotation = pShape->GetRotation();

		mEditBox_RotationX->SetCaption(util::f2str(rotation.x, true, "%.2f").c_wstr());
		mEditBox_RotationY->SetCaption(util::f2str(rotation.y, true, "%.2f").c_wstr());
		mEditBox_RotationZ->SetCaption(util::f2str(rotation.z, true, "%.2f").c_wstr());
	}
}

void EntityToolBar::OnShapeScaleChanged()
{
	Shape * pShape = Editor::Instance()->GetSelectedShape();
	if (pShape != NULL)
	{
		float scale = pShape->GetScale();

		mEditBox_Scale->SetCaption(util::f2str(scale, true, "%.2f").c_wstr());
	}
}