#
# SDK Lib
#
include $(CLEAR_VARS)
LOCAL_MODULE := libALAudioSystem
LOCAL_SRC_FILES := ../../SDK.Android/libs/$(TARGET_ARCH_ABI)/libALAudioSystem.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libSLAudioSystem
LOCAL_SRC_FILES := ../../SDK.Android/libs/$(TARGET_ARCH_ABI)/libSLAudioSystem.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libGLRenderSystem
LOCAL_SRC_FILES := ../../SDK.Android/libs/$(TARGET_ARCH_ABI)/libGLRenderSystem.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libMGui
LOCAL_SRC_FILES := ../../SDK.Android/libs/$(TARGET_ARCH_ABI)/libMGui.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libMParticleFX
LOCAL_SRC_FILES := ../../SDK.Android/libs/$(TARGET_ARCH_ABI)/libMParticleFX.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libMPhysics
LOCAL_SRC_FILES := ../../SDK.Android/libs/$(TARGET_ARCH_ABI)/libMPhysics.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libMCore
LOCAL_SRC_FILES := ../../SDK.Android/libs/$(TARGET_ARCH_ABI)/libMCore.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libLua
LOCAL_SRC_FILES := ../../ThirdParty.Android/obj/local/$(TARGET_ARCH_ABI)/libLua.a
include $(PREBUILT_STATIC_LIBRARY)