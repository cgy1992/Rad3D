#include "MGUI_Window.h"
#include "MGUI_Loader.h"

namespace Rad { namespace MGUI {

	Window::Window(const String & filename, Widget * parent)
		: mParent(parent)
	{
		mSource = new ResourceDataStream(filename, filename);
		mSource->E_Loaded += new cListener0<Window>(this, &Window::OnResourceLoaded);
		mSource->E_Unload += new cListener0<Window>(this, &Window::OnResourceUnload);
	}

	Window::~Window()
	{
		mSource = NULL;
	}

	void Window::Load()
	{
		mSource->Load();
	}

	void Window::Unload()
	{
		mSource->Unload();
	}

	void Window::OnResourceLoaded()
	{
		DataStreamPtr stream = mSource->GetDataStream();
		if (stream != NULL)
		{
			Loader loader;

			mLayout = loader.Load(stream, NULL);

			d_assert(mLayout != NULL);

			OnLoaded();
		}
	}

	void Window::OnResourceUnload()
	{
		OnUnload();

		safe_delete(mLayout);
	}
}}