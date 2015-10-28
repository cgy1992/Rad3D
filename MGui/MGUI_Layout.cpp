#include "MGUI_Layout.h"
#include "MGUI_Helper.h"
#include "MGUI_Engine.h"
#include "MGUI_Loader.h"

namespace Rad { namespace MGUI {

	Layout * Layout::Load(const String & _filename, Widget * _parent)
	{
		Loader loader;

		return loader.Load(_filename, _parent);
	}

	ImplementRTTI(Layout, Widget);

	Layout::Layout(const LookFeel * _lookfeel, Widget * _parent)
		: Widget(_lookfeel, _parent)
	{
		if (mParent == NULL)
			Engine::Instance()->GetRootLayout()->_notifyAttach(this);
	}

	Layout::~Layout()
	{
	}

	RenderItem * Layout::GetRenderItem(int _order, Texture * _skin)
	{
		return mRenderLayer.RequestRenderItem(_order, _skin);
	}

	void Layout::UpdateRenderItem()
	{
		mRenderLayer.Clear();

		Widget::UpdateRenderItem(this);
	}

	void Layout::DoRender()
	{
		mRenderLayer.DoRender();
	}

}}

