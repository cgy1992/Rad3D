LOCAL_PATH := $(call my-dir)

#
# SDK
#
include $(LOCAL_PATH)/SDK/Android.mk

#
# Demo
#
include $(LOCAL_PATH)/Demo01_Empty/Android.mk
include $(LOCAL_PATH)/Demo02_Mesh/Android.mk
include $(LOCAL_PATH)/Demo03_SkeletonAnimation/Android.mk
include $(LOCAL_PATH)/Demo04_Lighting/Android.mk
include $(LOCAL_PATH)/Demo05_Shading/Android.mk
include $(LOCAL_PATH)/Demo06_GUI/Android.mk
include $(LOCAL_PATH)/Demo07_ParticleFX/Android.mk
include $(LOCAL_PATH)/Demo08_Physics/Android.mk
include $(LOCAL_PATH)/Demo09_BackgroundLoad/Android.mk
include $(LOCAL_PATH)/Demo10_MultiVertexBuffer/Android.mk
include $(LOCAL_PATH)/Demo13_Sound/Android.mk
include $(LOCAL_PATH)/Demo13_Video/Android.mk
include $(LOCAL_PATH)/Demo14_RenderTarget/Android.mk
include $(LOCAL_PATH)/Demo15_Lua/Android.mk
include $(LOCAL_PATH)/Demo17_Shadow/Android.mk
include $(LOCAL_PATH)/Sample_Game/Android.mk