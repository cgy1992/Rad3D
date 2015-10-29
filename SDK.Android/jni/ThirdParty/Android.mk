#
# Ogg
#
include $(CLEAR_VARS)

LOCAL_MODULE := libOgg

LOCAL_SRC_FILES := ../../ThirdParty.Android/obj/local/$(TARGET_ARCH_ABI)/libOgg.a

include $(PREBUILT_STATIC_LIBRARY)

#
# libVorbis
#
include $(CLEAR_VARS)

LOCAL_MODULE := libVorbis

LOCAL_SRC_FILES := ../../ThirdParty.Android/obj/local/$(TARGET_ARCH_ABI)/libVorbis.a

include $(PREBUILT_STATIC_LIBRARY)


#
# libTheora
#
include $(CLEAR_VARS)

LOCAL_MODULE := libTheora

LOCAL_SRC_FILES := ../../ThirdParty.Android/obj/local/$(TARGET_ARCH_ABI)/libTheora.a

include $(PREBUILT_STATIC_LIBRARY)

#
# FreeType
#
include $(CLEAR_VARS)

LOCAL_MODULE := libFreeType

LOCAL_SRC_FILES := ../../ThirdParty.Android/obj/local/$(TARGET_ARCH_ABI)/libFreeType.a

include $(PREBUILT_STATIC_LIBRARY)

#
# Iconv
#
include $(CLEAR_VARS)

LOCAL_MODULE := libIconv

LOCAL_SRC_FILES := ../../ThirdParty.Android/obj/local/$(TARGET_ARCH_ABI)/libIconv.a

include $(PREBUILT_STATIC_LIBRARY)


#
# Ode
#
include $(CLEAR_VARS)

LOCAL_MODULE := libOde

LOCAL_SRC_FILES := ../../ThirdParty.Android/obj/local/$(TARGET_ARCH_ABI)/libOde.a

include $(PREBUILT_STATIC_LIBRARY)

#
# OpenAL
#
include $(CLEAR_VARS)

LOCAL_MODULE := libOpenAL

LOCAL_SRC_FILES := ../../ThirdParty.Android/obj/local/$(TARGET_ARCH_ABI)/libOpenAL.a

include $(PREBUILT_STATIC_LIBRARY)

