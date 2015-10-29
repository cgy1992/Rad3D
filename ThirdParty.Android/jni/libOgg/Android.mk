
include $(CLEAR_VARS)

LOCAL_MODULE     := libOgg

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../../ThirdParty/ogg/include
	
LOCAL_SRC_FILES  := \
	../../ThirdParty/ogg/source/ogg/bitwise.c \
	../../ThirdParty/ogg/source/ogg/framing.c

include $(BUILD_STATIC_LIBRARY)
