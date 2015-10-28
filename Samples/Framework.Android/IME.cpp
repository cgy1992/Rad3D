#include "IME.h"
#include "NDK_Utils.h"

void IME::OpenIMEKeyboard()
{
	JNIEnv* env = NDK_Uitl::GetJNIEnv();
	
	if (!env)
		return ;

	jclass glview = env->FindClass( "com/myway/MGLView");

	if (!glview)
	{
		d_printf("GLView class can't find.");
		return ;
	}

	jmethodID fn = env->GetStaticMethodID(glview, "OpenKeyboard", "()V");

	if (!fn)
	{
		d_printf("OpenKeyboard Function can't find in class MGLView.");
		return ;
	}

	env->CallStaticVoidMethod(glview, fn);

	env->DeleteLocalRef(glview);
}

void IME::CloseIMEKeyboard()
{
	JNIEnv* env = NDK_Uitl::GetJNIEnv();

	jclass glview = env->FindClass( "com/myway/MGLView");

	if (!glview)
	{
		d_printf("GLView class can't find.");
		return ;
	}

	jmethodID fn = env->GetStaticMethodID(glview, "CloseKeyboard", "()V");

	if (!fn)
	{
		d_printf("CloseKeyboard Function can't find in class MGLView.");
		return ;
	}

	env->CallStaticVoidMethod(glview, fn);

	env->DeleteLocalRef(glview);
}


IME::IME()
{
	MGUI::InputManager::Instance()->E_OpenIMEKeyboard += L_OpenIMEKeyboard(this, &IME::OnOpenIMEKeyboard);
	MGUI::InputManager::Instance()->E_CloseIMEKeyboard += L_CloseIMEKeyboard(this, &IME::OnCloseIMEKeyboard);

	bOpenKeyboard = false;
}

IME::~IME()
{
	MGUI::InputManager::Instance()->E_OpenIMEKeyboard -= L_OpenIMEKeyboard(this, &IME::OnOpenIMEKeyboard);
	MGUI::InputManager::Instance()->E_CloseIMEKeyboard -= L_CloseIMEKeyboard(this, &IME::OnCloseIMEKeyboard);
}

void IME::OnOpenIMEKeyboard()
{
	OpenIMEKeyboard();

	bOpenKeyboard = true;
}

void IME::OnCloseIMEKeyboard()
{
	CloseIMEKeyboard();

	bOpenKeyboard = true;
}