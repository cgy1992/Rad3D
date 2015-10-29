LOCAL_PATH := $(call my-dir)

#
# ThirdParty
#
include $(LOCAL_PATH)/ThirdParty/Android.mk

#
# Engine
#
include $(LOCAL_PATH)/MCore/Android.mk
include $(LOCAL_PATH)/MGui/Android.mk
include $(LOCAL_PATH)/MParticleFX/Android.mk
include $(LOCAL_PATH)/MPhysics/Android.mk
include $(LOCAL_PATH)/GLRenderSystem/Android.mk
include $(LOCAL_PATH)/ALAudioSystem/Android.mk
include $(LOCAL_PATH)/SLAudioSystem/Android.mk