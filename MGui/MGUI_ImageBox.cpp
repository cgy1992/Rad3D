#include "MGUI_ImageBox.h"
#include "MGUI_Layout.h"
#include "MGUI_Engine.h"

namespace Rad { namespace MGUI {

	ImplementRTTI(ImageBox, Widget);

	ImageBox::ImageBox(const LookFeel * _lookfeel, Widget * _parent)
		: Widget(NULL, _parent)
		, mUVRect(0, 0, 1, 1)
	{
	}

	ImageBox::~ImageBox()
	{
	}

	void ImageBox::SetSkin(const String & _skin)
	{
		mSkin = HWBufferManager::Instance()->LoadTexture(_skin, -1);
		mSkinName = _skin;
	}

	void ImageBox::SetSkinEx(TexturePtr _skin)
	{
		mSkin = _skin;
		mSkinName = _skin != NULL ? _skin->GetName() : "";
	}

	const String & ImageBox::GetSkinName() const
	{
		return mSkinName;
	}

	void ImageBox::SetSkinAligned(const String & _skin, const Rect & _uv)
	{
		TexturePtr skin = HWBufferManager::Instance()->LoadTexture(_skin, -1);

		SetSkinAlignedEx(skin, _uv);
	}

	void ImageBox::SetSkinAlignedEx(TexturePtr _skin, const Rect & _uv)
	{
		mSkin = _skin;
		mUVRect = _uv;

		Rect rect;

		rect.x = mRect.x;
		rect.y = mRect.y;

		if (_skin != NULL)
		{
			rect.w = Math::Abs(mUVRect.w) * (float)mSkin->GetWidth();
			rect.h = Math::Abs(mUVRect.h) * (float)mSkin->GetHeight();
		}
		else
		{
			rect.w = 0;
			rect.h = 0;
		}

		SetRect(rect);
	}

	void ImageBox::SetUVRect(const Rect & _uv)
	{
		mUVRect = _uv;
	}

	void ImageBox::UpdateRenderItem(Layout * _layout)
	{
		if (!mVisible)
			return ;

		if (mSkin == NULL)
			return ;

		Rect clipRect = mParent->GetClipRect();
		Rect _rect, _uv;

		if (Helper::Instance()->Clip(_rect, _uv, mAbsRect, mUVRect, clipRect))
		{
			RenderItem * ri = _layout->GetRenderItem(GetAbsOrder(), GetSkin().c_ptr());

			ri->AddQuad(Helper::Instance()->GetQuad(_rect, _uv,  mAbsColor));
		}

		for (int i = 0; i < mChildren.Size(); ++i)
		{
			Widget * child = GetChild(i);

			child->UpdateRenderItem(_layout);
		}
	}

	bool ImageBox::_LookupPickMask(float _x, float _y)
	{
		if (!mPickMaskEnable)
			return true;

		if (mSkin != NULL)
		{
			const Rect & uvRect = mLookFeel->GetUVRect(mState);

			float u = mUVRect.x * _x;
			float v = mUVRect.y * _y;

			u *= mSkin->GetWidth();
			v *= mSkin->GetHeight();

			Float4 color;
			mSkin->GetColorData(color, (int)u, (int)v);

			return color.r > EPSILON_E4;
		}

		return false;
	}

	void ImageBox::SerializeXML(xml_node root, bool is_out)
	{
		Widget::SerializeXML(root, is_out);

		if (is_out)
		{
			if (mSkin != NULL)
			{
				root.append_node("Skin").append_attribute("value", GetSkinName().c_str());
			}

			if (mUVRect != Rect(0, 0, 1, 1))
			{
				String str;
				str += f2str(mUVRect.x, true) + " ";
				str += f2str(mUVRect.y, true) + " ";
				str += f2str(mUVRect.w, true) + " ";
				str += f2str(mUVRect.h, true);

				root.append_node("UVRect").append_attribute("value", str.c_str());
			}
		}
		else
		{
			// Skin
			xml_node node = root.first_node("Skin");
			if (node != NULL)
			{
				String str = node.first_attribute("value");

				SetSkin(str);
			}

			// UVRect
			node = root.first_node("UVRect");
			if (node != NULL)
			{
				Rect rect;
				const char * str = node.first_attribute("value");
				str = str_getfloat(rect.x, str);
				str = str_getfloat(rect.y, str);
				str = str_getfloat(rect.w, str);
				str = str_getfloat(rect.h, str);

				SetUVRect(rect);
			}
		}
	}
}}
