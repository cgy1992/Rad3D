#include <GLES/gl.h>  
#include <GLES/glext.h>
#include <string.h>
#include <jni.h>

#include "NDK_Utils.h"
#include "App.h"

App * gApp = NULL;

extern "C" {

	//
	const char * js2s(JNIEnv * env, jstring str)
	{
		jboolean isCopy;
		return env->GetStringUTFChars(str, &isCopy);
	}

	const uchar_t * js2ws(JNIEnv * env, jstring str)
	{
		static uchar_t wstr[1024 + 1];

		jboolean isCopy;
		const jchar * jstr = env->GetStringChars(str, &isCopy);
		int len = env->GetStringLength(str);

		for (int i = 0; i < len && i < 1024; ++i)
		{
			wstr[i] = jstr[i];
		}

		wstr[len] = 0;

		return wstr;
	}

	jstring ws2js(JNIEnv * env,  const uchar_t * wstr)
	{
		static jchar jstr[1024 + 1];

		int len = 0;
		while(*wstr && len < 1024)
		{
			jstr[len] = wstr[len];
			len++;
		}
		jstr[len] = 0;

		return env->NewString(jstr, len);
	}

	JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
	{
		NDK_Uitl::SetJavaVM(vm);

		return JNI_VERSION_1_4;
	}

	JNIEXPORT void JNICALL Java_com_myway_Framework_OnInit(JNIEnv * env, jclass thiz, jobject assetMgr, jstring cachePath, jint w, jint h)
	{
		NDK_Uitl::SetAssetManager(env, assetMgr);
		NDK_Uitl::SetCachePath(js2s(env, cachePath));

		CreateApplication(&gApp);
		gApp->SetCachePath(NDK_Uitl::GetCachePath());
		gApp->Init(w, h);
	}

	JNIEXPORT void JNICALL Java_com_myway_Framework_OnShutdown(JNIEnv * env, jobject thiz)
	{
		if (gApp != NULL)
		{
			gApp->Shutdown();
			delete gApp;
			gApp = NULL;
		}
	}

	JNIEXPORT void JNICALL Java_com_myway_Framework_OnRun(JNIEnv * env, jobject thiz, jboolean keyfocus)
	{
		if (gApp != NULL)
		{
			gApp->Update();
		}
	}

	JNIEXPORT void JNICALL Java_com_myway_Framework_OnResume(JNIEnv * env, jobject thiz)
	{
		if (gApp != NULL)
		{
			gApp->Resume();
		}
	}

	JNIEXPORT void JNICALL Java_com_myway_Framework_OnPause(JNIEnv * env, jobject thiz)
	{
		if (gApp != NULL)
		{
			gApp->Pause();
		}
	}

	JNIEXPORT void JNICALL Java_com_myway_Framework_OnResize(JNIEnv * env, jobject thiz, jint w, jint h)
	{
		if (gApp != NULL)
		{
			gApp->Resize(w, h);
		}
	}


	// Event
	JNIEXPORT void JNICALL Java_com_myway_Framework_OnActionDown(JNIEnv * env, jobject thiz, jint id, jfloat x, jfloat y)
	{
		gApp->InjectTouchDown(id, x, y);
	}

	JNIEXPORT void JNICALL Java_com_myway_Framework_OnActionUp(JNIEnv * env, jobject thiz, jint id, jfloat x, jfloat y)
	{
		gApp->InjectTouchUp(id, x, y);
	}

	JNIEXPORT void JNICALL Java_com_myway_Framework_OnActionMove(JNIEnv * env, jobject thiz, jint id, jfloat x, jfloat y)
	{
		gApp->InjectTouchMove(id, x, y);
	}

	JNIEXPORT void JNICALL Java_com_myway_Framework_OnActionCancel(JNIEnv * env, jobject thiz, jint id, jfloat x, jfloat y)
	{
		gApp->InjectTouchCancel(id, x, y);
	}

	JNIEXPORT void JNICALL Java_com_myway_Framework_OnInsertText(JNIEnv * env, jobject thiz, jstring text)
	{
		const uchar_t * wstr = js2ws(env, text);
		while (*wstr)
		{
			uchar_t ch = *wstr++;

			if (ch == L'\n')
			{
				gApp->InjectKeyDown(InputCode::KC_ENTER, 0);
				break;
			}
			else
			{
				gApp->InjectKeyDown(InputCode::KC_NONE, ch);
			}
		}
	}

	JNIEXPORT void JNICALL Java_com_myway_Framework_OnDeleteBackword(JNIEnv * env, jobject thiz)
	{
		gApp->InjectKeyDown(InputCode::KC_DELETE, 0);
	}
};

