#include "GizmoBar.h"

#include "GizmoBar.h"
#include "Editor.h"
#include "BaseOperator.h"

GizmoBar::GizmoBar()
{
	mLayout = MGUI::Layout::Load("GizmoBar.layout", NULL);
	mLayout->SetVisible(false);

	mWidget_Move = mLayout->GetChild("Move"); d_assert (mWidget_Move);
	mWidget_Rotate = mLayout->GetChild("Rotate"); d_assert (mWidget_Rotate);
	mWidget_Scale = mLayout->GetChild("Scale"); d_assert (mWidget_Scale);

	mWidget_Move->SetAlpha(1.0f);
	mWidget_Rotate->SetAlpha(0.5f);
	mWidget_Scale->SetAlpha(0.5f);

	mWidget_Move->E_MouseClick += new cListener1<GizmoBar, const MGUI::MouseEvent *>(this, &GizmoBar::OnMove);
	mWidget_Rotate->E_MouseClick += new cListener1<GizmoBar, const MGUI::MouseEvent *>(this, &GizmoBar::OnRotate);
	mWidget_Scale->E_MouseClick += new cListener1<GizmoBar, const MGUI::MouseEvent *>(this, &GizmoBar::OnScale);

	mEditBox_X = (MGUI::EditBox *)mLayout->GetChild("x");
	mEditBox_Y = (MGUI::EditBox *)mLayout->GetChild("y");
	mEditBox_Z = (MGUI::EditBox *)mLayout->GetChild("z");

	mEditBox_X->E_KeyLostFocus += new cListener1<GizmoBar, const MGUI::FocusEvent *>(this, &GizmoBar::OnTextChanged);
	mEditBox_Y->E_KeyLostFocus += new cListener1<GizmoBar, const MGUI::FocusEvent *>(this, &GizmoBar::OnTextChanged);
	mEditBox_Z->E_KeyLostFocus += new cListener1<GizmoBar, const MGUI::FocusEvent *>(this, &GizmoBar::OnTextChanged);

	Editor::Instance()->E_NodeSelect += new cListener0<GizmoBar>(this, &GizmoBar::OnNodeSelect);
	Editor::Instance()->E_NodePositionChanged += new cListener0<GizmoBar>(this, &GizmoBar::OnPositionChanged); 
	Editor::Instance()->E_NodeRotationChanged += new cListener0<GizmoBar>(this, &GizmoBar::OnRotationChanged); 
	Editor::Instance()->E_NodeScaleChanged += new cListener0<GizmoBar>(this, &GizmoBar::OnScaleChanged); 
}

GizmoBar::~GizmoBar()
{
	delete mLayout;
}

void GizmoBar::Layout()
{
	MGUI::Rect rect = MGUI::Engine::Instance()->GetRect();
	float x = 256;
	float h = mLayout->GetRect().h;

	mLayout->SetRect(x, 0, rect.w - x, h);
}

void GizmoBar::OnMove(const MGUI::MouseEvent * e)
{
	mWidget_Move->SetAlpha(1.0f);
	mWidget_Rotate->SetAlpha(0.5f);
	mWidget_Scale->SetAlpha(0.5f);

	Gizmo::Instance()->SetOperator(Gizmo::OP_MOVE);

	Node * n = Editor::Instance()->GetSelectNode();
	if (n != NULL)
	{
		Float3 p = n->GetPosition();

		String x_str, y_str, z_str;
		x_str = f2str(p.x, true, "%.2f");
		y_str = f2str(p.y, true, "%.2f");
		z_str = f2str(p.z, true, "%.2f");

		mEditBox_X->SetCaption(x_str.c_wstr());
		mEditBox_Y->SetCaption(y_str.c_wstr());
		mEditBox_Z->SetCaption(z_str.c_wstr());
	}
	else
	{
		String x_str, y_str, z_str;

		mEditBox_X->SetCaption(x_str.c_wstr());
		mEditBox_Y->SetCaption(y_str.c_wstr());
		mEditBox_Z->SetCaption(z_str.c_wstr());
	}
}

void GizmoBar::OnRotate(const MGUI::MouseEvent * e)
{
	mWidget_Move->SetAlpha(0.5f);
	mWidget_Rotate->SetAlpha(1.0f);
	mWidget_Scale->SetAlpha(0.5f);

	Gizmo::Instance()->SetOperator(Gizmo::OP_ROTATE);

	Node * n = Editor::Instance()->GetSelectNode();
	if (n != NULL)
	{
		Quat q = n->GetRotation();
		Float3 p = q.ToEulerAngle();

		String x_str, y_str, z_str;
		x_str = f2str(p.x, true, "%.2f");
		y_str = f2str(p.y, true, "%.2f");
		z_str = f2str(p.z, true, "%.2f");

		mEditBox_X->SetCaption(x_str.c_wstr());
		mEditBox_Y->SetCaption(y_str.c_wstr());
		mEditBox_Z->SetCaption(z_str.c_wstr());
	}
	else
	{
		String x_str, y_str, z_str;

		mEditBox_X->SetCaption(x_str.c_wstr());
		mEditBox_Y->SetCaption(y_str.c_wstr());
		mEditBox_Z->SetCaption(z_str.c_wstr());
	}
}

void GizmoBar::OnScale(const MGUI::MouseEvent * e)
{
	mWidget_Move->SetAlpha(0.5f);
	mWidget_Rotate->SetAlpha(0.5f);
	mWidget_Scale->SetAlpha(1.0f);

	Gizmo::Instance()->SetOperator(Gizmo::OP_SCALE);

	Node * n = Editor::Instance()->GetSelectNode();
	if (n != NULL)
	{
		Float3 p = n->GetScale();

		String x_str, y_str, z_str;
		x_str = f2str(p.x, true, "%.2f");
		y_str = f2str(p.y, true, "%.2f");
		z_str = f2str(p.z, true, "%.2f");

		mEditBox_X->SetCaption(x_str.c_wstr());
		mEditBox_Y->SetCaption(y_str.c_wstr());
		mEditBox_Z->SetCaption(z_str.c_wstr());
	}
	else
	{
		String x_str, y_str, z_str;

		mEditBox_X->SetCaption(x_str.c_wstr());
		mEditBox_Y->SetCaption(y_str.c_wstr());
		mEditBox_Z->SetCaption(z_str.c_wstr());
	}
}

void GizmoBar::OnTextChanged(const MGUI::FocusEvent * e)
{
	Node * n = Editor::Instance()->GetSelectNode();
	if (n == NULL)
		return ;

	String x_str, y_str, z_str;
	x_str.FromUnicode(mEditBox_X->GetCaption().c_str());
	y_str.FromUnicode(mEditBox_Y->GetCaption().c_str());
	z_str.FromUnicode(mEditBox_Z->GetCaption().c_str());

	Float3 v;
	v.x = x_str.ToFloat();
	v.y = y_str.ToFloat();
	v.z = z_str.ToFloat();


	switch (Gizmo::Instance()->GetOperator())
	{
	case Gizmo::OP_MOVE:
		{
			UndoRedoManager::Instance()->Push(new UndoRedo_Move(n->GetUID(), n->GetPosition(), v));

			n->SetPosition(v);

			Editor::Instance()->E_NodePositionChanged();
		}
		break;

	case Gizmo::OP_ROTATE:
		{
			Quat q;
			q.FromEulerAngle(v);

			UndoRedoManager::Instance()->Push(new UndoRedo_Rotate(n->GetUID(), n->GetRotation(), q));

			n->SetRotation(q);

			Editor::Instance()->E_NodeRotationChanged();
		}
		break;

	case Gizmo::OP_SCALE:
		{
			UndoRedoManager::Instance()->Push(new UndoRedo_Scale(n->GetUID(), n->GetScale(), v));

			n->SetScale(v);

			Editor::Instance()->E_NodeScaleChanged();
		}
		break;
	}
}

void GizmoBar::OnNodeSelect()
{
	switch (Gizmo::Instance()->GetOperator())
	{
	case Gizmo::OP_MOVE:
		OnMove(NULL);
		break;

	case Gizmo::OP_ROTATE:
		OnRotate(NULL);
		break;

	case Gizmo::OP_SCALE:
		OnScale(NULL);
		break;
	}
}

void GizmoBar::OnPositionChanged()
{
	OnMove(NULL);
}

void GizmoBar::OnRotationChanged()
{
	OnRotate(NULL);
}

void GizmoBar::OnScaleChanged()
{
	OnScale(NULL);
}
