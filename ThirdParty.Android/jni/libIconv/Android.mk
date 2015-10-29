
include $(CLEAR_VARS)

LOCAL_MODULE := libIconv

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../libIconv/include \
	$(LOCAL_PATH)/../libIconv/source

LOCAL_SRC_FILES := \
	../libIconv/source/localcharset.c \
	../libIconv/source/relocatable.c \
	../libIconv/source/iconv.c
	
include $(BUILD_STATIC_LIBRARY)
