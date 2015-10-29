

include $(CLEAR_VARS)

LOCAL_MODULE := libSLAudioSystem
LOCAL_LDLIBS := -llog -lOpenSLES

LOCAL_STATIC_LIBRARIES := libOpenAL
LOCAL_SHARED_LIBRARIES := libMCore

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../../SLAudioSystem \
	$(LOCAL_PATH)/../../SLAudioSystem/Internal \
	$(LOCAL_PATH)/../../MCore \
	$(LOCAL_PATH)/../../MCore/Platform/android \
	$(LOCAL_PATH)/../../ThirdParty/xml
	
LOCAL_SRC_FILES := \
	../../SLAudioSystem/Internal/SLSound.cpp \
	../../SLAudioSystem/SLAudioSystem.cpp

include $(BUILD_SHARED_LIBRARY)
