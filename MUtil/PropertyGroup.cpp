#include "PropertyGroup.h"
#include "PropertyGrid.h"

namespace Rad {

	const float PropertyGroup::MIN_HEIGHT = 32.0f;

	PropertyGroup::PropertyGroup(const String & name, PropertyGrid * grid)
		: mPropertyGrid(grid)
	{
		mName = name;

		if (!grid->NoGroup())
		{
			mMaxHeight = MIN_HEIGHT;

			mLayout = MGUI::Layout::Load("PropertyGroup.layout", mPropertyGrid);
			mLayout->SetAlign(MGUI::eAlign::H_STRETCH);

			//
			mWidget_Caption = mLayout->GetChild("Caption");

			// TextBox
			mTextBox_Caption = new MGUI::TextBox(NULL, mWidget_Caption);
			mTextBox_Caption->SetAlign(MGUI::eAlign::CENTER);
			mTextBox_Caption->SetCaption(mName.c_wstr());

			// Expand
			mCheckBox_Expand = (MGUI::CheckBox *)mLayout->GetChild("Expand");
			mCheckBox_Expand->SetChecked(true);
			mCheckBox_Expand->E_Checked += new cListener2<PropertyGroup, const MGUI::Event *, bool>(this, &PropertyGroup::OnExpand);

			// Client
			mWidget_Client = mLayout->GetChild("Client");
			mWidget_Client->SetAlign(MGUI::eAlign::H_STRETCH);
		}
		else
		{
			mMaxHeight = 0;

			mLayout = new MGUI::Layout(NULL, mPropertyGrid);
			mLayout->SetAlign(MGUI::eAlign::H_STRETCH);

			mWidget_Caption = NULL;
			mTextBox_Caption = NULL;
			mCheckBox_Expand = NULL;

			mWidget_Client = new MGUI::Widget(NULL, mLayout);
			mWidget_Client->SetAlign(MGUI::eAlign::H_STRETCH);
		}


		MGUI::Rect rect = mLayout->GetRect();
		rect.h = MIN_HEIGHT;
		mLayout->SetRect(rect);
	}

	PropertyGroup::~PropertyGroup()
	{
		for (int i = 0; i < mFields.Size(); ++i)
		{
			delete mFields[i];
		}

		mFields.Clear();

		delete mLayout;
	}

	void PropertyGroup::Add(PropertyField * field)
	{
		mFields.PushBack(field);

		MGUI::Rect rect = mLayout->GetRect();

		if (!mPropertyGrid->NoGroup())
		{
			mMaxHeight += field->_updateHeight(mMaxHeight - MIN_HEIGHT);

			rect.h = mMaxHeight;

			mWidget_Client->SetRect(0, MIN_HEIGHT, rect.w, mMaxHeight - MIN_HEIGHT);
		}
		else
		{
			mMaxHeight += field->_updateHeight(mMaxHeight - 0);

			rect.h = mMaxHeight;

			mWidget_Client->SetRect(0, 0, rect.w, mMaxHeight);
		}

		mLayout->SetRect(MGUI::Helper::Instance()->GetExtendRect(rect, mLayout->GetClientRect()));
	}

	float PropertyGroup::_updateHeight(float y)
	{
		float cy = 0;
		for (int i = 0; i < mFields.Size(); ++i)
			cy += mFields[i]->_updateHeight(cy);

		MGUI::Rect rect = mLayout->GetRect();

		rect.y = y;

		mLayout->SetRect(rect);
		mLayout->_updateTM();

		if (mWidget_Caption != NULL)
			mWidget_Caption->_updateTM();

		return rect.h;
	}

	void PropertyGroup::OnExpand(const MGUI::Event * e, bool checked)
	{
		mLayout->RemoveAllComponent();

		MGUI::Rect rect = mLayout->GetClient();
		MGUI::ClientRect clRect = mLayout->GetClientRect();
		const float F_RATE = 300.0f;

		float d_h = checked ? mMaxHeight : MIN_HEIGHT;
		float d_t = Math::Abs(d_h - rect.h) / F_RATE;
		d_t = Min(d_t, 1.0f);

		d_h += clRect.dy0 + clRect.dy1;

		MGUI::KFAnimSize * pSizeKey = new MGUI::KFAnimSize;
		pSizeKey->Append(0, rect.w, rect.h);
		pSizeKey->Append(d_t, rect.w, d_h);

		mLayout->AddComponent(pSizeKey);
	}

}

