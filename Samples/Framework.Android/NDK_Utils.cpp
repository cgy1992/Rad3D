#include "NDK_Utils.h"
#include <stdarg.h>
#include <assert.h>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

namespace NDK_Uitl {

	void n_log(const char * fmt, ...)
	{
		char text[1024];

		va_list arglist;

		va_start(arglist, fmt);
		vsprintf(text, fmt, arglist);
		va_end(arglist);

		__android_log_write(ANDROID_LOG_ERROR, "Myway", text);
	}

	//
	JavaVM * gJavaVM = NULL;

	void SetJavaVM(JavaVM * javaVM)
	{
		gJavaVM = javaVM;
	}

	JavaVM * GetJavaVM()
	{
		return gJavaVM;
	}

	JNIEnv * GetJNIEnv()
	{
		if (!gJavaVM)
		{
			n_log("?: Java VM empty!");
			return NULL;
		}

		JNIEnv * env = NULL;
		int err = gJavaVM->GetEnv((void**)&env, JNI_VERSION_1_4);

		if (err == JNI_EDETACHED)
		{
			if (gJavaVM->AttachCurrentThread(&env, 0) != 0)
			{
				n_log("?: AttachCurrentThread().");
			}
		}
		else if (err != JNI_OK)
		{
			n_log("?: GetJNIEnv.");
		}
		
		return env;
	}


	//
	char gCachePath[256] = { 0 };

	void SetCachePath(const char * path)
	{
		strcpy(gCachePath, "/data/data/");
		strcat(gCachePath, path);
	}

	const char * GetCachePath()
	{
		return gCachePath;
	}


	//
	AAssetManager * gAssetManager = NULL;

	void SetAssetManager(JNIEnv* env, jobject assetManager)
	{
		n_log("-: Setup AssetManager.");
		gAssetManager = AAssetManager_fromJava(env, assetManager);
	}

	int ReadAPKFile(void ** data, const char * filename)
	{
		assert (gAssetManager != NULL);

		AAsset * pAsset = AAssetManager_open(gAssetManager, filename, AASSET_MODE_UNKNOWN);
		if (pAsset == NULL)
			return -1;

		size_t size = AAsset_getLength(pAsset);
		if (size > 0 && data != NULL)
		{
			unsigned char * pd = new unsigned char[size];
			int read_count = AAsset_read(pAsset, pd, size);
			if(read_count <= 0)
			{
				n_log("?: Read APK File '%s'.", filename);

				delete[] pd;
				pd = NULL;
				size = 0;
			}

			*data = pd;
		}

		AAsset_close(pAsset);
		
		return size;
	}

}