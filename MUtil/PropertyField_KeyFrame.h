/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MUtil.h"
#include "MKeyController.h"
#include "PropertyFieldButton.h"
#include "TrackerControl.h"

namespace Rad {

	class KeyFrameEd
	{
	public:
		KeyFrameEd(IKeyController * kf, MGUI::Layout * parent);
		virtual ~KeyFrameEd();

		void SetVisible(bool b);

		void RefrushTracker();

	protected:
		void OnSelChanged(int index);
		void OnAdd(const MGUI::ClickEvent * e);
		void OnRemove(const MGUI::ClickEvent * e);
		void OnModify(const MGUI::FocusEvent * e);
		void OnClose(const MGUI::ClickEvent * e);

		void OnTrackerDBClick(const MGUI::MouseEvent * e);
		void OnUpdate(const MGUI::Event * e);
		void OnSelPointer(const MGUI::Event * e, bool checked);

	protected:
		MGUI::Layout * mLayout;
		TrackerControl * mTrackerControl;
		MGUI::EditBox * mEditBox_Time;

		PropertyGrid * mPropertyGrid;
		IKeyController * mKeyFrame;

		Array<TrackerPointer *> mPointers;
		TrackerPointer * mCurrentPointer;
	};

	//
	class UTIL_ENTRY PropertyFiled_KeyFrame : public PropertyFieldButton
	{
	public:
		PropertyFiled_KeyFrame(PropertyGroup * group, IObject * obj, const Property * prop, MGUI::Layout * parent);
		virtual ~PropertyFiled_KeyFrame();

		void OnClick(const MGUI::ClickEvent * e);

	protected:
		KeyFrameEd * mEditor;
	};

}