
#
# GLRenderSystem
#
include $(CLEAR_VARS)

LOCAL_MODULE := libGLRenderSystem
LOCAL_LDLIBS := -lGLESv2
LOCAL_SHARED_LIBRARIES := libMCore

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../../MCore \
	$(LOCAL_PATH)/../../MCore/Platform/android \
	$(LOCAL_PATH)/../../GLRenderSystem \
	$(LOCAL_PATH)/../../GLRenderSystem/Internal \
	$(LOCAL_PATH)/../../ThirdParty/xml
	
LOCAL_SRC_FILES := \
	../../GLRenderSystem/GLRenderSystem.cpp \
	../../GLRenderSystem/Internal/GLIndexBuffer.cpp \
	../../GLRenderSystem/Internal/GLShaderFXManager.cpp \
	../../GLRenderSystem/Internal/GLShaderPass.cpp \
	../../GLRenderSystem/Internal/GLShaderProgram.cpp \
	../../GLRenderSystem/Internal/GLTexture.cpp \
	../../GLRenderSystem/Internal/GLRenderTarget.cpp \
	../../GLRenderSystem/Internal/GLDepthBuffer.cpp \
	../../GLRenderSystem/Internal/GLUtil.cpp \
	../../GLRenderSystem/Internal/GLVertexBuffer.cpp \
	../../GLRenderSystem/Internal/GLHWBufferManager.cpp

include $(BUILD_SHARED_LIBRARY)
