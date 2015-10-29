#include "PropertyField_KeyFrame.h"
#include "AllLookFeel.h"

KeyFrameEd::KeyFrameEd(IKeyController * kf, MGUI::Layout * parent)
	: mKeyFrame(kf)
{
	mLayout = MGUI::Layout::Load("PropertyFieldKeyFrame.layout", parent);
	mLayout->SetAlign(MGUI::eAlign::STRETCH);
	mLayout->SetVisible(false);

	mTrackerControl = new TrackerControl(30, 16, 32, mLayout->GetChild("Tracker"));
	mTrackerControl->SetAlign(MGUI::eAlign::STRETCH);
	mTrackerControl->E_MouseDoubleClick += new cListener1<KeyFrameEd, const MGUI::MouseEvent *>(this, &KeyFrameEd::OnTrackerDBClick);

	mPropertyGrid = new PropertyGrid(mLayout->GetChild("KeyFrame"), true);
	mPropertyGrid->SetAlign(MGUI::eAlign::H_STRETCH);

	mEditBox_Time = (MGUI::EditBox *)mLayout->GetChild("Time");

	MGUI::Button * bnAdd = (MGUI::Button *)mLayout->GetChild("Add");
	MGUI::Button * bnRemove = (MGUI::Button *)mLayout->GetChild("Remove");
	MGUI::Button * bnClose = (MGUI::Button *)mLayout->GetChild("Close");

	bnAdd->E_Click += new cListener1<KeyFrameEd, const MGUI::ClickEvent *>(this, &KeyFrameEd::OnAdd);
	bnRemove->E_Click += new cListener1<KeyFrameEd, const MGUI::ClickEvent *>(this, &KeyFrameEd::OnRemove);
	bnClose->E_Click += new cListener1<KeyFrameEd, const MGUI::ClickEvent *>(this, &KeyFrameEd::OnClose);
	mLayout->E_Update += new cListener1<KeyFrameEd, const MGUI::Event *>(this, &KeyFrameEd::OnUpdate);
	mEditBox_Time->E_KeyLostFocus += new cListener1<KeyFrameEd, const MGUI::FocusEvent *>(this, &KeyFrameEd::OnModify);

	RefrushTracker();
}

KeyFrameEd::~KeyFrameEd()
{
	delete mPropertyGrid;
	delete mLayout;
}

void KeyFrameEd::SetVisible(bool b)
{
	mLayout->SetVisible(b);
}

void KeyFrameEd::RefrushTracker()
{
	for (int i = 0; i < mPointers.Size(); ++i)
	{
		delete mPointers[i];
	}
	mPointers.Clear();

	mCurrentPointer = NULL;
	mKeyFrame->Adjust();

	MGUI::Widget * parent = mLayout->GetChild("Tracker");
	for (int i = 0; i < mKeyFrame->Size(); ++i)
	{
		TrackerPointer * p = new TrackerPointer(parent);
		p->SetRect(0, 0, 16, 16);
		p->SetUserData((void*)i);
		p->SetStateColor(MGUI::eWidgetState::NORMAL, Float4(1, 1, 1));
		p->E_Checked += new cListener2<KeyFrameEd, const MGUI::Event *, bool>(this, &KeyFrameEd::OnSelPointer);

		MGUI::Widget * v = new MGUI::Widget(AllLookFeel::Instance()->GetWhite(), p);
		v->SetAlign(MGUI::eAlign::CENTER);
		v->SetRect(0, 0, 12, 12);
		v->SetPickFlag(MGUI::ePickFlag::NONE);
		v->SetOrder(2);

		mPointers.PushBack(p);
	}
}

void KeyFrameEd::OnSelChanged(int index)
{
	if (index < 0)
		return ;

	float time;
	IObject * obj;
	mKeyFrame->GetKeyFrame(index, &time, &obj);

	mEditBox_Time->SetCaption(f2str(time, true, "%.2f").c_wstr());
	mPropertyGrid->Attach(obj);
}

void KeyFrameEd::OnAdd(const MGUI::ClickEvent * e)
{
	String t_str;
	t_str.FromUnicode(mEditBox_Time->GetCaption().c_str());
	if (t_str == "")
		return ;

	float time = t_str.ToFloat();

	mKeyFrame->Append(time, NULL);

	RefrushTracker();

	for (int i = 0; i < mKeyFrame->Size(); ++i)
	{
		float t;
		mKeyFrame->GetKeyFrame(i, &t, NULL);

		if (time == t)
		{
			MGUI::Event e(NULL);
			e.sender = mPointers[i]; 
			OnSelPointer(&e, true);
			break;
		}
	}
}

void KeyFrameEd::OnRemove(const MGUI::ClickEvent * e)
{
	if (mCurrentPointer)
	{
		int index = (int)mCurrentPointer->GetUserData();

		mKeyFrame->Erase(index);

		RefrushTracker();
	}
}

void KeyFrameEd::OnModify(const MGUI::FocusEvent * e)
{
	if (mCurrentPointer)
	{
		int index = (int)mCurrentPointer->GetUserData();

		String t_str;
		t_str.FromUnicode(mEditBox_Time->GetCaption().c_str());

		float time = t_str.ToFloat();

		mKeyFrame->SetKeyFrame(index, time, NULL);

		RefrushTracker();

		for (int i = 0; i < mKeyFrame->Size(); ++i)
		{
			float t;
			mKeyFrame->GetKeyFrame(i, &t, NULL);

			if (time == t)
			{
				MGUI::Event e(NULL);
				e.sender = mPointers[i]; 
				OnSelPointer(&e, true);
				break;
			}
		}
	}
}

void KeyFrameEd::OnClose(const MGUI::ClickEvent * e)
{
	SetVisible(false);
}

void KeyFrameEd::OnTrackerDBClick(const MGUI::MouseEvent * e)
{
	float time = mTrackerControl->MapUnit(e->x);
	String strTime = f2str(time, true);

	mEditBox_Time->SetCaption(strTime.c_wstr());

	OnAdd(NULL);
}

void KeyFrameEd::OnUpdate(const MGUI::Event * e)
{
	MGUI::Widget * parent = mLayout->GetChild("Tracker");

	for (int i = 0; i < mKeyFrame->Size(); ++i)
	{
		int index = (int)mPointers[i]->GetUserData();

		float time;
		IObject * obj;
		mKeyFrame->GetKeyFrame(index, &time, &obj);

		float k = Math::Clamp(time, 0.0f, 1.0f) * parent->GetClient().w;

		mPointers[i]->SetRectPosition(k - 8, 8);
	}
}

void KeyFrameEd::OnSelPointer(const MGUI::Event * e, bool checked)
{
	if (mCurrentPointer)
	{
		mCurrentPointer->SetChecked(false);
	}

	mCurrentPointer = (TrackerPointer *)e->sender;

	if (mCurrentPointer)
	{
		mCurrentPointer->SetChecked(true);

		int index = (int)mCurrentPointer->GetUserData();

		OnSelChanged(index);
	}
}




PropertyFiled_KeyFrame::PropertyFiled_KeyFrame(PropertyGroup * group, IObject * obj, const Property * prop, MGUI::Layout * parent)
	: PropertyFieldButton(group, obj, prop)
{
	mButton->SetCaption(WSTR_("..."));
	mButton->E_Click += new cListener1<PropertyFiled_KeyFrame, const MGUI::ClickEvent *>(this, &PropertyFiled_KeyFrame::OnClick);

	mEditor = new KeyFrameEd((IKeyController *)obj->GetPropertyData(prop), parent);
}

PropertyFiled_KeyFrame::~PropertyFiled_KeyFrame()
{
	delete mEditor;
}

void PropertyFiled_KeyFrame::OnClick(const MGUI::ClickEvent * e)
{
	mEditor->SetVisible(true);
}