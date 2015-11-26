/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MUtil.h"

namespace Rad {

	class UTIL_ENTRY ColorPickerDialog : public Singleton<ColorPickerDialog>
	{
	public:
		tEvent1<bool> E_EndDialog;
		tEvent0 E_Preview;

	public:
		ColorPickerDialog();
		~ColorPickerDialog();

		Float4 GetCurrentColor();

		void DoModal(const Float4 & color, void * userData = NULL);

	protected:
		void _genTexture();
		void _updateColor();
		void _updateTexture(const Float4 & color);
		void _updateFromColor(const Float4 & color);
		void _updateFromPoint(float x, float y);
		float& byIndex(Float4 & _color, size_t _index);


	protected:
		void OnClose(const MGUI::MouseEvent *);
		void OnOK(const MGUI::MouseEvent *);
		void OnCancel(const MGUI::MouseEvent *);

		void OnPickColor(const MGUI::MouseEvent *);
		void OnPickSlider(const MGUI::MouseEvent *);

		void OnColorTextChanged(const MGUI::FocusEvent *);

	protected:
		MGUI::Layout * mDialog;
		MGUI::Layout * mLayout;
		MGUI::EditBox * mEditBox_RGBA;
		MGUI::EditBox * mEditBox_S;
		MGUI::Widget * mWidget_View;
		MGUI::ImageBox * mImageBox_Color;
		MGUI::ImageBox * mImageBox_Picker;

		MGUI::ImageBox * mImageBox_Slider;
		MGUI::ImageBox * mImageBox_SliderPicker;

		Array<Float4> mColorRange;
		TexturePtr mColorTexture;
		TexturePtr mSliderTexture;

		Float4 mBaseColor;
		Float4 mCurrentColor;
		void * mUserData;
	};


}


