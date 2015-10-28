#include "MGUI_TabPage.h"
#include "MGUI_TabControl.h"

namespace Rad { namespace MGUI {

	TabPage::TabPage(const LookFeel * _lookfeel, TabControl * _parent)
		: Widget(_lookfeel, _parent)
	{
		mTabControl = _parent;
	}

	TabPage::~TabPage()
	{
	}

	void TabPage::UpdateRenderItem(Layout * layout)
	{
		Widget::OnUpdateWidgetState();

		if (!mTabControl->IsEnable())
		{
			mState = eWidgetState::DISABLE;
		}
	}

}}
