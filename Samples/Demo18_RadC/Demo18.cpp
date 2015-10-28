#include "App.h"
#include "RadC.h"

FixedString<1024> script = "\n\
function main(real a, real b) \n\
	string e = test(a, b) \n\
	return e \n\
end ";


bool test(radc_var * result, const radc_varlist * args)
{
	if (args->size() == 2)
	{
		radc_real a = args->getReal(0);
		radc_real b = args->getReal(1);

		result->setString("Hello RadC!");

		return true;
	}

	return false;
}


class Demo18 : public App
{
	MGUI::Layout * mLayout;
	MGUI::EditView * mScript;
	MGUI::EditView * mOutput;
	MGUI::EditBox * mArg1;
	MGUI::EditBox * mArg2;

public:
	Demo18() {}
	virtual ~Demo18() {}

	virtual void OnSetupResource()
	{
#ifdef M_PLATFORM_WIN32
		ResourceManager::Instance()->AddArchive(new FilePathArchive("../Core"));
		ResourceManager::Instance()->AddArchive(new FilePathArchive("../Sample"));
#endif

#ifdef M_PLATFORM_ANDROID
		ResourceManager::Instance()->AddArchive(new APKArchive("Core"));
		ResourceManager::Instance()->AddArchive(new APKArchive("Sample"));
#endif
	}

	virtual void OnInit()
	{
		CharSet::SetCharSet("gb2312");

		World::Instance()->MainRenderContext()->SetColorClear(eClearMode::ALL, Float4(0.1f, 0.1f, 0.3f));

		MGUI::FontManager::Instance()->Load("Sample.font");
		MGUI::LookFeelManager::Instance()->Load("Sample.lookfeel");

		mLayout = MGUI::Layout::Load("ScriptTest.layout", NULL);
		d_assert (mLayout != NULL);

		mScript = mLayout->GetChildT<MGUI::EditView>("Script");
		mOutput = mLayout->GetChildT<MGUI::Widget>("Editor")->GetChildT<MGUI::EditView>("Output");
		mArg1 = mLayout->GetChildT<MGUI::Widget>("Editor")->GetChildT<MGUI::EditBox>("arg1");
		mArg2 = mLayout->GetChildT<MGUI::Widget>("Editor")->GetChildT<MGUI::EditBox>("arg2");

		mScript->SetCaption(script.c_wstr());
		mOutput->SetStatic(true);
		mArg1->SetCaption(WS("1"));
		mArg2->SetCaption(WS("2"));

		MGUI::Button * bnRun = mLayout->GetChildT<MGUI::Widget>("Editor")->GetChildT<MGUI::Button>("Run");
		bnRun->E_Click += new cListener1<Demo18, const MGUI::ClickEvent *>(this, &Demo18::OnRun);

		radc_regFunc("test", test);
	}

	virtual void OnUpdate()
	{
	}

	virtual void OnShutdown()
	{
	}

	virtual void OnPause()
	{
	}

	virtual void OnResume()
	{
	}

	void OnRun(const MGUI::ClickEvent * e)
	{
		FixedString<1024> _script;
		String _arg1, _arg2;
		
		_script.FromUnicode(mScript->GetCaption().c_str());
		_arg1.FromUnicode(mArg1->GetCaption().c_str());
		_arg2.FromUnicode(mArg2->GetCaption().c_str());

		radc_varlist vlist;
		vlist.push(_arg1.ToFloat());
		vlist.push(_arg2.ToFloat());
		if (radc_do(_script.c_str(), 0, &vlist))
		{
			String text; 
			const radc_varlist * vl = radc_result();

			if (vl->size() > 0)
			{
				if (vl->getType(0) == radc_enum::type_real)
				{
					text = f2str((float)vl->getReal(0), true);
				}
				else if (vl->getType(0) == radc_enum::type_string)
				{
					text = vl->getString(0);
				}
			}

			mOutput->SetCaption(text.c_wstr());
		}
		else
		{
			String err = radc_getError();
			mOutput->SetCaption(err.c_wstr());
		}
	}
};

void CreateApplication(App ** ppApp)
{
	*ppApp = new Demo18;
}
