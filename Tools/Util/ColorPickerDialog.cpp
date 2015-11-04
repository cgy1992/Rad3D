#include "ColorPickerDialog.h"

#define COLOR_TEXTURE_SIZE 32

ImplementSingleton(ColorPickerDialog);

ColorPickerDialog::ColorPickerDialog()
	: mBaseColor(0, 0, 0, 1)
	, mCurrentColor(0, 0, 0, 1)
	, mUserData(NULL)
{
	mDialog = new MGUI::Layout(NULL, NULL);
	mDialog->SetOrder(MGUI::eOrder::TOP - 100);
	mDialog->SetColor(Float4(0.15f, 0.15f, 0.15f, 0.5f));
	mDialog->SetAlign(MGUI::eAlign::STRETCH);
	mDialog->SetVisible(false);

	mLayout = MGUI::Layout::Load("ColorPicker.layout", mDialog);
	mLayout->SetAlign(MGUI::eAlign::CENTER);
	mLayout->SetInheritColor(false);
	mLayout->SetInheritAlpha(false);

	MGUI::Button * bnClose = (MGUI::Button * )mLayout->GetChild("Close");
	MGUI::Button * bnOK = (MGUI::Button *)mLayout->GetChild("OK");
	MGUI::Button * bnCancel = (MGUI::Button *)mLayout->GetChild("Cancel");

	bnClose->E_MouseClick += new cListener1<ColorPickerDialog, const MGUI::MouseEvent *>(this, &ColorPickerDialog::OnClose);
	bnOK->E_MouseClick += new cListener1<ColorPickerDialog, const MGUI::MouseEvent *>(this, &ColorPickerDialog::OnOK);
	bnCancel->E_MouseClick += new cListener1<ColorPickerDialog, const MGUI::MouseEvent *>(this, &ColorPickerDialog::OnCancel);

	mEditBox_S = (MGUI::EditBox *)mLayout->GetChild("S");
	mEditBox_RGBA = (MGUI::EditBox *)mLayout->GetChild("RGBA");
	mEditBox_RGBA->E_KeyLostFocus += new cListener1<ColorPickerDialog, const MGUI::FocusEvent *>(this, &ColorPickerDialog::OnColorTextChanged);

	mWidget_View = mLayout->GetChild("Viewer");

	mColorRange.PushBack(Float4(1, 0, 0));
	mColorRange.PushBack(Float4(1, 0, 1));
	mColorRange.PushBack(Float4(0, 0, 1));
	mColorRange.PushBack(Float4(0, 1, 1));
	mColorRange.PushBack(Float4(0, 1, 0));
	mColorRange.PushBack(Float4(1, 1, 0));
	mColorRange.PushBack(mColorRange[0]);

	mColorTexture = HWBufferManager::Instance()->NewTexture("ColorPickerDialog_Color", COLOR_TEXTURE_SIZE, COLOR_TEXTURE_SIZE, 0);
	mSliderTexture = HWBufferManager::Instance()->NewTexture("ColorPickerDialog_Slider", 4, 128, 0, ePixelFormat::R8G8B8);

	_genTexture();

	mImageBox_Color = new MGUI::ImageBox(NULL, mLayout->GetChild("Color"));
	mImageBox_Color->SetAlign(MGUI::eAlign::STRETCH);
	mImageBox_Color->SetSkinEx(mColorTexture);
	mImageBox_Color->SetUVRect(MGUI::Rect(0.01f, 0.01f, 0.98f, 0.98f));
	mImageBox_Color->E_MouseClick += new cListener1<ColorPickerDialog, const MGUI::MouseEvent *>(this, &ColorPickerDialog::OnPickColor);
	mImageBox_Color->E_MouseDrag += new cListener1<ColorPickerDialog, const MGUI::MouseEvent *>(this, &ColorPickerDialog::OnPickColor);

	mImageBox_Picker = new MGUI::ImageBox(NULL, mImageBox_Color);
	mImageBox_Picker->SetSkinAligned("ColorPicker.png");
	mImageBox_Picker->SetPickFlag(MGUI::ePickFlag::NONE);

	mImageBox_Slider = new MGUI::ImageBox(NULL, mLayout->GetChild("ColorSlider"));
	mImageBox_Slider->SetAlign(MGUI::eAlign::STRETCH);
	mImageBox_Slider->SetSkinEx(mSliderTexture);
	mImageBox_Slider->E_MouseClick += new cListener1<ColorPickerDialog, const MGUI::MouseEvent *>(this, &ColorPickerDialog::OnPickSlider);
	mImageBox_Slider->E_MouseDrag += new cListener1<ColorPickerDialog, const MGUI::MouseEvent *>(this, &ColorPickerDialog::OnPickSlider);

	mImageBox_SliderPicker = new MGUI::ImageBox(NULL, mLayout->GetChild("ColorSlider"));
	mImageBox_SliderPicker->SetAlign(MGUI::eAlign::H_CENTER);	
	mImageBox_SliderPicker->SetSkinAligned("ColorPicker.png");
	mImageBox_SliderPicker->SetPickFlag(MGUI::ePickFlag::NONE);
}

ColorPickerDialog::~ColorPickerDialog()
{
	delete mDialog;
}

Float4 ColorPickerDialog::GetCurrentColor()
{
	String text;
	text.FromUnicode(mEditBox_S->GetCaption().c_str());
	
	float s = text.ToFloat();
	s = Max(1.0f, s);

	Float4 c4 = mCurrentColor;
	c4.r *= s;
	c4.g *= s;
	c4.b *= s;

	return c4;
}

void ColorPickerDialog::DoModal(const Float4 & color, void * userData)
{
	d_assert (!mDialog->IsVisible());

	mUserData = userData;

	Float4 c4 = color;
	Float3 c3(color.r, color.g, color.b);
	float s = Max(1.0f, Max3(c3.x, c3.y, c3.z));
	String text = f2str(s, true, "%.2f");
	mEditBox_S->SetCaption(text.c_wstr());

	c4.r /= s;
	c4.g /= s;
	c4.b /= s;

	_updateFromColor(c4);

	mDialog->SetVisible(true);
}

void ColorPickerDialog::_genTexture()
{
	byte * pixels = (byte *)mSliderTexture->Lock(eLockFlag::WRITE);
	for (int i = 0; i < 128; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			float y = i / 127.0f  * 6.0f;

			int k0 = (int)y;
			int k1 = k0 < 6 ? k0 + 1 : k0;

			float d = y - k0;

			const Float4 & from = mColorRange[k0];
			const Float4 & to = mColorRange[k1];

			float r = from.r + d * (to.r - from.r);
			float g = from.g + d * (to.g - from.g);
			float b = from.b + d * (to.b - from.b);

			*pixels++ = (byte)(r * 255);
			*pixels++ = (byte)(g * 255);
			*pixels++ = (byte)(b * 255);
			//*pixels++ = 255;
		}
	}
	mSliderTexture->Unlock();
}

void ColorPickerDialog::_updateColor()
{
	Float4 color;
	color.r = mCurrentColor.r;
	color.g = mCurrentColor.g;
	color.b = mCurrentColor.b;
	color.a = mCurrentColor.a;

	String text;
	text.Format("%d %d %d %d", 
		(int)(color.r * 255), 
		(int)(color.g * 255),
		(int)(color.b * 255),
		(int)(color.a * 255));

	mEditBox_RGBA->SetCaption(text.c_wstr());

	mWidget_View->SetColor(color);
}

void ColorPickerDialog::_updateTexture(const Float4 & _color)
{
	int * pDest  = (int *)mColorTexture->Lock(eLockFlag::WRITE);

	for (size_t j = 0; j < COLOR_TEXTURE_SIZE; j++)
	{
		for (size_t i = 0; i < COLOR_TEXTURE_SIZE; i++)
		{
			float x = (float)i / COLOR_TEXTURE_SIZE;
			float y = (float)j / COLOR_TEXTURE_SIZE;

			float r = (1 - y) * (_color.r * x + (1 - x));
			float g = (1 - y) * (_color.g * x + (1 - x));
			float b = (1 - y) * (_color.b * x + (1 - x));

			*pDest++ = M_RGBA_F(r, g, b, 1);
		}
	}

	mColorTexture->Unlock();
}

void ColorPickerDialog::_updateFromColor(const Float4 & color)
{
	mCurrentColor = color;

	Array<float> vec;
	vec.PushBack(mCurrentColor.r);
	vec.PushBack(mCurrentColor.g);
	vec.PushBack(mCurrentColor.b);
	Sort(&vec[0], 3, t_compare<float>());

	MGUI::Rect rect = mImageBox_Color->GetRect();
	MGUI::Rect myRect = mImageBox_Picker->GetRect();

	float px = Math::Clamp(1 - vec[0] / (vec[2] + 0.000001f), 0.0f, 1.0f) * rect.w;
	float py = (1 - vec[2]) * rect.h;

	myRect.x = px - myRect.w / 2;
	myRect.y = py - myRect.h / 2;

	mImageBox_Picker->SetRect(myRect);

	int iMax = (mCurrentColor.r == vec[2]) ? 0 : (mCurrentColor.g == vec[2]) ? 1 : 2;
	int iMin = (mCurrentColor.r == vec[0]) ? 0 : (mCurrentColor.g == vec[0]) ? 1 : 2;
	int iAvg = 3 - iMax - iMin;

	if (iMin == iMax) // if gray Color - set base red
	{
		iMax = 0;
		iMin = 1;
		iAvg = 2;
		byIndex(mBaseColor, iMin) = 0.;
		byIndex(mBaseColor, iAvg) = 0.;
		byIndex(mBaseColor, iMax) = 1.;
	}
	else
	{
		byIndex(mBaseColor, iMin) = 0.;
		byIndex(mBaseColor, iAvg) = (vec[1] - vec[0]) / (vec[2] - vec[0]);
		byIndex(mBaseColor, iMax) = 1.;
	}

	int i;
	for (i = 0; i < 6; ++i)
	{
		if ((fabs(byIndex(mColorRange[i], iMin) - byIndex(mBaseColor, iMin)) < 0.001) &&
			(fabs(byIndex(mColorRange[i], iMax) - byIndex(mBaseColor, iMax)) < 0.001) &&
			(fabs(byIndex(mColorRange[i+1], iMin) - byIndex(mBaseColor, iMin)) < 0.001) &&
			(fabs(byIndex(mColorRange[i+1], iMax) - byIndex(mBaseColor, iMax)) < 0.001))
			break;
	}

	
	float offset = byIndex(mBaseColor, iAvg);
	if (byIndex(mColorRange[i+1], iAvg) < byIndex(mColorRange[i], iAvg)) offset = 1 - byIndex(mBaseColor, iAvg);

	// float sector_size = (float)255.0 / 6.0f;
	// size_t current = i;
	// size_t pos = size_t((current + offset) * sector_size);
	// mScrollRange->setScrollPosition(pos);
	MGUI::Rect sliderRect = mImageBox_Slider->GetRect();
	float y = (i + offset) / 6 * sliderRect.h;
	mImageBox_SliderPicker->SetRectPosition(0, y - mImageBox_SliderPicker->GetRect().h / 2);

	mBaseColor.r = mColorRange[i].r + offset * (mColorRange[i+1].r - mColorRange[i].r);
	mBaseColor.g = mColorRange[i].g + offset * (mColorRange[i+1].g - mColorRange[i].g);
	mBaseColor.b = mColorRange[i].b + offset * (mColorRange[i+1].b - mColorRange[i].b);

	_updateTexture(mBaseColor);

	_updateColor();

	E_Preview();
}

void ColorPickerDialog::_updateFromPoint(float x, float y)
{
	MGUI::Rect rect = mImageBox_Color->GetRect();

	x = 1.0f * x / rect.w;
	y = 1.0f * y / rect.h;

	if (x > 1) x = 1;
	else if (x < 0) x = 0;
	if (y > 1) y = 1;
	else if (y < 0) y = 0;

	mCurrentColor.r = (1 - y) * (mBaseColor.r * x + 1 * (1 - x));
	mCurrentColor.g = (1 - y) * (mBaseColor.g * x + 1 * (1 - x));
	mCurrentColor.b = (1 - y) * (mBaseColor.b * x + 1 * (1 - x));

	_updateColor();

	E_Preview();
}

float& ColorPickerDialog::byIndex(Float4 & _color, size_t _index)
{
	if (_index == 0) return _color.r;
	else if (_index == 1) return _color.g;
	else if (_index == 2) return _color.b;
	else return _color.a;
}

void ColorPickerDialog::OnClose(const MGUI::MouseEvent *)
{
	OnCancel(NULL);
}

void ColorPickerDialog::OnOK(const MGUI::MouseEvent *)
{
	E_EndDialog(true);
	
	E_EndDialog.DetachAll();
	E_Preview.DetachAll();

	mDialog->SetVisible(false);

}

void ColorPickerDialog::OnCancel(const MGUI::MouseEvent *)
{
	E_EndDialog(false);

	E_EndDialog.DetachAll();
	E_Preview.DetachAll();

	mDialog->SetVisible(false);
}

void ColorPickerDialog::OnPickColor(const MGUI::MouseEvent * e)
{
	MGUI::Rect rect = mImageBox_Color->GetAbsRect();
	MGUI::Rect myRect = mImageBox_Picker->GetRect();

	float x = Math::Clamp(e->x - rect.x, 0.0f, rect.w);
	float y = Math::Clamp(e->y - rect.y, 0.0f, rect.h);

	myRect.x = x - myRect.w / 2;
	myRect.y = y - myRect.h / 2;

	mImageBox_Picker->SetRect(myRect);

	_updateFromPoint(x, y);
}

void ColorPickerDialog::OnPickSlider(const MGUI::MouseEvent * e)
{
	MGUI::Rect rect = mImageBox_Slider->GetAbsRect();
	MGUI::Rect myRect = mImageBox_SliderPicker->GetRect();

	float x = Math::Clamp(e->x - rect.x, 0.0f, rect.w);
	float y = Math::Clamp(e->y - rect.y, 0.0f, rect.h);

	myRect.x = x - myRect.w / 2;
	myRect.y = y - myRect.h / 2;
	mImageBox_SliderPicker->SetRect(myRect);

	y = y / rect.h  * 6.0f;

	int k0 = (int)y;
	int k1 = k0 < 6 ? k0 + 1 : k0;

	float d = y - k0;

	const Float4 & from = mColorRange[k0];
	const Float4 & to = mColorRange[k1];

	mBaseColor.r = from.r + d * (to.r - from.r);
	mBaseColor.g = from.g + d * (to.g - from.g);
	mBaseColor.b = from.b + d * (to.b - from.b);

	_updateTexture(mBaseColor);

	x = mImageBox_Picker->GetRect().x + mImageBox_Picker->GetRect().w / 2;
	y = mImageBox_Picker->GetRect().y + mImageBox_Picker->GetRect().h / 2;

	_updateFromPoint(x, y);
}

void ColorPickerDialog::OnColorTextChanged(const MGUI::FocusEvent *)
{
	String text;
	text.FromUnicode(mEditBox_RGBA->GetCaption().c_str());

	const char * str = text.c_str();
	Float4 color;
	str = str_getfloat(color.r, str);
	str = str_getfloat(color.g, str);
	str = str_getfloat(color.b, str);
	str = str_getfloat(color.a, str);
	color *= 1 / 255.0f;
	color.Saturate();

	if (mCurrentColor != color)
		_updateFromColor(color);
}
