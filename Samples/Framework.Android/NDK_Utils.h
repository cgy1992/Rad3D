#pragma once

#include <stdio.h>
#include <string.h>
#include <jni.h>

namespace NDK_Uitl {

	void 
		n_log(const char * fmt, ...);

	void 
		SetJavaVM(JavaVM * javaVM);
	JavaVM * 
		GetJavaVM();
	JNIEnv * 
		GetJNIEnv();

	void 
		SetCachePath(const char * path);
	const char * 
		GetCachePath();

	void 
		SetAssetManager(JNIEnv* env, jobject assetManager);
	int 
		ReadAPKFile(void ** data, const char * filename);
}
