#
#
#
include $(CLEAR_VARS)

LOCAL_MODULE := libSample_Game

LOCAL_LDLIBS := -llog -lz -landroid
LOCAL_SHARED_LIBRARIES := libMCore libGLRenderSystem libSLAudioSystem libMGui libMParticleFX libMPhysics

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../../SDK.Android/include \
	$(LOCAL_PATH)/../../ThirdParty.Android/libLua/include \
	$(LOCAL_PATH)/../../Samples/Framework.Android \
	$(LOCAL_PATH)/../../Samples/Sample_Game
	
LOCAL_SRC_FILES := \
	../../Samples/Framework.Android/stdafx.cpp \
	../../Samples/Framework.Android/NDK_Utils.cpp \
	../../Samples/Framework.Android/IME.cpp \
	../../Samples/Framework.Android/APKArchive.cpp \
	../../Samples/Framework.Android/App.cpp \
	../../Samples/Framework.Android/DebugInfo.cpp \
	../../Samples/Framework.Android/Framework.cpp \
	../../Samples/Sample_Game/Sample_Game.cpp \
	../../Samples/Sample_Game/Module.cpp \
	../../Samples/Sample_Game/ModuleDemo.cpp \
	../../Samples/Sample_Game/Engine.cpp \
	../../Samples/Sample_Game/Actor.cpp \
	../../Samples/Sample_Game/CameraController.cpp \
	../../Samples/Sample_Game/MotionComponent.cpp \
	../../Samples/Sample_Game/MoveComponent.cpp \
	../../Samples/Sample_Game/RotateComponent.cpp \
	../../Samples/Sample_Game/ShadowComponent.cpp

include $(BUILD_SHARED_LIBRARY)
