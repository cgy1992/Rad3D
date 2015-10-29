
#
# MPhysics
#

include $(CLEAR_VARS)

LOCAL_MODULE := libMPhysics
LOCAL_STATIC_LIBRARIES := libOde
LOCAL_SHARED_LIBRARIES := libMCore

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../../MCore \
	$(LOCAL_PATH)/../../MCore/Platform/android \
	$(LOCAL_PATH)/../../MPhysics \
	$(LOCAL_PATH)/../../MPhysics/Internal \
	$(LOCAL_PATH)/../../ThirdParty/xml \
	$(LOCAL_PATH)/../../ThirdParty/ode/include
	
	
LOCAL_SRC_FILES := \
	../../MPhysics/Internal/PhyCallback.cpp \
	\
	../../MPhysics/PhyBox.cpp \
	../../MPhysics/PhyCapsule.cpp \
	../../MPhysics/PhyContact.cpp \
	../../MPhysics/PhyCylinder.cpp \
	../../MPhysics/PhyEntity.cpp \
	../../MPhysics/PhyMesh.cpp \
	../../MPhysics/PhyMeshData.cpp \
	../../MPhysics/PhyPlane.cpp \
	../../MPhysics/PhyRay.cpp \
	../../MPhysics/PhySphere.cpp \
	../../MPhysics/PhyWorld.cpp
	
include $(BUILD_SHARED_LIBRARY)
