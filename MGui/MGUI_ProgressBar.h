/*
*	ProgressBar
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Widget.h"
#include "MGUI_ImageBox.h"

namespace Rad { namespace MGUI {

	class MGUI_ENTRY Progress
	{
	public:
		Progress();
		~Progress();

		void 
			SetPercentage(int _percentage);
		int 
			GetPrecentage();

	protected:
		int mPercentage;
	};


	class MGUI_ENTRY ProgressBarH : public Widget, public Progress
	{
		DECLARE_RTTI();

	public:
		ProgressBarH(const LookFeel * _lookfeel, Widget * _parent);
		virtual ~ProgressBarH();

		virtual void 
			UpdateRenderItem(Layout * _layout);
	};


	class MGUI_ENTRY ProgressBarV : public Widget, public Progress
	{
		DECLARE_RTTI();

	public:
		ProgressBarV(const LookFeel * _lookfeel, Widget * _parent);
		virtual ~ProgressBarV();

		virtual void 
			UpdateRenderItem(Layout * _layout);
	};


	class MGUI_ENTRY HImageProgressBar : public ImageBox, public Progress
	{
		DECLARE_RTTI();

	public:
		HImageProgressBar(Widget * _parent);
		virtual ~HImageProgressBar();

		virtual void 
			UpdateRenderItem(Layout * _layout);
	};


	class MGUI_ENTRY VImageProgressBar : public ImageBox, public Progress
	{
		DECLARE_RTTI();

	public:
		VImageProgressBar(Widget * _parent);
		virtual ~VImageProgressBar();

		virtual void 
			UpdateRenderItem(Layout * _layout);
	};

	class MGUI_ENTRY CircleProgressBar : public Widget
	{
		DECLARE_RTTI();

	public:
		CircleProgressBar(const LookFeel * _lookfeel, Widget * _parent);
		virtual ~CircleProgressBar();

		void SetPercentage(float _percentage);

		virtual void UpdateRenderItem(Layout * _layout);

	protected:
		float mPercentage;
	};

}}
