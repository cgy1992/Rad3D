

include $(CLEAR_VARS)

LOCAL_MODULE := libALAudioSystem
LOCAL_LDLIBS := -llog -Wl,-s

LOCAL_STATIC_LIBRARIES := libOpenAL
LOCAL_SHARED_LIBRARIES := libMCore

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../../ALAudioSystem \
	$(LOCAL_PATH)/../../ALAudioSystem/Internal \
	$(LOCAL_PATH)/../../MCore \
	$(LOCAL_PATH)/../../MCore/Platform/android \
	$(LOCAL_PATH)/../../ThirdParty/xml \
	$(LOCAL_PATH)/../../ThirdParty.Android/libOpenAL/include/AL
	
	
LOCAL_SRC_FILES := \
	../../ALAudioSystem/Internal/ALSound.cpp \
	../../ALAudioSystem/ALAudioSystem.cpp

include $(BUILD_SHARED_LIBRARY)
