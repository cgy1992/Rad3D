#include <jni.h>
#include <errno.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android_native_app_glue.h>

#include <EGL/egl.h>

#include "App.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

struct engine {
	struct android_app* app;

	ASensorManager* sensorManager;
	const ASensor* accelerometerSensor;
	ASensorEventQueue* sensorEventQueue;

	EGLDisplay display;
	EGLSurface surface;
	EGLContext context;
	int32_t width;
	int32_t height;
};

ANativeActivity * ANDROID_ACTIVITY = NULL;
const char * DATA_PATH = NULL;
App * gApp = NULL;
bool gInit = false;

void OnOpenKeyboard();
void OnCloseKeyboard();

static int engine_init_display(struct engine* engine)
{
    EGLint w, h, dummy, format;
    EGLint numConfigs;
    EGLConfig config;
    EGLSurface surface;
    EGLContext context;

    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(display, 0, 0);

	const EGLint attribs[] = {
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_NONE
	};
    eglChooseConfig(display, attribs, &config, 1, &numConfigs);

    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

    ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);

    surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);

	EGLint attrib_list[] = {
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};
    context = eglCreateContext(display, config, NULL, attrib_list);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        LOGW("Unable to eglMakeCurrent");
        return -1;
    }

    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);

    engine->display = display;
    engine->context = context;
    engine->surface = surface;
    engine->width = w;
    engine->height = h;

	if (!gInit)
	{
		gApp->Init(w, h);
		
		MGUI::InputManager::Instance()->E_OpenIMEKeyboard += new ncListener0(OnOpenKeyboard);
		MGUI::InputManager::Instance()->E_CloseIMEKeyboard += new ncListener0(OnCloseKeyboard);

		gInit = true;
	}
	else
	{
		gApp->Resume();
	}

    return 0;
}

static void engine_term_display(struct engine* engine)
{
	d_printf("term display");

	if (gInit)
		gApp->Pause();

	if (engine->display != EGL_NO_DISPLAY)
	{
		eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

		if (engine->context != EGL_NO_CONTEXT) 
			eglDestroyContext(engine->display, engine->context);

		if (engine->surface != EGL_NO_SURFACE) 
			eglDestroySurface(engine->display, engine->surface);
		
		eglTerminate(engine->display);
	}

	engine->display = EGL_NO_DISPLAY;
	engine->context = EGL_NO_CONTEXT;
	engine->surface = EGL_NO_SURFACE;
}

static void engine_draw_frame(struct engine* engine)
{
	if (engine->display == NULL)
		return;

	gApp->Update();

	eglSwapBuffers(engine->display, engine->surface);
}

// android not send AKEYCODE_DEL to native app, so we use java editbox
#if 0
void display_keyboard(bool bshow)
{
	// Attaches the current thread to the JVM.
	jint lResult;
	jint lFlags = 0;

	JavaVM* lJavaVM = ANDROID_ACTIVITY->vm;
	JNIEnv* lJNIEnv = ANDROID_ACTIVITY->env;

	JavaVMAttachArgs lJavaVMAttachArgs;
	lJavaVMAttachArgs.version = JNI_VERSION_1_6;
	lJavaVMAttachArgs.name = "NativeThread";
	lJavaVMAttachArgs.group = NULL;

	lResult=lJavaVM->AttachCurrentThread(&lJNIEnv, &lJavaVMAttachArgs);
	if (lResult == JNI_ERR)
		return;

	// Retrieves NativeActivity.
	jobject lNativeActivity = ANDROID_ACTIVITY->clazz;
	jclass ClassNativeActivity = lJNIEnv->GetObjectClass(lNativeActivity);

	// Retrieves Context.INPUT_METHOD_SERVICE.
	jclass ClassContext = lJNIEnv->FindClass("android/content/Context");
	jfieldID FieldINPUT_METHOD_SERVICE =
		lJNIEnv->GetStaticFieldID(ClassContext,
		"INPUT_METHOD_SERVICE", "Ljava/lang/String;");
	jobject INPUT_METHOD_SERVICE =
		lJNIEnv->GetStaticObjectField(ClassContext,
		FieldINPUT_METHOD_SERVICE);
	if (INPUT_METHOD_SERVICE == NULL)
		return ;

	// Runs getSystemService(Context.INPUT_METHOD_SERVICE).
	jclass ClassInputMethodManager = lJNIEnv->FindClass(
		"android/view/inputmethod/InputMethodManager");
	jmethodID MethodGetSystemService = lJNIEnv->GetMethodID(
		ClassNativeActivity, "getSystemService",
		"(Ljava/lang/String;)Ljava/lang/Object;");
	jobject lInputMethodManager = lJNIEnv->CallObjectMethod(
		lNativeActivity, MethodGetSystemService,
		INPUT_METHOD_SERVICE);

	// Runs getWindow().getDecorView().
	jmethodID MethodGetWindow = lJNIEnv->GetMethodID(
		ClassNativeActivity, "getWindow",
		"()Landroid/view/Window;");
	jobject lWindow = lJNIEnv->CallObjectMethod(lNativeActivity,
		MethodGetWindow);
	jclass ClassWindow = lJNIEnv->FindClass(
		"android/view/Window");
	jmethodID MethodGetDecorView = lJNIEnv->GetMethodID(
		ClassWindow, "getDecorView", "()Landroid/view/View;");
	jobject lDecorView = lJNIEnv->CallObjectMethod(lWindow,
		MethodGetDecorView);

	if (bshow) {
		// Runs lInputMethodManager.showSoftInput(...).
		jmethodID MethodShowSoftInput = lJNIEnv->GetMethodID(
			ClassInputMethodManager, "showSoftInput",
			"(Landroid/view/View;I)Z");
		jboolean lResult = lJNIEnv->CallBooleanMethod(
			lInputMethodManager, MethodShowSoftInput,
			lDecorView, lFlags);
	} else {
		// Runs lWindow.getViewToken()
		jclass ClassView = lJNIEnv->FindClass(
			"android/view/View");
		jmethodID MethodGetWindowToken = lJNIEnv->GetMethodID(
			ClassView, "getWindowToken", "()Landroid/os/IBinder;");
		jobject lBinder = lJNIEnv->CallObjectMethod(lDecorView,
			MethodGetWindowToken);

		// lInputMethodManager.hideSoftInput(...).
		jmethodID MethodHideSoftInput = lJNIEnv->GetMethodID(
			ClassInputMethodManager, "hideSoftInputFromWindow",
			"(Landroid/os/IBinder;I)Z");
		jboolean lRes = lJNIEnv->CallBooleanMethod(
			lInputMethodManager, MethodHideSoftInput,
			lBinder, lFlags);
	}

	// Finished with the JVM.
	lJavaVM->DetachCurrentThread();
}

int GetUnicodeChar(int eventType, int keyCode, int metaState)
{
	JavaVM* javaVM = ANDROID_ACTIVITY->vm;
	JNIEnv* jniEnv = ANDROID_ACTIVITY->env;

	JavaVMAttachArgs attachArgs;
	attachArgs.version = JNI_VERSION_1_6;
	attachArgs.name = "NativeThread";
	attachArgs.group = NULL;

	jint result = javaVM->AttachCurrentThread(&jniEnv, &attachArgs);
	if(result == JNI_ERR)
		return 0;

	jclass class_key_event = jniEnv->FindClass("android/view/KeyEvent");
	int unicodeKey;

	if(metaState == 0)
	{
		jmethodID method_get_unicode_char = jniEnv->GetMethodID(class_key_event, "getUnicodeChar", "()I");
		jmethodID eventConstructor = jniEnv->GetMethodID(class_key_event, "<init>", "(II)V");
		jobject eventObj = jniEnv->NewObject(class_key_event, eventConstructor, eventType, keyCode);

		unicodeKey = jniEnv->CallIntMethod(eventObj, method_get_unicode_char);
	}
	else
	{
		jmethodID method_get_unicode_char = jniEnv->GetMethodID(class_key_event, "getUnicodeChar", "(I)I");
		jmethodID eventConstructor = jniEnv->GetMethodID(class_key_event, "<init>", "(II)V");
		jobject eventObj = jniEnv->NewObject(class_key_event, eventConstructor, eventType, keyCode);

		unicodeKey = jniEnv->CallIntMethod(eventObj, method_get_unicode_char, metaState);
	}

	javaVM->DetachCurrentThread();

	return unicodeKey;
}

void OnOpenKeyboard()
{
	display_keyboard(true);
}

void OnCloseKeyboard()
{
	display_keyboard(false);
}

#else

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

jclass JC_EDIT_TEXT = NULL;

extern "C" {

	JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
	{
		d_printf("JNI_OnLoad");

		return JNI_VERSION_1_4;
	}

	JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void* reserved)
	{
		d_printf("JNI_OnUnload");
	}

	JNIEXPORT void JNICALL Java_com_rad3d_Framework_OnInit(JNIEnv * env, jclass thiz)
	{
		JC_EDIT_TEXT = env->FindClass( "com/rad3d/MEditText");
		if (JC_EDIT_TEXT == NULL)
			d_printf("JC_EDIT_TEXT == NULL");
	}

	JNIEXPORT void JNICALL Java_com_rad3d_Framework_OnInsertText(JNIEnv * env, jobject thiz, jstring text)
	{
		const uchar_t * wstr = js2ws(env, text);
		while (*wstr)
		{
			uchar_t ch = *wstr++;
			if (ch == WC('\n'))
			{
				App::Instance()->InjectKeyDown(InputCode::KC_ENTER, 0);
				break;
			}
			else
			{
				App::Instance()->InjectKeyDown(InputCode::KC_NONE, ch);
			}
		}
	}

	JNIEXPORT void JNICALL Java_com_rad3d_Framework_OnDeleteBackword(JNIEnv * env, jobject thiz)
	{
		App::Instance()->InjectKeyDown(InputCode::KC_DELETE, 0);
	}
}

JNIEnv * GetJNIEnv()
{
	JavaVM * vm = ANDROID_ACTIVITY->vm;
	JNIEnv * env = NULL;
	int err = ANDROID_ACTIVITY->vm->GetEnv((void**)&env, JNI_VERSION_1_4);
	if (err == JNI_EDETACHED)
	{
		if (vm->AttachCurrentThread(&env, 0) != 0)
			d_log("?: AttachCurrentThread().");
	}
	else if (err != JNI_OK)
	{
		d_log("?: GetJNIEnv.");
	}

	return env;
}

void OnOpenKeyboard()
{
	JNIEnv * jenv = GetJNIEnv();
	if (jenv == NULL)
		return ;

	if (JC_EDIT_TEXT == NULL)
	{
		d_log("JC_EDIT_TEXT is null.");
		return ;
	}

	jmethodID fn = jenv->GetStaticMethodID(JC_EDIT_TEXT, "OpenKeyboard", "()V");
	if (!fn)
		return ;

	jenv->CallStaticVoidMethod(JC_EDIT_TEXT, fn);
}

void OnCloseKeyboard()
{
	JNIEnv * jenv = GetJNIEnv();
	if (jenv == NULL)
		return ;

	jclass editbox = jenv->FindClass( "com/rad3d/MEditText");
	if (!editbox)
		return ;

	jmethodID fn = jenv->GetStaticMethodID(editbox, "CloseKeyboard", "()V");
	if (!fn)
		return ;

	jenv->CallStaticVoidMethod(editbox, fn);
}

#endif 

static int32_t engine_handle_input(struct android_app* app, AInputEvent* event)
{
	if (!gInit)
		return 0;

	int32_t eventType = AInputEvent_getType(event);
	if (eventType == AINPUT_EVENT_TYPE_MOTION)
	{
		int id = AMotionEvent_getPointerId(event, 0);
		float x = AMotionEvent_getX(event, 0);
		float y = AMotionEvent_getY(event, 0);

		switch(AInputEvent_getSource(event))
		{
		case AINPUT_SOURCE_TOUCHSCREEN:
			int action = AKeyEvent_getAction(event) & AMOTION_EVENT_ACTION_MASK;
			switch(action)
			{
			case AMOTION_EVENT_ACTION_DOWN:
				App::Instance()->InjectTouchDown(id, x, y);
				break;
			case AMOTION_EVENT_ACTION_UP:
				App::Instance()->InjectTouchUp(id, x, y);
				break;
			case AMOTION_EVENT_ACTION_MOVE:
				App::Instance()->InjectTouchMove(id, x, y);
				break;
			case AMOTION_EVENT_ACTION_CANCEL:
				App::Instance()->InjectTouchCancel(id, x, y);
				break;
			}
			break;
		} 
	}
#if 0
	else if (eventType == AINPUT_EVENT_TYPE_KEY)
	{
		int key = AKeyEvent_getKeyCode(event);

		d_printf("Key down %d", key);

		switch (AKeyEvent_getAction(event))
		{
		case AKEY_EVENT_ACTION_DOWN:

			if (key == AKEYCODE_BACK ||
				key == AKEYCODE_DEL) {
					MGUI::InputManager::Instance()->_injectKeyDown(InputCode::KC_DELETE, 0);
			}
			else if (key == AKEYCODE_ENTER) {
				MGUI::InputManager::Instance()->_injectKeyDown(InputCode::KC_ENTER, 0);
			}
			else {
				int metaState = AKeyEvent_getMetaState(event);
				int uniValue;
				if(metaState != 0)
					uniValue = GetUnicodeChar(AKEY_EVENT_ACTION_DOWN, key, metaState);
				else
					uniValue = GetUnicodeChar(AKEY_EVENT_ACTION_DOWN, key, 0);

				MGUI::InputManager::Instance()->_injectKeyDown(InputCode::KC_NONE, uniValue);
			}
			break;
		}
	}
#endif

	return 0;
}

static void engine_handle_cmd(struct android_app* app, int32_t cmd) 
{
	struct engine* engine = (struct engine*)app->userData;
	switch (cmd) {
	case APP_CMD_SAVE_STATE:
		// The system has asked us to save our current state.  Do so.
		break;

	case APP_CMD_INIT_WINDOW:
		// The window is being shown, get it ready.
		if (engine->app->window != NULL) {
			engine_init_display(engine);
			engine_draw_frame(engine);
		}
		break;

	case APP_CMD_TERM_WINDOW:
		// The window is being hidden or closed, clean it up.
		engine_term_display(engine);
		break;

	case APP_CMD_WINDOW_RESIZED:
		if (gInit && engine->app->window != NULL) {
			int w = ANativeWindow_getWidth(engine->app->window);
			int h = ANativeWindow_getHeight(engine->app->window);
			gApp->Resize(w, h);
		}
		break;

	case APP_CMD_GAINED_FOCUS:
		// When our app gains focus, we start monitoring the accelerometer.
		if (engine->accelerometerSensor != NULL) {
			ASensorEventQueue_enableSensor(engine->sensorEventQueue,
				engine->accelerometerSensor);
			// We'd like to get 60 events per second (in us).
			ASensorEventQueue_setEventRate(engine->sensorEventQueue,
				engine->accelerometerSensor, (1000L/60)*1000);
		}

		break;
	case APP_CMD_LOST_FOCUS:
		// When our app loses focus, we stop monitoring the accelerometer.
		// This is to avoid consuming battery while not being used.
		if (engine->accelerometerSensor != NULL) {
			ASensorEventQueue_disableSensor(engine->sensorEventQueue,
				engine->accelerometerSensor);
		}
		break;

	case APP_CMD_START:
		break;

	case APP_CMD_PAUSE:
		break;

	case APP_CMD_RESUME:
		break;

	case APP_CMD_STOP:
		break;
	}
}


/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(struct android_app * app) {
    struct engine engine;

    // Make sure glue isn't stripped.
    app_dummy();

    memset(&engine, 0, sizeof(engine));
    app->userData = &engine;
	app->onAppCmd = engine_handle_cmd;
    app->onInputEvent = engine_handle_input;
    engine.app = app;

	ANDROID_ACTIVITY = app->activity;
	DATA_PATH = app->activity->internalDataPath;

	CreateApplication(&gApp);
	d_assert (gApp != NULL);

    // Prepare to monitor accelerometer
    engine.sensorManager = ASensorManager_getInstance();
    engine.accelerometerSensor = ASensorManager_getDefaultSensor(engine.sensorManager,
            ASENSOR_TYPE_ACCELEROMETER);
    engine.sensorEventQueue = ASensorManager_createEventQueue(engine.sensorManager,
            app->looper, LOOPER_ID_USER, NULL, NULL);

	// loop waiting for stuff to do.
    while (1) {
        // Read all pending events.
        int ident;
        int events;
        struct android_poll_source* source;

        // If not animating, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
        while ((ident=ALooper_pollAll(!gApp->IsPaused() ? 0 : -1, NULL, &events, (void**)&source)) >= 0) {

            // Process this event.
            if (source != NULL) {
                source->process(app, source);
            }

            // If a sensor has data, process it now.
            if (ident == LOOPER_ID_USER) {
                if (engine.accelerometerSensor != NULL) {
                    ASensorEvent event;
                    while (ASensorEventQueue_getEvents(engine.sensorEventQueue, &event, 1) > 0) {
                        //LOGI("accelerometer: x=%f y=%f z=%f", event.acceleration.x, event.acceleration.y,  event.acceleration.z);
                    }
                }
            }

            // Check if we are exiting.
            if (app->destroyRequested != 0) {
                engine_term_display(&engine);
                goto _end;
            }
        }

		engine_draw_frame(&engine);
    }

_end:
	gApp->Shutdown();
	delete gApp;

	gApp = NULL;
	gInit = false;
}
